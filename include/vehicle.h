#pragma once

class Vehicle {
public:
    Vehicle(double massKg, double gravityMetersPerSecondSquared);

    void setThrust(double thrustNewtons);
    void setDisturbanceForce(double disturbanceNewtons);

    void update(double deltaTimeSeconds);

    double getMass() const;
    double getGravity() const;
    double getAltitude() const;
    double getVelocity() const;
    double getAcceleration() const;
    double getThrust() const;
    double getDisturbanceForce() const;

private:
    double massKg;
    double gravityMetersPerSecondSquared;

    double altitudeMeters;
    double velocityMetersPerSecond;
    double accelerationMetersPerSecondSquared;

    double thrustNewtons;
    double disturbanceForceNewtons;
};