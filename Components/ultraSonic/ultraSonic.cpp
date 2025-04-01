// ======================================================================
// \title  ultraSonic.cpp
// \author cristian
// \brief  cpp file for ultraSonic component implementation class
// ======================================================================

#include "Components/ultraSonic/ultraSonic.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  ultraSonic ::
    ultraSonic(const char* const compName) :
      ultraSonicComponentBase(compName)
  {

  }

  ultraSonic ::
    ~ultraSonic()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void ultraSonic ::
    TODO_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

}
