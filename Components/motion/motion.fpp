module Components {

    @ will run PID controller and necessary sensors
    active component motion {

        @ Command to turn on or off the blinking LED
        async command moveTo(
            target: U8 @< sets target distance target for PID controller
        )

        @ Reports the interval we set to blinking.
        event TargetSet(target: U8) \
            severity activity high \
            format "PID target set to {}"

        @ Reports the constants set
        event PconstantSet(consP: U32) \
            severity activity high \
            format "kP set to {}"

        @ Reports the constants set
        event IconstantSet(consI: U32) \
            severity activity high \
            format "kI set to {}"

        @ Reports the constants set
        event DconstantSet(consD: U32) \
            severity activity high \
            format "kD set to {}"

        @ Reports current encoder tick count
        event EncoderTick(ticks: I32) \
            severity activity high \
            format "Encoder tick count: {}"

        @ Port receiving calls from the rate group
        async input port run: Svc.Sched

        @ Port for writing to motor driver
        output port driv: PIDout

        @ Parameter for kP 
        param kP: U32 default 1

        @ Parameter for kI
        param kI: U32 default 1

        @ Parameter for kD
        param kD: U32 default 1

        @ reading channel A from gpio pin
        output port channelA: Drv.GpioRead

        @ reading channel B from gpio pin
        output port channelB: Drv.GpioRead

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