// ======================================================================
// \title  motorCommand.cpp
// \author cristian
// \brief  cpp file for motorCommand component implementation class
// ======================================================================

#include "Components/motorCommand/motorCommand.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  motorCommand ::
    motorCommand(const char* const compName) :
      motorCommandComponentBase(compName)
  {
    this->writeData[0] = 0x46;  // Hardcoded: direction = 'F'
    this->writeData[1] = 0x50;  // Hardcoded: speed = 80
    this->buffer.set(this->writeData, sizeof(this->writeData), 0x01);
  }

  motorCommand ::
    ~motorCommand()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for typed input ports
  // ----------------------------------------------------------------------

  void motorCommand ::
    cmndr_handler(
        FwIndexType portNum,
        U16 value
    )
  {
        // Update command value
        this->motCmndVal = value;

        // split into two bytes
        this->writeData[0] = (value >> 8) & 0xFF;  // dir
        this->writeData[1] = value & 0xFF;         // speed
    
        // Initialize buffer with the writeData array
        this->buffer.set(this->writeData, sizeof(this->writeData), 0x01);
    
        // Log the new motor command
        this->tlmWrite_motorState(value);
        this->log_ACTIVITY_HI_mtrStateSet(value);
    
        // Send command response
        //this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void motorCommand ::
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
          this->log_ACTIVITY_HI_mtrStateSet(this->motCmndVal);
      }
    }
  }

}
