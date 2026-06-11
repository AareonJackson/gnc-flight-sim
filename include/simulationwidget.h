#pragma once

#include <QElapsedTimer>
#include <QTimer>
#include <QWidget>
#include "pidcontroller.h"
#include "vehicle.h"

class QPainter;

class SimulationWidget : public QWidget {
public:
    explicit SimulationWidget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    void updateFrame();
    void drawWorld(QPainter& painter);
    void drawTelemetry(QPainter& painter);

    static constexpr unsigned int windowWidth = 900;
    static constexpr unsigned int windowHeight = 700;

    static constexpr float groundY = 620.0f;
    static constexpr float pixelsPerMeter = 4.0f;

    double targetAltitudeMeters = 100.0;

    double kp = 15.0;
    double ki = 0.0;
    double kd = 20.0;

    Vehicle vehicle;
    PIDController altitudeController;

    double hoverThrust = 0.0;
    const double minimumThrust = 0.0;
    double maximumThrust = 0.0;

    QTimer frameTimer;
    QElapsedTimer frameClock;

    const double fixedDeltaTimeSeconds = 0.02;
    const double maxFrameTimeSeconds = 0.25;

    double accumulatorSeconds = 0.0;
    double printTimerSeconds = 0.0;
    double simulationTimeSeconds = 0.0;
};

