// ======================================================================
// \title  proximityGuard.cpp
// \author Cristian
// \brief  cpp file for proximityGuard component implementation class
// ======================================================================

#include "Components/proximityGuard/proximityGuard.hpp"
#include <unistd.h> 

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  proximityGuard :: 
    proximityGuard(const char* const compName) :
      proximityGuardComponentBase(compName)
  {

  }

  proximityGuard ::
    ~proximityGuard() 
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for typed input ports
  // ----------------------------------------------------------------------

  void proximityGuard ::
    run_handler(
      FwIndexType portNum, 
      U32 context
    ) 
  {
    if (this->m_enabled){
      this->ledOut_out(0, Fw::Logic::HIGH);
    }else{
      this->ledOut_out(0, Fw::Logic::LOW);
    }
  }

  // ----------------------------------------------------------------------
  // Handler implementation for command
  // ----------------------------------------------------------------------

  void proximityGuard ::
    EnableProximity_cmdHandler(
      FwOpcodeType opCode, U32 cmdSeq, 
      Fw::On enable
    ) 
  {
    this->m_enabled = (enable == Fw::On::ON);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

}
