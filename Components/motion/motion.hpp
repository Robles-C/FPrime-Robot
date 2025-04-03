// ======================================================================
// \title  motion.hpp
// \author cristian
// \brief  hpp file for motion component implementation class
// ======================================================================

#ifndef Components_motion_HPP
#define Components_motion_HPP

#include "Components/motion/motionComponentAc.hpp"

namespace Components {

  class motion :
    public motionComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct motion object
      motion(
          const char* const compName //!< The component name
      );

      //! Destroy motion object
      ~motion();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for typed input ports
      // ----------------------------------------------------------------------
      //! Handler implementation for channelA
      //!

      //! Handler implementation for run
      //!
      //! Port receiving calls from the rate group
      void run_handler(
          FwIndexType portNum, //!< The port number
          U32 context //!< The call order
      ) override;

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command moveTo
      //!
      //! Command to turn on or off the blinking LED
      void moveTo_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq, //!< The command sequence number
          U8 target //!< sets target distance target for PID controller
      ) override;

      void parameterUpdated(FwPrmIdType id  //!< The parameter ID
      ) override;



      Fw::Logic m_lastA;
      Fw::Logic m_lastB;
      Fw::Logic m_currA;
      Fw::Logic m_currB;
      I32 m_encoderTicks = 0;
      I32 m_targetTicks;
      I32 m_integral = 0;
      I32 m_lastError = 0;
      
  };

}

#endif
