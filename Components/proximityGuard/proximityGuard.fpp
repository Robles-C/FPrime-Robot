module Components {
    @ ultrasonic sensor detects distances. It halts motion and flashes LED when robot is on a collision course

    active component proximityGuard {

        @ Command to enable or disable proximity monitoring
        async command EnableProximity(enable: Fw.On)

        @ Event emitted when an obstacle is too close
        event ObstacleDetected(distance: F32) \
        severity warning high \
        format "Obstacle detected at {} cm"

        @ Event emitted when LED is turned ON/OFF
        event LedStateChanged($state: Fw.On) \
        severity activity low \
        format "LED turned {}"

        @ Telemetry reporting current measured distance
        telemetry CurrentDistance: F32

        @ Parameter for safe distance threshold in cm
        param ThresholdDistance: F32 default 20.0

        @ Input port for scheduled checks (rate group)
        async input port run: Svc.Sched

        @ Output port to control trigger
        output port trigger: Drv.GpioWrite

        @ Output port to read from echo pin
        output port echo: Drv.GpioRead

        @ Output port to control LED
        output port ledOut: Drv.GpioWrite

        #@ Output port to motion component
        #output port col: collis

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