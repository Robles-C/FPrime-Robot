// ======================================================================
// \title  enc.cpp
// \author cristian
// \brief  cpp file for enc component implementation class
// ======================================================================

#include "Components/enc/enc.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  enc ::
    enc(const char* const compName) :
      encComponentBase(compName)
  {

  }

  enc ::
    ~enc()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void enc ::
    reset_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        bool res
    )
  {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

}
