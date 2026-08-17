//
// Created by Aareon Jackson on 6/11/26.
//

#include "simulationwidget.h"
#include <algorithm>
#include <iostream>
#include <QFont>
#include <QKeyEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QString>


SimulationWidget::SimulationWidget(QWidget* parent)
    : QWidget(parent),
      vehicle(10.0, 9.81),
      altitudeController(kp, ki, kd) {
    setWindowTitle("Flight Simulator");
    resize(static_cast<int>(windowWidth), static_cast<int>(windowHeight));
    setFocusPolicy(Qt::StrongFocus);

    hoverThrust = vehicle.getMass() * vehicle.getGravity();
    maximumThrust = hoverThrust * 2.5;

    if (telemetryLogger.start("telemetry.csv")) {
        std::cout << "Telemetry logging started: telemetry.csv" << std::endl;
    } else {
        std::cout << "Warning: failed to start telemetry logging." << std::endl;
    }

    frameClock.start();

    connect(&frameTimer, &QTimer::timeout, this, [this]() {
        updateFrame();
    });

    frameTimer.start(16);
}

void SimulationWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.fillRect(rect(), QColor(20, 20, 30));

    drawWorld(painter);
    drawTopDownView(painter);
    drawTelemetry(painter);
}

void SimulationWidget::drawWorld(QPainter& painter) {
    const Vector3 position = vehicle.getPosition();

    const float worldCenterX = static_cast<float>(width()) * 0.5f;
    const float vehicleScreenX = worldCenterX + static_cast<float>(position.x) * pixelsPerMeter;
    const float vehicleScreenY = groundY - static_cast<float>(position.z) * pixelsPerMeter;
    const float targetScreenY = groundY - static_cast<float>(targetAltitudeMeters) * pixelsPerMeter;

    painter.setPen(Qt::NoPen);

    painter.setBrush(QColor(220, 80, 80));
    painter.drawRect(0, static_cast<int>(targetScreenY), width(), 2);

    painter.setBrush(QColor(100, 220, 100));
    painter.drawRect(0, static_cast<int>(groundY), width(), 3);

    painter.setPen(QColor(90, 90, 110));
    painter.drawLine(static_cast<int>(worldCenterX), 0, static_cast<int>(worldCenterX), static_cast<int>(groundY));

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(230, 230, 80));
    painter.drawEllipse(QPointF(vehicleScreenX, vehicleScreenY), 12.0, 12.0);
}

void SimulationWidget::drawTopDownView(QPainter &painter) {
    const Vector3 position = vehicle.getPosition();

    const int mapSize = 160;
    const int mapMargin = 18;
    const int mapX = width() - mapSize - mapMargin;
    const int mapY = mapMargin;

    const QPointF mapCenter(mapX + mapSize * 0.5, mapY + mapSize * 0.5);

    const float topDownScale = 2.0f;
    const float vehicleMapX = static_cast<float>(mapCenter.x()) + static_cast<float>(position.x) * topDownScale;
    const float vehicleMapY = static_cast<float>(mapCenter.y()) - static_cast<float>(position.y) * topDownScale;

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 150));
    painter.drawRoundedRect(mapX, mapY, mapSize, mapSize, 8, 8);

    painter.setPen(QColor(90, 90, 110));
    painter.drawLine(mapX, static_cast<int>(mapCenter.y()), mapX + mapSize, static_cast<int>(mapCenter.y()));
    painter.drawLine(static_cast<int>(mapCenter.x()), mapY, static_cast<int>(mapCenter.x()), mapY + mapSize);

    painter.setPen(QColor(230, 230, 230));
    painter.drawText(mapX + 10, mapY + 20, "Top View X/Y");

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(80, 180, 255));
    painter.drawEllipse(QPointF(vehicleMapX, vehicleMapY), 6.0, 6.0);
}

void SimulationWidget::drawTelemetry(QPainter& painter) {
    painter.setPen(QColor(230, 230, 230));

    QFont telemetryFont("Roboto-Regular");
    telemetryFont.setStyleHint(QFont::SansSerif);
    telemetryFont.setPointSize(12);
    painter.setFont(telemetryFont);

    const int footerHeight = 55;
    const int footerX = 0;
    const int footerY = height() - footerHeight;
    const int footerWidth = width();

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 170));
    painter.drawRoundedRect(footerX, footerY, footerWidth, footerHeight, 8, 8);

    painter.setPen(QColor(230, 230, 230));

    const int leftPadding = 18;
    const int firstRowY = footerY + 21;
    const int secondRowY = footerY + 43;

    const Vector3 position = vehicle.getPosition();
    const Vector3 velocity = vehicle.getVelocityVector();

    const QString rowOne = QString("Time: %1 s    Pos XYZ: [%2, %3, %4] m    Vel XYZ: [%5, %6, %7] m/s    Logging: %8")
        .arg(simulationTimeSeconds, 0, 'f', 2)
        .arg(position.x, 0, 'f', 2)
        .arg(position.y, 0, 'f', 2)
        .arg(position.z, 0, 'f', 2)
        .arg(velocity.x, 0, 'f', 2)
        .arg(velocity.y, 0, 'f', 2)
        .arg(velocity.z, 0, 'f', 2)
        .arg(telemetryLogger.isLogging() ? "ON" : "OFF");

    const QString rowTwo = QString("Target Z: %1 m    Thrust: %2 N    Kp: %3    Ki: %4    Kd: %5    Gust: %6    Gust Time: %7 s")
        .arg(targetAltitudeMeters, 0, 'f', 2)
        .arg(vehicle.getThrust(), 0, 'f', 2)
        .arg(altitudeController.getKp(), 0, 'f', 2)
        .arg(altitudeController.getKi(), 0, 'f', 3)
        .arg(altitudeController.getKd(), 0, 'f', 2)
        .arg(windGustActive ? "ON" : "OFF")
        .arg(windGustTimeRemainingSeconds, 0, 'f', 2);

    painter.drawText(leftPadding, firstRowY, rowOne);
    painter.drawText(leftPadding, secondRowY, rowTwo);
}


void SimulationWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_L) {
        toggleTelemetryLogging();
    }

    if (event->key() == Qt::Key_Space) {
        windGustActive = true;
        windGustTimeRemainingSeconds = windGustDurationSeconds;
        std::cout << "Wind gust input detected" << std::endl;
    }

    if (event->key() == Qt::Key_Up) {
        targetAltitudeMeters += 10.0;
        std::cout << "Target altitude increased: " << targetAltitudeMeters << " m" << std::endl;
    }

    if (event->key() == Qt::Key_Down) {
        targetAltitudeMeters -= 10.0;
        std::cout << "Target altitude decreased: " << targetAltitudeMeters << " m" << std::endl;
    }

    if (event->key() == Qt::Key_Q) {
        kp += 1.0;
        std::cout << "Kp increased: " << kp << std::endl;
    }

    if (event->key() == Qt::Key_A) {
        kp -= 1.0;
        std::cout << "Kp decreased: " << kp << std::endl;
    }

    if (event->key() == Qt::Key_W) {
        ki += 0.1;
        std::cout << "Ki increased: " << ki << std::endl;
    }

    if (event->key() == Qt::Key_S) {
        ki -= 0.1;
        std::cout << "Ki decreased: " << ki << std::endl;
    }

    if (event->key() == Qt::Key_E) {
        kd += 1.0;
        std::cout << "Kd increased: " << kd << std::endl;
    }

    if (event->key() == Qt::Key_D) {
        kd -= 1.0;
        std::cout << "Kd decreased: " << kd << std::endl;
    }
}

void SimulationWidget::toggleTelemetryLogging() {
    if (telemetryLogger.isLogging()) {
        telemetryLogger.stop();
        std::cout << "Telemetry logging stopped" << std::endl;
        return;
    }

    if (telemetryLogger.start("telemetry.csv")) {
        std::cout << "Telemetry logging started: telemetry.csv" << std::endl;
    } else {
        std::cout << "Warning: failed to start telemetry logging." << std::endl;
    }
}

void SimulationWidget::updateFrame() {
    double frameTimeSeconds = static_cast<double>(frameClock.restart()) / 1000.0;

    if (frameTimeSeconds > maxFrameTimeSeconds) {
        frameTimeSeconds = maxFrameTimeSeconds;
    }

    targetAltitudeMeters = std::max(0.0, targetAltitudeMeters);
    kp = std::max(0.0, kp);
    ki = std::max(0.0, ki);
    kd = std::max(0.0, kd);

    altitudeController.setGains(kp, ki, kd);

    accumulatorSeconds += frameTimeSeconds;

    while (accumulatorSeconds >= fixedDeltaTimeSeconds) {
        const double pidCorrection = altitudeController.update(
            targetAltitudeMeters,
            vehicle.getAltitude(),
            fixedDeltaTimeSeconds
        );

        const double unclampedThrust = hoverThrust + pidCorrection;
        const double thrustCommand = std::clamp(unclampedThrust, minimumThrust, maximumThrust);

        if (windGustActive) {
            if (windGustTimeRemainingSeconds > 0.0) {
                windGustTimeRemainingSeconds -= fixedDeltaTimeSeconds;
                vehicle.setDisturbanceForce(windGustForceNewtons);
            } else {
                windGustActive = false;
                vehicle.setDisturbanceForce(0.0);
            }
        }
        vehicle.setThrust(thrustCommand);
        vehicle.update(fixedDeltaTimeSeconds);

        accumulatorSeconds -= fixedDeltaTimeSeconds;
        simulationTimeSeconds += fixedDeltaTimeSeconds;
        printTimerSeconds += fixedDeltaTimeSeconds;

        telemetryLogger.logSample(
            simulationTimeSeconds,
            targetAltitudeMeters,
            vehicle.getAltitude(),
            vehicle.getVelocity(),
            vehicle.getAcceleration(),
            vehicle.getThrust(),
            vehicle.getDisturbanceForce(),
            altitudeController.getKp(),
            altitudeController.getKi(),
            altitudeController.getKd(),
            windGustActive
        );

        if (printTimerSeconds >= 1.0) {
            printTimerSeconds = 0.0;

            std::cout << "Sim Time: " << simulationTimeSeconds << " s | "
                      << "Target: " << targetAltitudeMeters << " m | "
                      << "Altitude: " << vehicle.getAltitude() << " m | "
                      << "Velocity: " << vehicle.getVelocity() << " m/s | "
                      << "Acceleration: " << vehicle.getAcceleration() << " m/s^2 | "
                      << "Thrust: " << vehicle.getThrust() << " N | "
                      << "Kp:" << altitudeController.getKp()
                      << " | Ki:" << altitudeController.getKi()
                      << " | Kd:" << altitudeController.getKd()
                      << " | Disturbance Force: " << vehicle.getDisturbanceForce() << " N"
                      << " | Logging: " << (telemetryLogger.isLogging() ? "ON" : "OFF")
                      << " | Gust: " << (windGustActive ? "ON" : "OFF")
                      << " | Gust Time Remaining: " << windGustTimeRemainingSeconds << " s"
            << std::endl;
        }
    }

    update();
}