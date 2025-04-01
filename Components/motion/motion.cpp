// ======================================================================
// \title  motion.cpp
// \author cristian
// \brief  cpp file for motion component implementation class
// ======================================================================

#include "Components/motion/motion.hpp"

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
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void motion ::
    TODO_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

}
