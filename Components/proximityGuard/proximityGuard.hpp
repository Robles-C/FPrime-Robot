// ======================================================================
// \title  proximityGuard.hpp
// \author cristian
// \brief  hpp file for proximityGuard component implementation class
// ======================================================================

#ifndef Components_proximityGuard_HPP
#define Components_proximityGuard_HPP

#include "Components/proximityGuard/proximityGuardComponentAc.hpp"
#include "Fw/Time/Time.hpp"

namespace Components {

  class proximityGuard :
    public proximityGuardComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct proximityGuard object
      proximityGuard(
          const char* const compName //!< The component name
      );

      //! Destroy proximityGuard object
      ~proximityGuard();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for run
      //!
      //! Input port for scheduled checks (rate group)
      void run_handler(
          FwIndexType portNum, //!< The port number
          U32 context //!< The call order
      ) override;

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command EnableProximity
      //!
      //! Command to enable or disable proximity monitoring
      void EnableProximity_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq, //!< The command sequence number
          Fw::On enable
      ) override;

      enum class EchoState {
        IDLE,
        TRIGGERED,
        WAITING_FOR_HIGH,
        WAITING_FOR_LOW
      };

      EchoState m_echoState = EchoState::IDLE;
      Fw::Time m_startTime;
      Fw::Time m_endTime;

      bool m_enabled = false;
      bool m_braking = false;

  };

}

#endif
