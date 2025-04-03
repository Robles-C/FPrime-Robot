// ======================================================================
// \title  motorCommand.hpp
// \author cristian
// \brief  hpp file for motorCommand component implementation class
// ======================================================================

#ifndef Components_motorCommand_HPP
#define Components_motorCommand_HPP

#include "Components/motorCommand/motorCommandComponentAc.hpp"

namespace Components {

  class motorCommand :
    public motorCommandComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct motorCommand object
      motorCommand(
          const char* const compName //!< The component name
      );

      //! Destroy motorCommand object
      ~motorCommand();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for cmndr
      //!
      //! Port receiving calls from the rate group
      void cmndr_handler(
          FwIndexType portNum, //!< The port number
          U16 value
      ) override;

      //! Handler implementation for run
      //!
      //! Port receiving calls from the rate group
      void run_handler(
          FwIndexType portNum, //!< The port number
          U32 context //!< The call order
      ) override;

      U16 motCmndVal = 0;
      U8 writeData[2];
      Fw::Buffer buffer;
  };

}

#endif
