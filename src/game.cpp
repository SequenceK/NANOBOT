#include "game.hpp"
#include "entities.hpp"
#include "system.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

/* bool EntityManager::addEntity(Entity& e) {
	e.ID = this->INDEX;
	INDEX++;
	this->entities.push_back(e);
	return true;
};

bool EntityManager::addComponent(Entity& e, Component* c) {
	c->owner = e.ID;
	c->ID = e.componentIndex;
	e.componentIndex++;
	this->components.push_back(c);
	return true;
} */
using namespace G;

//System<PositionComponent> pSys;
//System<RenderComponent> rSys;
//System<MovementComponent> mSys;
std::vector<EntityId> boxes;
EntityId player;
bool mouseReleased = true;
float speed = 400;
int cellSize = 32;
bool jumped = false;

void World::init() {
	player = this->createId();
	this->pSys.components[player] =  new PositionComponent(20,20,player);
	this->rSys.components[player] = new RenderComponent("../data/face.png", pSys, player);
	this->mSys.components[player] = new MovementComponent(300, 300, 0.5,pSys, player);
	this->hbSys.components[player] = new HitboxComponent(0,0,32,32,this->pSys,player);
	
	//this->rSys.components[player]->sprite.setScale(2,2);
	
};
void World::update(float dt, sf::RenderWindow& window) {

	this->mSys.components[player]->ay = 80;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if(mouseReleased)
		{
			bool occ = false;
			for (auto it = this->hbSys.components.begin(); it != this->hbSys.components.end(); it++) {
				if(it->second->position->x == (int(sf::Mouse::getPosition(window).x/cellSize)) * 32 && it->second->position->y == (int(sf::Mouse::getPosition(window).y/cellSize)) * 32) {
					occ = true;
				}
			}
			if(!occ)
				boxes.push_back(entities::Box((int(sf::Mouse::getPosition(window).x/cellSize)) * 32, (int(sf::Mouse::getPosition(window).y/cellSize)) * 32, *this));
			mouseReleased = false;
		}
	} else {
		mouseReleased = true;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		this->pSys.components[player]->x = sf::Mouse::getPosition(window).x;
		this->pSys.components[player]->y = sf::Mouse::getPosition(window).y;
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !jumped) {
		this->mSys.components[player]->vy -= speed*2;
		jumped = true;
	}
	if(this->hbSys.components[player]->overlaped )//this->hbSys.components[player]->touching == TOP)
	{
		jumped = false;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		this->mSys.components[player]->vy = speed;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		this->mSys.components[player]->vx = speed;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->mSys.components[player]->vx = -speed;
	}
	
	for (auto it = this->mSys.components.begin(); it != this->mSys.components.end(); it++) {
		it->second->update(dt);
	}
	this->hbSys.components[player]->overlaped = false;
	QuadTree qt(sf::Rect<float>(0,0,1366/2,768/2), 1);
	for (auto it = this->hbSys.components.begin(); it != this->hbSys.components.end(); it++) {
		qt.insert(this->hbSys, it->second->owner);
	}
	std::vector<EntityId> toCheck;
	for (auto it = this->hbSys.components.begin(); it != this->hbSys.components.end(); it++) {
		
		if(!it->second->moveable)
			continue;
		toCheck = qt.retreive(toCheck, (*this->hbSys.components[it->first]));
	
		for (int i = 0; i < toCheck.size(); i++) {
			//std::cout <<"checking: " << toCheck[i] << "\n";
			if(overlap((*this->hbSys.components[it->first]), (*this->hbSys.components[toCheck[i]])) && it->first != toCheck[i] )
			{
				this->hbSys.components[it->first]->overlaped = true;
				//this->rSys.components[it->first]->sprite.setColor(sf::Color(0, 255, 0));
				collide(it->first, toCheck[i], *this);
			} else {
				//this->rSys.components[player]->sprite.setColor(sf::Color(255, 255, 255));
			}
		}
	/* if(this->hbSys.components[e]->overlaped)
	{
		this->rSys.components[e]->sprite.setColor(sf::Color(0, 255, 0));
	} else {
		this->rSys.components[e]->sprite.setColor(sf::Color(255, 255, 255));
	}*/
	
	}
	qt.render(window);
	qt.clear();
	if(this->pSys.components[player]->x < 0) {
		this->pSys.components[player]->x = 0;
	}
	if(this->pSys.components[player]->y < 0) {
		this->pSys.components[player]->y = 0;
	}
	
	for (auto it = this->rSys.components.begin(); it != this->rSys.components.end(); it++) {
		it->second->update();
	}
	
};

void World::render(sf::RenderWindow& window) {
	for (auto it = this->rSys.components.begin(); it != this->rSys.components.end(); it++) {
		window.draw(it->second->sprite);
	}
};
