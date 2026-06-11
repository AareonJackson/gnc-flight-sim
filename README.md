# Flight Dynamics & GNC Simulator

## Overview
A real-time, physics-based simulation of a 1D vertical flight vehicle, built in C++. This project demonstrates foundational concepts in Guidance, Navigation, and Control (GNC) software, specifically focusing on deterministic execution and PID control laws.

The software models vehicle kinematics and uses a custom PID controller to calculate the thrust required to reach and maintain a target altitude, simulating the core responsibilities of safety-critical flight software.

## Key Features
* **Deterministic Physics Loop:** Decouples rendering from physics calculations, ensuring the vehicle state updates at a strict, fixed time step (e.g., 50Hz) regardless of graphical frame rate.
* **Custom PID Controller:** Implements a tuned Proportional-Integral-Derivative controller to handle state estimation and target acquisition.
* **Real-Time Telemetry:** Visualizes live data including altitude, velocity, current thrust, and active PID gains.
* **Disturbance Simulation:** Allows the user to inject artificial wind gusts to test the controller's recovery response.

## Architecture
The project enforces a strict separation of concerns, mimicking real-world flight software architecture:
* **`Vehicle Model`:** Handles the physical state (mass, gravity, velocity, altitude). It knows nothing about how to fly; it only accepts thrust commands and updates its kinematics using Newtonian physics.
* **`PID Controller`:** The "brain" of the system. It reads the vehicle's current altitude, compares it to the target, calculates the error, and outputs a thrust correction.
* **`Simulation Widget`:** The Qt based application layer that manages timing, keyboard input, rendering, and the fixed timestep simulation loop.

## Dependencies
* **C++17** (or higher)
* **CMake** (3.10+)
* **QT 6 Widgets** - Used for the visual interface, rendering, timing, and hardware input.

## Build Instructions
This project uses CMake for cross-platform compilation. To build the project, run the following commands:

1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `make` (or `cmake --build .` on Windows)

## Controls
* **[Up/Down Arrows]**: Adjust Target Altitude
* **[Q / A]**: Increase/Decrease Proportional (P) Gain
* **[W / S]**: Increase/Decrease Integral (I) Gain
* **[E / D]**: Increase/Decrease Derivative (D) Gain
* **[Spacebar]**: Inject Wind Gust (Apply sudden downward force)

## Future Roadmap
* [ ] Implement 2D movement (pitch and lateral thrust).
* [ ] Add a telemetry data logger to export flight data to `.csv`.
* [ ] Introduce sensor noise to test the controller against imperfect data.
