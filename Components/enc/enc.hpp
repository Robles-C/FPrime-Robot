// ======================================================================
// \title  enc.hpp
// \author cristian
// \brief  hpp file for enc component implementation class
// ======================================================================

#ifndef Components_enc_HPP
#define Components_enc_HPP

#include "Components/enc/encComponentAc.hpp"

namespace Components {

  class enc :
    public encComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct enc object
      enc(
          const char* const compName //!< The component name
      );

      //! Destroy enc object
      ~enc();


    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command reset
      //!
      //! Command to reset enc
      void reset_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq, //!< The command sequence number
          bool res
      ) override;

  };

}

#endif
