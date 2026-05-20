#include <iostream>
#include <SFML/Graphics.hpp>
#include "vehicle.h"

int main() {
    // Create a 800x600 window with a title
    sf::RenderWindow window(sf::VideoMode({800, 600}), "FlightSimulator Simulator - Setup Test");

    Vehicle vehicle(10.0, 9.81);

    const double hoverThrust = vehicle.getMass() * vehicle.getGravity();
    vehicle.setThrust(hoverThrust);
    
    sf::Clock clock;
    double printTimerSeconds = 0.0;
    
    // Main loop: runs as long as the window is open
    while (window.isOpen()) {
        // Check for poll events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        const double deltaTimeSeconds = clock.restart().asSeconds();
        
        vehicle.update(deltaTimeSeconds);

        printTimerSeconds += deltaTimeSeconds;
        if (printTimerSeconds >= 1.0) {
            printTimerSeconds = 0.0;

            std::cout << "Altitude: " << vehicle.getAltitude() << " m | "
                    << "Velocity: " << vehicle.getVelocity() << " m/s | "
                    << "Acceleration: " << vehicle.getAcceleration() << " m/s^2 | "
                    << "Thrust: " << vehicle.getThrust() << " N" << std::endl;
        }


        window.clear(sf::Color(20, 20, 30));
        window.display();
    }
    std::cout << "Simulation closed successfully" << std::endl;
    return 0;
}
