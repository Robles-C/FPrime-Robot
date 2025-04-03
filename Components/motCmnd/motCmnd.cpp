// ======================================================================
// \title  motCmnd.cpp
// \author cristian
// \brief  cpp file for motCmnd component implementation class
// ======================================================================

#include "Components/motCmnd/motCmnd.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  motCmnd ::
    motCmnd(const char* const compName) :
      motCmndComponentBase(compName)
  {
    this->writeData[0] = 0;
  }

  motCmnd ::
    ~motCmnd()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for typed input ports
  // ----------------------------------------------------------------------

  void motCmnd ::
    dist_handler(
        FwIndexType portNum,
        U16 out
    )
  {
    this->processCommand(out);
  }
  
  void motCmnd ::
    run_handler(
        FwIndexType portNum,
        U32 context
    )
  {
    if (this->isConnected_i2cWrite_OutputPort(0) && this->buffer.isValid()) {
      U8 address = 0x08;  // Example I2C device address

      //static U8 testData[1] = {'F'};
      //Fw::Buffer serBuffer(testData, sizeof(testData), 0x01);

      // Send the command stored in the buffer
      Drv::I2cStatus status = this->i2cWrite_out(0, address, this->buffer);
      
      // Log success/failure
      if (status == Drv::I2cStatus::I2C_OK) {
          //this->log_ACTIVITY_HI_mtrStateSet(this->motCmndVal);
      }
    }

  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void motCmnd ::
    SEND_CMND_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        U16 cmnd
    )
  {
   /* // Update command value 
    this->motCmndVal = cmnd;

    // split into two bytes
    this->writeData[0] = (cmnd >> 8) & 0xFF;  // dir
    this->writeData[1] = cmnd & 0xFF;         // speed

    // Initialize buffer with the writeData array
    this->buffer.set(this->writeData, sizeof(this->writeData), 0x01);

    // Log the new motor command
    this->tlmWrite_motorState(cmnd);
    this->log_ACTIVITY_HI_mtrStateSet(cmnd);*/

    // Send command response
    this->processCommand(cmnd);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void motCmnd::processCommand(U16 cmnd) {
    this->motCmndVal = cmnd;

    this->writeData[0] = (cmnd >> 8) & 0xFF;
    this->writeData[1] = cmnd & 0xFF;

    this->buffer.set(this->writeData, sizeof(this->writeData), 0x01);

    this->tlmWrite_motorState(cmnd);
    //this->log_ACTIVITY_HI_mtrStateSet(cmnd);
  }


}
