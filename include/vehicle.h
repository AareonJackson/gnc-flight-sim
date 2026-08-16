#pragma once
#include "vector3.h"

class Vehicle {
public:
    Vehicle(double massKg, double gravityMetersPerSecondSquared);

    void setThrust(double thrustNewtons);
    void setDisturbanceForce(double disturbanceNewtons);
    void setForce(const Vector3& forceNewtons);
    void setDisturbanceForce(const Vector3& disturbanceForceNewtons);

    void update(double deltaTimeSeconds);

    double getMass() const;
    double getGravity() const;
    double getAltitude() const;
    double getVelocity() const;
    double getAcceleration() const;
    double getThrust() const;
    double getDisturbanceForce() const;

    Vector3 getPosition() const;
    Vector3 getVelocityVector() const;
    Vector3 getAccelerationVector() const;
    Vector3 getForce() const;
    Vector3 getDisturbanceForceVector() const;

private:
    double massKg;
    double gravityMetersPerSecondSquared;

    Vector3 positionMeters;
    Vector3 velocityMetersPerSecond;
    Vector3 accelerationMetersPerSecondSquared;

    Vector3 forceNewtons;
    Vector3 disturbanceForceNewtons;

    double thrustNewtons;
};
