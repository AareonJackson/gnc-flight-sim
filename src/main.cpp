#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "pidcontroller.h"
#include "vehicle.h"

int main() {
    constexpr unsigned int windowWidth = 900;
    constexpr unsigned int windowHeight = 700;

    constexpr float groundY = 620.0f;
    constexpr float pixelsPerMeter = 4.0f;
    double targetAltitudeMeters = 100.0;

    // Create a 800x600 window with a title
    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}), "Flight Simulator");

    double kp = 15.0;
    double ki = 0.0;
    double kd = 8.0;

    Vehicle vehicle(10.0, 9.81);
    PIDController altitudeController(kp, ki, kd);

    const double hoverThrust = vehicle.getMass() * vehicle.getGravity();
    const double minimumThrust = 0.0;
    const double maximumThrust = hoverThrust * 2.5;
    
    sf::Clock clock;
    
    const double fixedDeltaTimeSeconds = 0.02;
    const double maxFrameTimeSeconds = 0.25;

    const double targetAltitudeChangeRate = 25.0;
    const double kpChangeRate = 5.0;
    const double kiChangeRate = 0.2;
    const double kdChangeRate = 5.0;
    
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

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    std::cout << "Wind gust input detected" << std::endl;
                }
                if (keyPressed->code == sf::Keyboard::Key::Up) {
                    targetAltitudeMeters += 10.0;
                    std::cout << "Target altitude increased: " << targetAltitudeMeters << " m" << std::endl;
                }

                if (keyPressed->code == sf::Keyboard::Key::Down) {
                    targetAltitudeMeters -= 10.0;
                    std::cout << "Target altitude decreased: " << targetAltitudeMeters << " m" << std::endl;
                }

                if (keyPressed->code == sf::Keyboard::Key::Q) {
                    kp += 1.0;
                    std::cout << "Kp increased: " << kp << std::endl;
                }

                if (keyPressed->code == sf::Keyboard::Key::A) {
                    kp -= 1.0;
                    std::cout << "Kp decreased: " << kp << std::endl;
                }

                if (keyPressed->code == sf::Keyboard::Key::W) {
                    ki += 0.1;
                    std::cout << "Ki increased: " << ki << std::endl;
                }

                if (keyPressed->code == sf::Keyboard::Key::S) {
                    ki -= 0.1;
                    std::cout << "Ki decreased: " << ki << std::endl;
                }

                if (keyPressed->code == sf::Keyboard::Key::E) {
                    kd += 1.0;
                    std::cout << "Kd increased: " << kd << std::endl;
                }

                if (keyPressed->code == sf::Keyboard::Key::D) {
                    kd -= 1.0;
                    std::cout << "Kd decreased: " << kd << std::endl;
                }
            }
        }



        double frameTimeSeconds = clock.restart().asSeconds();
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
            const double pidCorrection = altitudeController.update(targetAltitudeMeters, vehicle.getAltitude(), fixedDeltaTimeSeconds);
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
