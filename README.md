# Autonomous Drone GNC Simulator

## Overview

This project is a C++17 / Qt-based autonomous drone simulation focused on Guidance, Navigation, and Control (GNC) concepts.

The simulator began as a 1D vertical flight model and is being expanded into a 3D autonomous drone simulator. The current architecture models translational vehicle motion in three dimensions and uses PID-based position control to move toward target positions.

The long-term goal is to build a progressively more realistic autonomous drone simulation platform, including:

- 3D position control
- autonomous takeoff
- position hold
- waypoint navigation
- telemetry logging
- wind disturbance simulation
- drone-style attitude and motor modeling
- sensor simulation and state estimation

---

## Current Capabilities

### 3D Translational Vehicle Model

The vehicle tracks:

- position: `x`, `y`, `z`
- velocity: `vx`, `vy`, `vz`
- acceleration: `ax`, `ay`, `az`
- applied force: `fx`, `fy`, `fz`
- disturbance force

The current vehicle model is translational only. It does not yet simulate drone attitude, angular rates, or individual motor thrusts.

---

### PID Position Control

The simulator uses PID controllers to drive the vehicle toward a target 3D position.

Current controller structure: X position PID -> X force command Y position PID -> Y force command Z position PID -> vertical thrust correction

The vertical controller adds its correction to hover thrust: thrust = hoverThrust + verticalCorrection

This is a simplified control model intended to provide a foundation for autonomous navigation before introducing full quadrotor attitude dynamics.

---

### Real-Time Visualization

The Qt UI displays:

- side view of the vehicle using `x` and `z`
- top-down mini-map using `x` and `y`
- target position
- current position
- velocity
- thrust
- PID gain values
- wind gust status
- telemetry logging status

---

### CSV Telemetry Logging

The simulator can export flight telemetry to a CSV file.

Telemetry logging can be toggled during runtime.

The CSV output is useful for:

- plotting flight response
- evaluating controller tuning
- checking overshoot and settling behavior
- debugging simulation behavior
- later analysis in Python, MATLAB, Excel, or other tools

---

### Disturbance Simulation

A wind gust disturbance can be injected during runtime to test controller recovery.

At the current stage, the gust acts as a force disturbance on the vertical axis.

---

## Controls
- `[Up / Down]`     Target Z altitude
- `[J / K]`         Target X left/right
- `[U / O]`         Target Y forward/back
- `[L]`             Toggle telemetry logging
- `[Space]`         Vertical wind gust
- `[Q / A]`         Vertical Kp up/down
- `[W / S]`         Vertical Ki up/down
- `[E / D]`         Vertical Kd up/down

Future autonomous controls will include:
- `[T]` Autonomous takeoff
- `[H]` Hold current position
- `[M]` Start waypoint mission
- `[R]` Reset simulation

---
## Architecture

The project is organized around a simple GNC-style separation of concerns.

### Vehicle Model

The vehicle model owns the physical state of the drone-like body:

- mass
- gravity
- position
- velocity
- acceleration
- force
- disturbance force

It updates its state using Newtonian translational physics.

The vehicle model does not know about keyboard input, rendering, telemetry files, or mission logic.

---

### PID Controller

The PID controller computes control effort from: target value - current value
It is reused for position control on multiple axes.

---

### Simulation Widget

The simulation widget owns the application-level loop:

- fixed timestep physics update
- keyboard input
- Qt rendering
- telemetry display
- controller updates
- disturbance activation

The fixed timestep loop keeps simulation physics deterministic even if rendering frame timing varies.

---

### Telemetry Logger

The telemetry logger owns CSV file output.

It is intentionally separated from the vehicle and controller logic so logging can be enabled, disabled, or expanded without changing the physics model.

---

## Build Requirements

- C++17 or newer
- CMake 3.10+
- Qt 6 Widgets

---

## Build Instructions

From the project root: 
- bash mkdir
- build cd
- build cmake ..
- cmake --build .

---

## Current Development Roadmap

### Milestone 1 — 3D Translational State

Status: Complete / In Progress

- Added `Vector3`
- Vehicle supports 3D position, velocity, acceleration, and force
- UI displays side view and top-down view
- Telemetry UI shows 3D state

---

### Milestone 2 — 3-Axis Position Control

Status: Complete / In Progress

- Added target 3D position
- Added X/Y/Z PID control
- Vehicle can move toward target X/Y/Z position
- UI displays current and target position

---

### Milestone 3 — Autonomy State Machine

Status: Next

Planned modes:
- Idle: Drone remains on the ground / no autonomous target tracking
- Takeoff: Drone climbs to a safe altitude
- PositionHold: Drone holds its current target position

The goal is to allow the simulator to begin behaving like an autonomous drone instead of only responding to manual target changes.

---

### Milestone 4 — Waypoint Navigation

Planned features:

- waypoint list
- waypoint acceptance radius
- mission start command
- automatic waypoint switching
- mission completion state

---

### Milestone 5 — Drone Attitude Model

Planned features:

- roll, pitch, yaw
- angular rates
- thrust vector direction
- attitude control
- simplified quadrotor behavior

---

### Milestone 6 — Sensors and Estimation

Planned features:

- noisy GPS
- barometer
- IMU acceleration
- gyro rates
- estimated state
- controller based on estimated state rather than perfect truth

---

## Long-Term Goal

The long-term goal is to evolve this into a learning-focused autonomous drone simulator that demonstrates the major concepts used in real flight software:
- Mission Planning
- Guidance Navigation Control
- Vehicle Dynamics
- Telemetry
- Disturbance Rejection
- State Estimation

The simulator is intentionally being built incrementally so each concept remains understandable and testable.