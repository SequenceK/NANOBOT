#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 0;
	
	
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML TEST", sf::Style::Default, settings);
	
	sf::CircleShape shape(100);
	shape.setFillColor(sf::Color(100,250,50));

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
		window.draw(shape);

        // end the current frame
        window.display();
    }

    return 0;
}