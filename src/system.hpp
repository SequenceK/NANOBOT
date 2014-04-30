#ifndef SYSTEM_HPP
#define SYSTEM_HPP
#include "components.hpp"
#include "game.hpp"
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

using namespace G;



extern bool seperateX(EntityId e1, EntityId e2, World& world);
extern bool seperateY(EntityId e1, EntityId e2, World& world);


class QuadTree {
	private:
		int MAX_ENTITIES = 4;
		int MAX_LEVELS = 8;
		int level;
		std::vector<EntityId> entities;
		std::vector<QuadTree *> nodes;
		sf::Rect<float> bounds;
	public:
		QuadTree(sf::Rect<float> b, int l) :nodes(4, nullptr), bounds(b), level(l) {
		};
		
		void clear();
		void split();
		int getIndex(HitboxComponent& rect);
		int getIndex(float x, float y);
		void insert(System<HitboxComponent>& hbSys, EntityId e);
		std::vector<EntityId> retreive(std::vector<EntityId>& returnEntities, HitboxComponent& rect);
		void render(sf::RenderWindow& window);
		
};

bool overlap(HitboxComponent o1, HitboxComponent o2,float &area);
bool overlap(float x, float y, HitboxComponent o2);
bool collide(EntityId e1, EntityId e2, World& world);
bool seperateX(EntityId e1, EntityId e2, World& world);
bool seperateY(EntityId e1, EntityId e2, World& world);
bool saveHBC(std::vector<EntityId> entities, World& world);
bool loadHBC(World& world);


	

#endif