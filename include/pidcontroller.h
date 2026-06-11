#pragma once

class PIDController {
public:
    PIDController(double kp, double ki, double kd);
    double update(double targetValue, double currentValue, double deltaTimeSeconds);
    void reset();
    void setGains(double kp, double ki, double kd);
    double getKp() const;
    double getKi() const;
    double getKd() const;
    double getPreviousError() const;
    double getIntegralError() const;
    double getLastOutput() const;

private:
    double kp;
    double ki;
    double kd;
    double previousError;
    double integralError;
    double lastOutput;
    bool hasPreviousError;

};


