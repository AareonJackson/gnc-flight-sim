#include "vehicle.h"

Vehicle::Vehicle(double massKg, double gravityMetersPerSecondSquared)
    : massKg(massKg),
      gravityMetersPerSecondSquared(gravityMetersPerSecondSquared),
      altitudeMeters(0.0),
      velocityMetersPerSecond(0.0),
      accelerationMetersPerSecondSquared(0.0),
      thrustNewtons(0.0),
      disturbanceForceNewtons(0.0) {
}

void Vehicle::setThrust(double thrustNewtons) {
    this->thrustNewtons = thrustNewtons;
}

void Vehicle::setDisturbanceForce(double disturbanceNewtons) {
    this->disturbanceForceNewtons = disturbanceNewtons;
}

void Vehicle::update(double deltaTimeSeconds) {
    const double weightForceNewtons = massKg * gravityMetersPerSecondSquared;
    const double netForceNewtons = thrustNewtons - weightForceNewtons + disturbanceForceNewtons;
    accelerationMetersPerSecondSquared = netForceNewtons / massKg;
    velocityMetersPerSecond += accelerationMetersPerSecondSquared * deltaTimeSeconds;
    altitudeMeters += velocityMetersPerSecond * deltaTimeSeconds;

    if (altitudeMeters < 0.0) {
        altitudeMeters = 0.0;
        if (velocityMetersPerSecond < 0.0) {
            velocityMetersPerSecond = 0.0;
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
    return altitudeMeters;
}

double Vehicle::getVelocity() const {
    return velocityMetersPerSecond;
}

double Vehicle::getAcceleration() const {
    return accelerationMetersPerSecondSquared;
}

double Vehicle::getThrust() const {
    return thrustNewtons;
}

double Vehicle::getDisturbanceForce() const {
    return disturbanceForceNewtons;
}

