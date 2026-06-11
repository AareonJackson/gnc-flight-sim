//
// Created by Aareon Jackson on 6/11/26.
//

#include "simulationwidget.h"
#include <algorithm>
#include <iostream>
#include <QKeyEvent>
#include <QPainter>
#include <QPaintEvent>


SimulationWidget::SimulationWidget(QWidget* parent)
    : QWidget(parent),
      vehicle(10.0, 9.81),
      altitudeController(kp, ki, kd) {
    setWindowTitle("Flight Simulator");
    resize(static_cast<int>(windowWidth), static_cast<int>(windowHeight));
    setFocusPolicy(Qt::StrongFocus);

    hoverThrust = vehicle.getMass() * vehicle.getGravity();
    maximumThrust = hoverThrust * 2.5;

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

    const float vehicleScreenX = static_cast<float>(width()) * 0.5f;
    const float vehicleScreenY = groundY - static_cast<float>(vehicle.getAltitude()) * pixelsPerMeter;
    const float targetScreenY = groundY - static_cast<float>(targetAltitudeMeters) * pixelsPerMeter;

    painter.setPen(Qt::NoPen);

    painter.setBrush(QColor(220, 80, 80));
    painter.drawRect(0, static_cast<int>(targetScreenY), width(), 2);

    painter.setBrush(QColor(100, 220, 100));
    painter.drawRect(0, static_cast<int>(groundY), width(), 3);

    painter.setBrush(QColor(230, 230, 80));
    painter.drawEllipse(QPointF(vehicleScreenX, vehicleScreenY), 12.0, 12.0);
}

void SimulationWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Space) {
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

        vehicle.setThrust(thrustCommand);
        vehicle.update(fixedDeltaTimeSeconds);

        accumulatorSeconds -= fixedDeltaTimeSeconds;
        simulationTimeSeconds += fixedDeltaTimeSeconds;
        printTimerSeconds += fixedDeltaTimeSeconds;

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
                      << std::endl;
        }
    }

    update();
}