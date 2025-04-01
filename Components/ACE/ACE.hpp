// ======================================================================
// \title  ACE.hpp
// \author cristian
// \brief  hpp file for ACE component implementation class
// ======================================================================

#ifndef Components_ACE_HPP
#define Components_ACE_HPP

#include "Components/ACE/ACEComponentAc.hpp"

namespace Components {

  class ACE :
    public ACEComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct ACE object
      ACE(
          const char* const compName //!< The component name
      );

      //! Destroy ACE object
      ~ACE();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command TODO
      //!
      //! TODO
      void TODO_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq //!< The command sequence number
      ) override;

  };

}

#endif
