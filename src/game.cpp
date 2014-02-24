#include "game.hpp"
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

System<PositionComponent> pSys;
System<RenderComponent> rSys;
System<MovementComponent> mSys;
EntityId player;

void Game::init() {
	player = 1;
	pSys.components[player] =  new PositionComponent(20,20,player);
	rSys.components[player] = new RenderComponent("../data/face.png", pSys, player);
	mSys.components[player] = new MovementComponent(1000, 1000, 0.1f,pSys, player);
};
void Game::update(float dt) {
	mSys.components[player]->moving = false;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		//pSprite->move( 0 , -speed*dt );
		mSys.components[player]->ay = -100;
		mSys.components[player]->moving = true;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		//pSprite->move( 0 , speed*dt );
		mSys.components[player]->ay = 100;
		mSys.components[player]->moving = true;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		//pSprite->move( speed*dt , 0 );
		mSys.components[player]->ax = 100;
		mSys.components[player]->moving = true;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		//pSprite->move( -speed*dt , 0 );
		mSys.components[player]->ax = -100;
		mSys.components[player]->moving = true;
	}
	
	for (auto it = mSys.components.begin(); it != mSys.components.end(); it++) {
		it->second->update(dt);
	}
	
	if(pSys.components[player]->x < 0) {
		pSys.components[player]->x = 0;
	}
	if(pSys.components[player]->y < 0) {
		pSys.components[player]->y = 0;
	}
	
	for (auto it = rSys.components.begin(); it != rSys.components.end(); it++) {
		it->second->update();
	}
	
};

void Game::render(sf::RenderWindow& window) {
	for (auto it = rSys.components.begin(); it != rSys.components.end(); it++) {
		window.draw(it->second->sprite);
	}
}
