#include <iostream>
#include <SFML/Graphics.hpp>
#include "vehicle.h"

int main() {
    // Create a 800x600 window with a title
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Flight Simulator");

    Vehicle vehicle(10.0, 9.81);

    const double hoverThrust = vehicle.getMass() * vehicle.getGravity();
    vehicle.setThrust(hoverThrust);
    
    sf::Clock clock;
    
    const double fixedDeltaTimeSeconds = 0.02;
    const double maxFrameTimeSeconds = 0.25;
    
    double accumulatorSeconds = 0.0;
    double printTimerSeconds = 0.0;
    double simulationTimeSeconds = 0.0;
    
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

        window.clear(sf::Color(20, 20, 30));
        window.display();
    }
   
    return 0;
}
