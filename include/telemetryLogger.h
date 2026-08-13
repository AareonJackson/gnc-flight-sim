#pragma once

#include <string>
#include <fstream>


class TelemetryLogger {
public:
    TelemetryLogger();
    ~TelemetryLogger();
    bool start( const std::string& filePath);
    void stop();
    bool isLogging() const;
    void logSample(double timeSeconds,
                    double targetAltitude,
                    double altitude,
                    double velocity,
                    double acceleration,
                    double thrust,
                    double disturbanceForce,
                    double kp,
                    double ki,
                    double kd,
                    bool windGustActive);

private:
    std::ofstream file_;
    bool logging_;
};



