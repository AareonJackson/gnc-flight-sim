#pragma once

#include <string>
#include <fstream>


class telemetryLogger {
public:
    bool start( const std::string& filePath);
    void stop();
    bool isLogging() const;
    void logSample(double timeSeconds,
                    double targetAltitude,
                    double targetAltitude,
                    double altitude,
                    double velocity,
                    double thrust,
                    double pGain,
                    double iGain,
                    double dGain);

private:
    std::ofstream file_;
    bool logging_; false;
};



