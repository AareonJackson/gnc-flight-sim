#include  "pidcontroller.h"

PIDController::PIDController(double kp, double ki, double kd)
    :   kp(kp),
        ki(ki),
        kd(kd),
        previousError(0.0),
        integralError(0.0),
        lastOutput(0.0),
        hasPreviousError(false) {
}

double PIDController::update(double targetValue, double currentValue, double deltaTimeSeconds) {
    const double error = targetValue - currentValue;
    integralError += error * deltaTimeSeconds;
    double derivativeError = 0.0;
    if (hasPreviousError) {
        derivativeError = (error - previousError) / deltaTimeSeconds;
    }
    lastOutput = (kp * error) + (ki * integralError) + (kd * derivativeError);
    previousError = error;
    hasPreviousError = true;
    return lastOutput;
}

void PIDController::reset() {
    previousError = 0.0;
    integralError = 0.0;
    lastOutput = 0.0;
    hasPreviousError = false;
}

void PIDController::setGains(double kp, double ki, double kd) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

double PIDController::getKp() const {
    return kp;
}

double PIDController::getKi() const {
    return ki;
}

double PIDController::getKd() const {
    return kd;
}

double PIDController::getPreviousError() const {
    return previousError;
}

double PIDController::getIntegralError() const {
    return integralError;
}

double PIDController::getLastOutput() const {
    return lastOutput;
}


