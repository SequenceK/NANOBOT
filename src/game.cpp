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
float speed = 130;
int cellSizeW = 32;
int cellSizeH = 32;
bool jumped = false;
bool jumping = true;
std::vector<int> running = {1,2,3,4};


void World::init() {
	player = this->createId();
	this->pSys.components[player] =  new PositionComponent(20,20,player);
	this->rSys.components[player] = new RenderComponent("../data/player.png", pSys, player);
	this->mSys.components[player] = new MovementComponent(300, 600, 60, 1000,pSys, player);
	this->hbSys.components[player] = new HitboxComponent(0,0,18,28,this->pSys,player);
	this->rSys.components[player]->frameWidth = 30;
	this->rSys.components[player]->frameHeight = 27;
	
	//this->rSys.components[player]->sprite.setScale(2,2);
	std::vector<int> test = {1,2,3,4,5,6};
	
	//this->rSys.components[player]->sprite.setScale(2,2);
	
};
void World::update(float dt, sf::RenderWindow& window) {
	if(this->paused)
		return;
	this->mSys.components[player]->ay = 600;
	if(this->mSys.components[player]->vx > 0 || this->mSys.components[player]->vx < 0 ){
		this->rSys.components[player]->playAnimation(running, 6, true);
	} else {
		this->rSys.components[player]->playAnimation(std::vector<int>{0}, 1, false, true);
	}
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if(mouseReleased)
		{
			bool occ = false;
			for (auto it = this->hbSys.components.begin(); it != this->hbSys.components.end(); it++) {
				if(it->second->position->x == (int(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x/cellSizeW)) * cellSizeW && it->second->position->y == (int(window.mapPixelToCoords(sf::Mouse::getPosition(window)).y/cellSizeH)) * cellSizeH) {
					occ = true;
				}
			}
			if(!occ)
				boxes.push_back(entities::Box((int(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x/cellSizeW)) * cellSizeW, (int(window.mapPixelToCoords(sf::Mouse::getPosition(window)).y/cellSizeH)) * cellSizeH, *this));
			mouseReleased = true;
		}
	} else {
		mouseReleased = true;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		this->pSys.components[player]->setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x, window.mapPixelToCoords(sf::Mouse::getPosition(window)).y);
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !jumped) {
		this->mSys.components[player]->vy = -300;
		jumped = true;
		jumping = true;
	}
	else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && jumped && jumping) {
		this->mSys.components[player]->vy = this->mSys.components[player]->vy/(-300);
		jumping = false;
	}
	if(this->hbSys.components[player]->overlaped && (this->hbSys.components[player]->touching & DOWN))
	{
		jumped = false;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		this->mSys.components[player]->vy = speed-1;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		this->mSys.components[player]->vx = speed;
		this->rSys.components[player]->facing = RenderComponent::RIGHT;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->mSys.components[player]->vx = -speed;
		this->rSys.components[player]->facing = RenderComponent::LEFT;
	}
	
	for (auto it = this->mSys.components.begin(); it != this->mSys.components.end(); it++) {
		it->second->update(dt);
	}
	this->hbSys.components[player]->overlaped = false;
	QuadTree qt(sf::Rect<float>(0,0,1366,768), 1);
	for (auto it = this->hbSys.components.begin(); it != this->hbSys.components.end(); it++) {
		it->second->update();
		qt.insert(this->hbSys, it->second->owner);
	}
	std::vector<EntityId> toCheck;
	for (auto it = this->hbSys.components.begin(); it != this->hbSys.components.end(); it++) {
		
		if(it->second->immovable)
			continue;
		toCheck = qt.retreive(toCheck, (*this->hbSys.components[it->first]));
		std::map<EntityId, float> overlaps;
		for (int i = 0; i < toCheck.size(); i++) {
			//std::cout <<"checking: " << toCheck[i] << "\n";
			float area=0;
			if(overlap((*this->hbSys.components[it->first]), (*this->hbSys.components[toCheck[i]]), area) && it->first != toCheck[i] )
			{
				this->hbSys.components[it->first]->overlaped = true;
				//this->rSys.components[it->first]->sprite.setColor(sf::Color(0, 255, 0));
				//collide(it->first, toCheck[i], *this);
				
				overlaps[toCheck[i]] = area;
			} else {
				//this->rSys.components[player]->sprite.setColor(sf::Color(255, 255, 255));
			}
		}
		int done=0;
		float maxA=0;
		EntityId maxC;
		while(done!=2){
			bool foundM=false;
			done=1;
			for (auto itO = overlaps.begin(); itO != overlaps.end(); itO++) {
				if(itO->second > maxA){
					maxC = itO->first;
					maxA = itO->second;
					foundM = true;
				}
				if(itO->second != 0){
					done--;
				}
			}
			if(foundM){
			collide(it->first, maxC, *this);
			overlaps[maxC] = 0;
			maxA=0;
			}
			done++;
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
		it->second->update(dt);
	}
	
};

void World::render(sf::RenderWindow& window) {
	for (auto it = this->rSys.components.begin(); it != this->rSys.components.end(); it++) {
		window.draw(it->second->sprite);
	}
};
