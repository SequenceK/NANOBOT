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
	//settings.antialiasingLevel = 0;
	
	
    // create the window
    sf::RenderWindow window(sf::VideoMode(G::WIN_W, G::WIN_H), "SFML TEST", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(60);
	
	sf::Texture texture;
	texture.loadFromFile("../data/lava.jpg");
	
	
	
	//System<PositionComponent> posSys;
	//System<RenderComponent> rSys;
	//EntityId p = 1;
	//posSys.components[p] = new PositionComponent(20,12,p);
	//rSys.components[p] = new RenderComponent("../data/face.png", posSys, p);
	
	G::World world;
	world.init();
	
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
	sf::Font font;
	font.loadFromFile("../data/fonts/PressStart2P.ttf");
	sf::Text FPS;
	FPS.setFont(font);
	FPS.setColor(sf::Color::White);
	FPS.setCharacterSize(12);
	float z = 1;
	sf::View view = window.getDefaultView();
	float vx=view.getCenter().x, vy=view.getCenter().y;
	bool Minit = false;
	bool followPlayer = false;
	bool F1pressed=false;
	float mx=0, my=0, dx=0, dy=0;
    // run the program as long as the window is open
    while (window.isOpen()) {
		view = window.getDefaultView();
		
		sf::Time e = clock.restart();
		G::elapsed = e.asSeconds();
		std::string s = std::to_string(1.f/G::elapsed); 
		FPS.setString("FPS: " + s);
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
		
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
			
			if(event.type == sf::Event::MouseWheelMoved)
			{
				if(event.mouseWheel.delta>0) {
					z += 0.1;
				} else if (event.mouseWheel.delta<0) {
					z -= 0.1;
				}
			}
			
			if (event.type == sf::Event::LostFocus)
				world.paused=true;
			if (event.type == sf::Event::GainedFocus)
				world.paused=false;
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
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::F1)){
			if(F1pressed)
			{
				if(followPlayer)
					followPlayer = false;
				else
					followPlayer = true;
				F1pressed = false;
			}
		} else {
			F1pressed = true;
		}
		if(sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
			if(!Minit){
				mx = sf::Mouse::getPosition().x;
				my = sf::Mouse::getPosition().y;
				dx = vx;
				dy = vy;
			}
			Minit = true;
			if(Minit){
				vx = sf::Mouse::getPosition().x - mx + dx;
				vy = sf::Mouse::getPosition().y - my + dy;
			}
		} else {
			if(followPlayer) {
				vx = world.pSys.components[1]->x;
				vy = world.pSys.components[1]->y;
			}
			Minit = false;
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				window.close();
		}
		
		view.setCenter(vx, vy );
		view.zoom(z);
		window.setView(view);
		
        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
		//window.draw(s);
		//window.draw(world.rSys.components[world.player]->sprite);
		world.update(G::elapsed, window);
		world.render(window);
		window.draw(FPS);
        // end the current frame
        window.display();
		
		
    }
    return 0;
}