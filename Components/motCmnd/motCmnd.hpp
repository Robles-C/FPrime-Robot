// ======================================================================
// \title  motCmnd.hpp
// \author cristian
// \brief  hpp file for motCmnd component implementation class
// ======================================================================

#ifndef Components_motCmnd_HPP
#define Components_motCmnd_HPP

#include "Components/motCmnd/motCmndComponentAc.hpp"

namespace Components {

  class motCmnd :
    public motCmndComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct motCmnd object
      motCmnd(
          const char* const compName //!< The component name
      );

      //! Destroy motCmnd object
      ~motCmnd();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for typed input ports
      // ----------------------------------------------------------------------
      void dist_handler(
        FwIndexType portNum, //!< The port number
        U16 out
      ) override;
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

      //! Handler implementation for command SEND_CMND 
      //!
      //! Command sent to the arduino
      void SEND_CMND_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq, //!< The command sequence number
          U16 cmnd //!< indicates the motor command sent
      ) override;

      void processCommand(U16 cmnd);

      U16 motCmndVal = 0;
      U8 writeData[2];
      Fw::Buffer buffer;
  };

}

#endif
