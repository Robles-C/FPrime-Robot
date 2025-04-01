// ======================================================================
// \title  ACE.cpp
// \author cristian
// \brief  cpp file for ACE component implementation class
// ======================================================================

#include "Components/ACE/ACE.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  ACE ::
    ACE(const char* const compName) :
      ACEComponentBase(compName)
  {

  }

  ACE ::
    ~ACE()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void ACE ::
    TODO_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

}
