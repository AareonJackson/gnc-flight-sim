#include <iostream>
#include <SFML/Graphics.hpp>

#include "pidcontroller.h"
#include "vehicle.h"

int main() {
    constexpr unsigned int windowWidth = 900;
    constexpr unsigned int windowHeight = 700;

    constexpr float groundY = 620.0f;
    constexpr float pixelsPerMeter = 4.0f;
    constexpr double targetAltitudeMeters = 100.0;

    // Create a 800x600 window with a title
    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}), "Flight Simulator");

    Vehicle vehicle(10.0, 9.81);
    PIDController altitudeController(15.0, 0.0, 8.0);

    const double hoverThrust = vehicle.getMass() * vehicle.getGravity();
    vehicle.setThrust(hoverThrust * 1.2);
    
    sf::Clock clock;
    
    const double fixedDeltaTimeSeconds = 0.02;
    const double maxFrameTimeSeconds = 0.25;
    
    double accumulatorSeconds = 0.0;
    double printTimerSeconds = 0.0;
    double simulationTimeSeconds = 0.0;

    sf::RectangleShape groundLine(sf::Vector2f(static_cast<float>(windowWidth), 3.0f));
    groundLine.setPosition({0.0f, groundY});
    groundLine.setFillColor(sf::Color(100, 220, 100));

    sf::CircleShape vehicleShape(12.0f);
    vehicleShape.setOrigin({12.0f, 12.0f});
    vehicleShape.setFillColor(sf::Color(230, 230, 80));

    sf::RectangleShape targetLine(sf::Vector2f(static_cast<float>(windowWidth), 2.0f));
    targetLine.setFillColor(sf::Color(220, 80, 80));

    // Main loop: runs as long as the window is open
    while (window.isOpen()) {
        // Check for poll events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        double frameTimeSeconds = clock.restart().asSeconds();
         if (frameTimeSeconds > maxFrameTimeSeconds) {
             frameTimeSeconds = maxFrameTimeSeconds;
         }
        
        accumulatorSeconds += frameTimeSeconds;
        
        while (accumulatorSeconds >= fixedDeltaTimeSeconds) {
            const double pidOutput = altitudeController.update(targetAltitudeMeters, vehicle.getAltitude(), fixedDeltaTimeSeconds);

            vehicle.update(fixedDeltaTimeSeconds);
            accumulatorSeconds -= fixedDeltaTimeSeconds;
            simulationTimeSeconds += fixedDeltaTimeSeconds;
            printTimerSeconds += fixedDeltaTimeSeconds;
        
        
            if (printTimerSeconds >= 1.0) {
                printTimerSeconds = 0.0;

                std::cout << "Sim Time: " << simulationTimeSeconds << " s | "
                        << "Altitude: " << vehicle.getAltitude() << " m | "
                        << "Velocity: " << vehicle.getVelocity() << " m/s | "
                        << "Acceleration: " << vehicle.getAcceleration() << " m/s^2 | "
                        << "Thrust: " << vehicle.getThrust() << " N" << std::endl;
            }
        }

        const float vehicleScreenX = static_cast<float>(windowWidth) * 0.5f;
        const float vehicleScreenY = groundY - static_cast<float>(vehicle.getAltitude()) * pixelsPerMeter;

        const float targetScreenY = groundY - static_cast<float>(targetAltitudeMeters) * pixelsPerMeter;
        vehicleShape.setPosition({vehicleScreenX, vehicleScreenY});
        targetLine.setPosition({0.0f, targetScreenY});

        window.clear(sf::Color(20, 20, 30));
        window.draw(targetLine);
        window.draw(groundLine);
        window.draw(vehicleShape);
        window.display();
    }
   
    return 0;
}
