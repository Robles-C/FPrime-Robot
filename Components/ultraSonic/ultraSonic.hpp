// ======================================================================
// \title  ultraSonic.hpp
// \author cristian
// \brief  hpp file for ultraSonic component implementation class
// ======================================================================

#ifndef Components_ultraSonic_HPP
#define Components_ultraSonic_HPP

#include "Components/ultraSonic/ultraSonicComponentAc.hpp"

namespace Components {

  class ultraSonic :
    public ultraSonicComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct ultraSonic object
      ultraSonic(
          const char* const compName //!< The component name
      );

      //! Destroy ultraSonic object
      ~ultraSonic();

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
