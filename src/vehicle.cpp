#include "vehicle.h"

Vehicle::Vehicle(double massKg, double gravityMetersPerSecondSquared)
    : massKg(massKg),
      gravityMetersPerSecondSquared(gravityMetersPerSecondSquared),
      positionMeters(),
      velocityMetersPerSecond(),
      accelerationMetersPerSecondSquared(),
      forceNewtons(),
      disturbanceForceNewtons(),
      thrustNewtons(0.0) {
}

void Vehicle::setThrust(double thrustNewtons) {
    this->thrustNewtons = thrustNewtons;
    forceNewtons.z = thrustNewtons;
}

void Vehicle::setDisturbanceForce(double disturbanceNewtons) {
    disturbanceForceNewtons.z = disturbanceNewtons;
}

void Vehicle::setForce(const Vector3& forceNewtons) {
    this->forceNewtons = forceNewtons;
    thrustNewtons = forceNewtons.z;
}

void Vehicle::setDisturbanceForce(const Vector3& disturbanceForceNewtons) {
    this->disturbanceForceNewtons = disturbanceForceNewtons;
}

void Vehicle::update(double deltaTimeSeconds) {
    const Vector3 gravityForceNewtons(0.0, 0.0, -massKg * gravityMetersPerSecondSquared);
    const Vector3 netForceNewtons = forceNewtons + disturbanceForceNewtons + gravityForceNewtons;

    accelerationMetersPerSecondSquared = netForceNewtons / massKg;
    velocityMetersPerSecond += accelerationMetersPerSecondSquared * deltaTimeSeconds;
    positionMeters += velocityMetersPerSecond * deltaTimeSeconds;

    if (positionMeters.z < 0.0) {
        positionMeters.z = 0.0;

        if (velocityMetersPerSecond.z < 0.0) {
            velocityMetersPerSecond.z = 0.0;
        }
    }
}

double Vehicle::getMass() const {
    return massKg;
}

double Vehicle::getGravity() const {
    return gravityMetersPerSecondSquared;
}

double Vehicle::getAltitude() const {
    return positionMeters.z;
}

double Vehicle::getVelocity() const {
    return velocityMetersPerSecond.z;
}

double Vehicle::getAcceleration() const {
    return accelerationMetersPerSecondSquared.z;
}

double Vehicle::getThrust() const {
    return thrustNewtons;
}

double Vehicle::getDisturbanceForce() const {
    return disturbanceForceNewtons.z;
}

Vector3 Vehicle::getPosition() const {
    return positionMeters;
}

Vector3 Vehicle::getVelocityVector() const {
    return velocityMetersPerSecond;
}

Vector3 Vehicle::getAccelerationVector() const {
    return accelerationMetersPerSecondSquared;
}

Vector3 Vehicle::getForce() const {
    return forceNewtons;
}

Vector3 Vehicle::getDisturbanceForceVector() const {
    return disturbanceForceNewtons;
}

