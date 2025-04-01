# 🤖 Tank Robot – F´-Based Differential Drive Platform

<p align="center">
  <img src="img/tank1.jpg" alt="Tank Robot" width="500"/>
</p>

📺 <strong>Tutorial Playlist:</strong>  
<a href="https://youtube.com/playlist?list=PLc2_zoZcQme6YFMZ-624sp3PF5laEbIG0&si=dT59qtRuxb4fQL4O" target="_blank">Watch on YouTube</a>


---

**Tank Robot** is a modular robotics project powered by <a href="https://github.com/nasa/fprime" target="_blank">NASA’s F´ (F Prime)</a> flight software framework. Designed for reliable motion control, obstacle sensing, and autonomous decision-making, the robot uses a differential drive system with real-time feedback via gyro, encoder, and ultrasonic sensors.


---

## 🧠 System Overview

The robot’s software is architected using the <a href="https://fprime.jpl.nasa.gov/latest/docs/user-manual/design-patterns/app-man-drv/#application-component" target="_blank"><strong>Application-Manager-Driver</strong></a> pattern, with custom F´ components for motion control, high-level logic, and environment sensing.

### Components

| Component     | Role                                                                 |
|---------------|----------------------------------------------------------------------|
| `ACE`         | Central mission controller that coordinates movement modes           |
| `MotC`        | Motion controller: reads sensors, runs PID, and sends motor commands |
| `ultraSonic`  | Obstacle detection using an ultrasonic sensor                        |
| `motCmnd`     | Low-level component that sends `U16`-encoded I²C commands to Romeo   |

---

## 📦 Hardware Integration

- **Romeo board**: Drives motors based on I²C commands from Raspberry Pi
- **MPU-6050**: Provides gyro data for heading PID
- **Rotational Encoder**: Tracks distance for straight-line motion
- **Ultrasonic Sensor**: Measures proximity for obstacle avoidance
- **Raspberry Pi 4**: Runs the F´ flight software stack

---

## 🔁 Key Features

- Modular F´ component architecture
- I²C command packing: 1 byte direction control + 1 byte speed control
- PID-controlled turning using gyro
- PID-controlled movements using encoder
- Obstacle detection and LED response
- Rate-group-based scheduling for precise timing
