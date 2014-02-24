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

	typedef unsigned long EntityId;
	static EntityId INDEX = 1;
	static int WIN_H = 600;
	static int WIN_W = 800;

	
	class Component {
		public:
			Component(EntityId id) : owner(id) {};
			EntityId owner;
	};

	template <typename C> class System {
		public:
			std::map<EntityId, C * > components;
	};

	class PositionComponent : public Component {
		public:
			float x, y;
			PositionComponent(float xx, float yy, EntityId id) : x(xx), y(yy), Component(id) {};
	};
	
	class RenderComponent : public Component {
		public :
			PositionComponent * position;
			sf::Sprite sprite;
			sf::Texture texture;
			RenderComponent(const std::string & filename, System<PositionComponent>& posSys, EntityId id) : Component(id), position(posSys.components[owner]) {
				texture.loadFromFile(filename);
				sprite.setTexture(texture);
				sprite.setPosition(position->x, position->y);
			};
			
			void update() {
				sprite.setPosition(position->x, position->y);
			};
	};
	
	class MovementComponent : public Component {
		public :
			PositionComponent * position;
			float vx=0, vy=0, ax=0, ay=0, maxVx=0, maxVy=0, drag = 0;
			bool moving = false;
			MovementComponent(float mvx, float mvy, float d, System<PositionComponent>& posSys, EntityId id) : maxVx(mvx), maxVy(mvy), drag(d), Component(id), position(posSys.components[owner]) {
			};
			void update(float dt) {
				if(!moving) {
					ax = 0;
					ay = 0;
				}
				position->x += vx*dt;
				position->y += vy*dt;
				vx += ax - vx*drag;
				vy += ay - vy*drag;
				if((std::abs(vx) > std::abs(maxVx))) {
					if( vx < 0 ){
						vx = -maxVx;
					} else if( vx > 0) {
						vx = maxVx;
					}
				}
				if((std::abs(vy) > std::abs(maxVy))) {
					if( vy < 0 ){
						vy = -maxVy;
					} else if( vy > 0) {
						vy = maxVy;
					}
				}
			};
	};
	
	//class InputComponent
	
	
	class Game {
		public:
		
			System<PositionComponent> pSys;
			System<RenderComponent> rSys;
			System<MovementComponent> mSys;
			EntityId player;
			sf::RenderWindow * window;
			
			
			void update(float dt);
			void init();
			void render(sf::RenderWindow& window);
	};
	
	
	//void render(sf::RenderWindow window, sf::Drawable &obj);
	
}
/*
class Entity {
	public:
		int componentIndex = 1;
		EntityID ID;
		std::vector<int> ComponentsID;
};

class Component {
	public:
		EntityID owner;
		int ID;
		std::string Ctype;
};

class PositionComponent : public Component {
	public:
		float x, y;
		PositionComponent(float xx, float yy) : x(xx) , y(yy) {
		}
		PositionComponent();
};

//Entity e = world.addEntity();
//world.addComponent(e, new PostitionComponent(x,y));

template <class C> class ComponentTable {
	std::vector<C> components;
};

class EntityManager {
	private :
		//double long INDEX = 1;
		//std::vector<Entity> entities;
		//std::vector<Component> components;
	public :
		double long INDEX = 1;
		std::vector<Entity> entities;
		std::vector<Component*> components;
		bool addEntity(Entity& e);
		bool removeEntity(EntityID id);
		Entity getEntity(Entity e);
		bool addComponent(Entity& e, Component* C);
		bool removeComponent(Entity e, int cID);
		//Component getComponent(
};*/


#endif