#ifndef GAME_HPP
#define GAME_HPP
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <cmath>
#include <iostream>
#include "components.hpp"


/*

Entity: contains unsigned long integer

Component: contains an entity's ID(owner)
	|_ Position Component: contains X and Y
	|_ Velocity Component: contains X's velocity and Y's velocity
	|_ etc.
	
or 
		  ----> data
Component |
		  ----> behavior
	
how the game engine will work:
	mainLoop -> update -> postion system update -> render system update -> etc.
position system update:
	check position components and checks velocity.
render system update:
	check render components and position components and renders.

*/
namespace G {
	static EntityId INDEX = 1;
	static int WIN_H = 600;
	static int WIN_W = 800;
	static float elapsed = 0;
	
	
	struct Camera {
		sf::View view;
		float zoom{1.f};
		float cameraSpeed{10};
		bool followPlayer{true};
		float cX{}, cY{}, dcY{}, dcX{}, mX{}, mY{};
		bool mInit{false};
		void update(sf::RenderWindow& window);
	};
	
	struct World {
		EntityId eIndex = 0;
		System<PositionComponent> pSys;
		System<RenderComponent> rSys;
		System<MovementComponent> mSys;
		System<HitboxComponent> hbSys;
		EntityId player;
		sf::RenderWindow * window;
		sf::Texture * currentTex;
		//void update(float dt, sf::RenderWindow& window);
		void init();
		//void render(sf::RenderWindow& window); */
		EntityId createId(){
			eIndex++;
			return eIndex;
		};
		
		PositionComponent* 	getPositionC(EntityId e) 	{ return pSys.components[e]; };
		RenderComponent*	getRenderC(EntityId e) 		{ return rSys.components[e]; };
		MovementComponent*	getMovementC(EntityId e)	{ return mSys.components[e]; };
		HitboxComponent*	getHitboxC(EntityId e) 		{ return hbSys.components[e]; };
		
	};	
	//void render(sf::RenderWindow window, sf::Drawable &obj);
	
	struct Game {
		unsigned int windowW{800};
		unsigned int windowH{600};
		sf::ContextSettings Csettings{};
		sf::RenderWindow window;
		sf::Font font;
		World world;
		Camera camera;
		
		bool running{false};
		bool paused{false};
		bool editMode{false};
		
		
		float frameTime{};
		
		void newGame(bool edit);
		void run();
		
		void inputUpdate();
		void update();
		void render();
		
	};
};



#endif