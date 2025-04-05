// ======================================================================
// \title  motion.cpp
// \author cristian
// \brief  cpp file for motion component implementation class
// ======================================================================

#include "Components/motion/motion.hpp"
#include <cmath>
#include <algorithm> 

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  motion ::
    motion(const char* const compName) :
      motionComponentBase(compName)
  {

  }

  motion ::
    ~motion()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for typed input ports
  // ----------------------------------------------------------------------
 

  void motion ::
    run_handler(
        FwIndexType portNum,
        U32 context
    )
  {
    Fw::ParamValid isValid = Fw::ParamValid::INVALID;
    U32 kP = this->paramGet_kP(isValid);
    U32 kI = this->paramGet_kI(isValid);
    U32 kD = this->paramGet_kD(isValid);
    
    //Read encoder pins
    FW_ASSERT((isValid != Fw::ParamValid::INVALID) && (isValid != Fw::ParamValid::UNINIT), static_cast<FwAssertArgType>(isValid));
    
    if (this->isConnected_channelA_OutputPort(0)) {
      this->channelA_out(0, this->m_currA);
    }
    if (this->isConnected_channelB_OutputPort(0)) {
      this->channelB_out(0, this->m_currB);
    }

    //Decode quadrature encoder
    U8 lastState = (static_cast<U8>(this->m_lastA) << 1) | static_cast<U8>(this->m_lastB);
    U8 currState = (static_cast<U8>(this->m_currA) << 1) | static_cast<U8>(this->m_currB);

    // transition table: [prev][curr] = -1, 0, +1
    const I8 transitionTable[4][4] = {
      {  0, -1, +1,  0 },  // 00 -> ...
      { +1,  0,  0, -1 },  // 01 -> ...
      { -1,  0,  0, +1 },  // 10 -> ...
      {  0, +1, -1,  0 }   // 11 -> ...
    };

    // Determine direction
    I8 delta = transitionTable[lastState][currState];
    this->m_encoderTicks += delta;

    // Only log on change
    if (delta != 0) {
      //this->log_ACTIVITY_HI_EncoderTick(this->m_encoderTicks);
    }

    // Update last state
    this->m_lastA = this->m_currA;
    this->m_lastB = this->m_currB;

    // PID calculation
    I32 error = this->m_targetTicks - this->m_encoderTicks;

    // Update integral with clamping
    this->m_integral += error;
    this->m_integral = std::max(-10, std::min(this->m_integral, 10));  

    // Derivative
    I32 derivative = error - this->m_lastError;
    this->m_lastError = error;

    // Compute control
    I32 control = (kP * error) + (kI * this->m_integral) + (kD * derivative);

    // Clamp control to motor bounds
    control = std::max(-255, std::min(control, 255));

    // Convert to motor command format
    U8 speed = static_cast<U8>(std::abs(control));
    U8 dir = (control >= 0) ? 'F' : 'B';
    U16 packed = (static_cast<U16>(dir) << 8) | speed;


   if (this->isConnected_driv_OutputPort(0)) {
       this->driv_out(0, packed);
   }
  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void motion ::
    moveTo_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        U8 target
    )
  {
    // TODO
    this->m_targetTicks = target;
    this->log_ACTIVITY_HI_TargetSet(target);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void motion :: 
    parameterUpdated (
      FwPrmIdType id
    )
  {
    Fw::ParamValid isValid = Fw::ParamValid::INVALID;
    switch (id) {
      case PARAMID_KP: {
          const U32 kP = this->paramGet_kP(isValid);
          FW_ASSERT(isValid == Fw::ParamValid::VALID, static_cast<FwAssertArgType>(isValid));
          this->log_ACTIVITY_HI_PconstantSet(kP);
          break;
      }
      case PARAMID_KI: {
        const U32 kI = this->paramGet_kP(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, static_cast<FwAssertArgType>(isValid));
        this->log_ACTIVITY_HI_IconstantSet(kI);
        break;
      }
      case PARAMID_KD: {
        const U32 kD = this->paramGet_kP(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, static_cast<FwAssertArgType>(isValid));
        this->log_ACTIVITY_HI_DconstantSet(kD);
        break;
      }
      default:
          FW_ASSERT(0, static_cast<FwAssertArgType>(id));
          break;
    }
  }
}