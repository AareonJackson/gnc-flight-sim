#pragma once

#include "../include/telemetryLogger.h"
#include <iomanip>


TelemetryLogger::TelemetryLogger()
    : logging_(false) {
}

TelemetryLogger::~TelemetryLogger() {
    stop();
}

bool TelemetryLogger::start(const std::string& filePath) {
    stop();

    file_.open(filePath);

    if (!file_.is_open()) {
        logging_ = false;
        return false;
    }

    logging_ = true;
    file_ << "timeSeconds,"
         << "targetAltitude,"
         << "altitude,"
         << "velocity,"
         << "acceleration,"
         << "thrust,"
         << "disturbanceForce,"
         << "kp,"
         << "ki,"
         << "kd,"
         << "windGustActive\n";

    return true;
}


void TelemetryLogger::stop() {
    if (file_.is_open()) {
        file_.flush();
        file_.close();
    }

    logging_ = false;
}

bool TelemetryLogger::isLogging() const {
    return logging_;
}

void TelemetryLogger::logSample(double timeSeconds,
                                double targetAltitude,
                                double altitude,
                                double velocity,
                                double acceleration,
                                double thrust,
                                double disturbanceForce,
                                double kp,
                                double ki,
                                double kd,
                                bool windGustActive) {
    if (!logging_ || !file_.is_open()) {
        return;
    }

    file_ << std::fixed << std::setprecision(6)
          << timeSeconds << ','
          << targetAltitude << ','
          << altitude << ','
          << velocity << ','
          << thrust << ','
          << disturbanceForce << ','
          << kp << ','
          << ki << ','
          << kd << ','
          << (windGustActive ? 1 : 0)
          << '\n';
}
