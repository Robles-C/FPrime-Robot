module Components {
    @ commands for motors
    active component motCmnd {

        @ Command sent to the arduino manually through gds
        async command SEND_CMND(
            cmnd: U16 @< indicates the motor command sent
        ) 

        @ Reports the state we set to motor.
        event mtrStateSet(cmnd: U16) \
            severity activity high \
            format "Set motor state to {}." 
        
        @ telemetry motor state
        telemetry motorState: U16

        @ Blinking interval in rate group ticks
        param BLINK_INTERVAL: U32 default 1

        @ Port receiving calls from the rate group
        async input port run: Svc.Sched

        @ Port for recieving command from motion
        async input port dist: PIDout

        @ Port sending data via I2C
        output port i2cWrite: Drv.I2c

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        # @ Example async command
        # async command COMMAND_NAME(param_name: U32)

        # @ Example telemetry counter
        # telemetry ExampleCounter: U64

        # @ Example event
        # event ExampleStateEvent(example_state: Fw.On) severity activity high id 0 format "State set to {}"

        # @ Example port: receiving calls from the rate group
        # sync input port run: Svc.Sched

        # @ Example parameter
        # param PARAMETER_NAME: U32

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}