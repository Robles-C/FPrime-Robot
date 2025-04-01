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
