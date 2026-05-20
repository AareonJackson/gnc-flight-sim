#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
    // Create a 800x600 window with a title
    sf::RenderWindow window(sf::VideoMode({800, 600}), "FlightSimulator Simulator - Setup Test");

    // Main loop: runs as long as the window is open
    while (window.isOpen()) {
        // Check for poll events
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            // Close window if the escape key is pressed
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
            }
        }
        // Clear the screen with a specific color
        window.clear(sf::Color(40, 40, 40));

        // TODO: Draw the drone and target line here later

        // Display the render to the window
        window.display();
    }
    std::cout << "Simulation closed successfully" << std::endl;
    return 0;
}
