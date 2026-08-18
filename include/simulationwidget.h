#pragma once

#include <QElapsedTimer>
#include <QTimer>
#include <QWidget>
#include "pidcontroller.h"
#include "telemetryLogger.h"
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
    void drawTopDownView(QPainter& painter);
    void toggleTelemetryLogging();

    static constexpr unsigned int windowWidth = 900;
    static constexpr unsigned int windowHeight = 700;

    static constexpr float groundY = 620.0f;
    static constexpr float pixelsPerMeter = 4.0f;

    Vector3 targetPositionMeters = Vector3(0.0, 0.0, 100.0);


    double horizontalKp = 2.0;
    double horizontalKi = 0.0;
    double horizontalKd = 4.0;

    double verticalKp = 15.0;
    double verticalKi = 0.0;
    double verticalKd = 20.0;

    Vehicle vehicle;
    PIDController xPositionController;
    PIDController yPositionController;
    PIDController zPositionController;
    TelemetryLogger telemetryLogger;

    double hoverThrust = 0.0;
    const double minimumThrust = 0.0;
    double maximumThrust = 0.0;
    const double maximumHorizontalForce = 60.0;
    const double windGustForceNewtons = -250.0;
    const double windGustDurationSeconds = 0.25;
    double windGustTimeRemainingSeconds = 0.0;
    bool windGustActive = false;


    QTimer frameTimer;
    QElapsedTimer frameClock;

    const double fixedDeltaTimeSeconds = 0.02;
    const double maxFrameTimeSeconds = 0.25;

    double accumulatorSeconds = 0.0;
    double printTimerSeconds = 0.0;
    double simulationTimeSeconds = 0.0;
};

