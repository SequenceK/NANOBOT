#include "game.hpp"
#include "entities.hpp"
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

void Game::init() {
	player = this->createId();
	this->pSys.components[player] =  new PositionComponent(20,20,player);
	this->rSys.components[player] = new RenderComponent("../data/face.png", pSys, player);
	this->mSys.components[player] = new MovementComponent(400, 400, 0.3,pSys, player);
	
	//this->rSys.components[player]->sprite.setScale(2,2);
	
};
void Game::update(float dt, sf::RenderWindow& window) {
	this->mSys.components[player]->moving = false;
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if(mouseReleased)
		{
			boxes.push_back(entities::Box(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, *this));
			mouseReleased = false;
		}
	} else {
		mouseReleased = true;
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		//pSprite->move( 0 , -speed*dt );
		this->mSys.components[player]->ay = -100;
		this->mSys.components[player]->moving = true;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		//pSprite->move( 0 , speed*dt );
		this->mSys.components[player]->ay = 100;
		this->mSys.components[player]->moving = true;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		//pSprite->move( speed*dt , 0 );
		this->mSys.components[player]->ax = 100;
		this->mSys.components[player]->moving = true;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		//pSprite->move( -speed*dt , 0 );
		this->mSys.components[player]->ax = -100;
		this->mSys.components[player]->moving = true;
	}
	
	for (auto it = this->mSys.components.begin(); it != this->mSys.components.end(); it++) {
		it->second->update(dt);
	}
	
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

void Game::render(sf::RenderWindow& window) {
	for (auto it = this->rSys.components.begin(); it != this->rSys.components.end(); it++) {
		window.draw(it->second->sprite);
	}
};
