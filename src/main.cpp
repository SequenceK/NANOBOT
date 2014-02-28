#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <typeinfo>
#include "Player.hpp"
#include "game.hpp"

int main()
{
	sf::Clock clock;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 0;
	
	
    // create the window
    sf::RenderWindow window(sf::VideoMode(G::WIN_W, G::WIN_H), "SFML TEST", sf::Style::Fullscreen, settings);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
	
	sf::Texture texture;
	texture.loadFromFile("../data/lava.jpg");
	
	
	
	//System<PositionComponent> posSys;
	//System<RenderComponent> rSys;
	//EntityId p = 1;
	//posSys.components[p] = new PositionComponent(20,12,p);
	//rSys.components[p] = new RenderComponent("../data/face.png", posSys, p);
	
	G::Game game;
	game.init();
	
	//RenderComponent * pSprite = rSys.components[p]; 
	//pSprite->sprite.setScale(2,2);
	//std::cout << posSys.components[p]->x;
	
	//Player s(posSys.components[p]->x,posSys.components[p]->y,texture);
	//s.init();
	
	
	
	//sf::Sprite s(texture);
	//s.setScale(16/(s.getGlobalBounds().width),16/(s.getGlobalBounds().height));
	//s.scale(2,2);
	//s.setTexture(texture);
	//s.setTextureRect(sf::IntRect(10,10,50,50));
	
	int speed = 100;
	
    // run the program as long as the window is open
    while (window.isOpen()) {
		sf::Time e = clock.restart();
		float dt = e.asSeconds();
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
		
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
/* 			if(event.type == sf::Event::KeyPressed) {
				switch(event.key.code){
					case sf::Keyboard::W:
						s.move(0,-1);
					case sf::Keyboard::S:
						s.move(0,1);
					case sf::Keyboard::A:
						s.move(-1,0);
					case sf::Keyboard::D:
						s.move(1,0);
					}
			} */
			
			
			
        }
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				window.close();
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				//pSprite->move( 0 , -speed*dt );
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				//pSprite->move( 0 , speed*dt );
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				//pSprite->move( speed*dt , 0 );
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				//pSprite->move( -speed*dt , 0 );
		}
		game.update(dt, window);
        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
		//window.draw(s);
		//window.draw(game.rSys.components[game.player]->sprite);
		game.render(window);
        // end the current frame
        window.display();
		
    }
    return 0;
}