#pragma once

#include "../include/telemetryLogger.h"
#include <iomanip>


bool TelemetryLogger::start(const std::string& filePath) {
    file_.open(filePath);

    if (!file_.is_open()) {
        logging_ = false;
        return false;
    }

    logging_ = true;
    file_ << "time_seconds, target_altitude,altitude,velocity,thrust,p_gain,i_gain,d_gain\n";

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
                                double thrust,
                                double pGain,
                                double iGain,
                                double dGain) {
    if (!logging_ || !file_.is_open()) {
        return;
    }

    file_ << std::fixed << std::setprecision(6)
          << timeSeconds << ','
          << targetAltitude << ','
          << altitude << ','
          << velocity << ','
          << thrust << ','
          << pGain << ','
          << iGain << ','
          << dGain << '\n';
}
