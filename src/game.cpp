#include "game.hpp"
#include "entities.hpp"
#include "system.hpp"
#include "components.hpp"
#include "textures.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>


using namespace G;
using namespace std;

//System<PositionComponent> pSys;
//System<RenderComponent> rSys;
//System<MovementComponent> mSys;
std::vector<EntityId> boxes;

//EntityId player;
const int framerate = 60;
bool mouseReleased = true;
float speed = 60;
float gravity = 0.4;
float maxJump = 10;
float minJump = 1;
float xInit{0}, yInit{0}, xFinal{0}, yFinal{0};
int cellSizeW = 32;
int cellSizeH = 32;
bool jumped = false;
bool jumping = true;
bool renderB = true;
bool passingLI = false;
bool inSI = false;
std::vector<int> runningAnimation = {1,2,3,4};


void Game::inputUpdate()
{
	sf::Event event;
	while(this->window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
			this->running = false;
		}
		
		if(event.type == sf::Event::MouseWheelMoved)
		{
			if(event.mouseWheel.delta>0) {
				this->camera.zoom += 0.1;
			} else if (event.mouseWheel.delta<0) {
				this->camera.zoom -= 0.1;
			}
		}
		
		if (event.type == sf::Event::LostFocus)
			this->paused=true;
		if (event.type == sf::Event::GainedFocus)
			this->paused=false;

		if (event.type == sf::Event::KeyReleased)
		{
			if(event.key.code == sf::Keyboard::F1) {
				this->camera.followPlayer = this->camera.followPlayer?false:true;
			} else if(event.key.code == sf::Keyboard::F5) {
				saveHBC(boxes, world);
			} else if(event.key.code == sf::Keyboard::F3) {
				
			} else if(event.key.code == sf::Keyboard::Escape) {
				this->window.close();
				this->running = false;
			} else if( event.key.code == sf::Keyboard::Num1 ) {
				if(world.currentTex == nullptr)
				world.currentTex = &Tex::box;
				else if(world.currentTex == &Tex::box)
				world.currentTex = &Tex::tEnd;
				else if(world.currentTex == &Tex::tEnd)
				world.currentTex = &Tex::tooth;
				else
				world.currentTex = &Tex::box;
			} else if( event.key.code == sf::Keyboard::Num2 ) {
				if(!renderB)
				{
					for (auto id:boxes) {
						
						world.rSys.components[id]->render = true;
					}
					renderB = true;
				} else {
					for (auto id:boxes) {
						
						world.rSys.components[id]->render = false;
					}
					renderB = false;
				}
			}
		}
	}
	

	
};

void Game::update()
{
	
	if(world.getPositionC(world.player)->y > 1900 + 2435 && world.getPositionC(world.player)->y < 2600+2435 && world.getPositionC(world.player)->x > 900 && world.getPositionC(world.player)->x < 1100 && !inSI) {
		passingLI = true;
	} else {
		passingLI = false;
	}
	if (world.getPositionC(world.player)->y > 2600 + 2435) {
		inSI = true;
	}
	
	if(sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
		if(!this->camera.mInit){
			this->camera.mX = sf::Mouse::getPosition().x;
			this->camera.mY = sf::Mouse::getPosition().y;
			this->camera.dcX = this->camera.cX;
			this->camera.dcY = this->camera.cY;
		}
		this->camera.mInit = true;
		if(this->camera.mInit){
			this->camera.cX = sf::Mouse::getPosition().x - this->camera.mX + this->camera.dcX;
			this->camera.cY = sf::Mouse::getPosition().y - this->camera.mY + this->camera.dcY;
		}
	} else {
		if(this->camera.followPlayer) {
			this->camera.cX = this->world.pSys.components[world.player]->x;
			this->camera.cY = this->world.pSys.components[world.player]->y;
		}
		this->camera.mInit = false;
	}
	
	this->world.getMovementC(this->world.player)->ay = gravity;
	if(world.getMovementC(world.player)->vy < 0 && !(this->world.getHitboxC(this->world.player)->touching & DOWN)) {
		world.getRenderC(world.player)->playAnimation({5}, 1, false, true);
	} else if (world.getMovementC(world.player)->vy > 0 && !(this->world.getHitboxC(this->world.player)->touching & DOWN)) {
		world.getRenderC(world.player)->playAnimation({6}, 1, false, true);
	} else {	
		if(this->world.getMovementC(this->world.player)->vx > 0 || this->world.getMovementC(this->world.player)->vx < 0 ){
			this->world.getRenderC(this->world.player)->playAnimation(runningAnimation, 6, true);
		} else {
			this->world.getRenderC(this->world.player)->playAnimation(std::vector<int>{0}, 1, false, true);
		}
	}
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if(mouseReleased)
		{
			xInit = window.mapPixelToCoords(sf::Mouse::getPosition(window)).x;
			yInit = window.mapPixelToCoords(sf::Mouse::getPosition(window)).y;
			//bool occ = false;
			//for (auto it = this->world.hbSys.components.begin(); it != this->world.hbSys.components.end(); it++) {
			//	if(it->second->position->x == (int(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x/cellSizeW)) * cellSizeW && it->second->position->y == (int(window.mapPixelToCoords(sf::Mouse::getPosition(window)).y/cellSizeH)) * cellSizeH) {
			//		occ = true;
			//	}
			//}
			//if(!occ)
			//	boxes.push_back(entities::Box((int(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x/cellSizeW)) * cellSizeW, (int(window.mapPixelToCoords(sf::Mouse::getPosition(window)).y/cellSizeH)) * cellSizeH, this->world));
			mouseReleased = false;
		} else {
			xFinal = window.mapPixelToCoords(sf::Mouse::getPosition(window)).x;
			yFinal = window.mapPixelToCoords(sf::Mouse::getPosition(window)).y;
		}
	} else {
		if(!mouseReleased) {
			if(xInit < xFinal) {
				if(yInit < yFinal)
					boxes.push_back(entities::Box(xInit, yInit, xFinal-xInit, yFinal-yInit, world));
				else
					boxes.push_back(entities::Box(xInit, yFinal, xFinal-xInit, yInit-yFinal, world));
			} else {
				if(yInit < yFinal)
					boxes.push_back(entities::Box(xFinal, yInit, xInit-xFinal, yFinal-yInit, world));
				else
					boxes.push_back(entities::Box(xFinal, yFinal, xInit-xFinal, yInit-yFinal, world));
			}
		}
		mouseReleased = true;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		this->world.getPositionC(this->world.player)->setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x, window.mapPixelToCoords(sf::Mouse::getPosition(window)).y);
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !jumped) {
		this->world.getMovementC(this->world.player)->vy = -maxJump;
		jumped = true;
		jumping = true;
	}
	else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && jumped && jumping) {
		if(this->world.getMovementC(this->world.player)->vy < 0 && this->world.getMovementC(this->world.player)->vy > -minJump)
			this->world.getMovementC(this->world.player)->vy = 0;
		if(this->world.getMovementC(this->world.player)->vy < -minJump)
			this->world.getMovementC(this->world.player)->vy = this->world.getMovementC(this->world.player)->vy + minJump;
		jumping = false;
	}
	if(this->world.getHitboxC(this->world.player)->overlaped && (this->world.getHitboxC(this->world.player)->touching & DOWN))
	{
		jumped = false;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		this->world.getMovementC(this->world.player)->vy = speed;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !passingLI) {
		this->world.getMovementC(this->world.player)->vx = speed;
		this->world.getRenderC(this->world.player)->facing = RenderComponent::RIGHT;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !passingLI) {
		this->world.getMovementC(this->world.player)->vx = -speed;
		this->world.getRenderC(this->world.player)->facing = RenderComponent::LEFT;
	}
	
	this->camera.update(this->window);
	
	for (auto it = this->world.mSys.components.begin(); it != this->world.mSys.components.end(); it++) {
		it->second->update(this->frameTime);
	}
	this->world.getHitboxC(world.player)->overlaped = false;
	QuadTree qt(sf::Rect<float>(0,0,3200,2435+4200), 1);
	for (auto it = this->world.hbSys.components.begin(); it != this->world.hbSys.components.end(); it++) {
		it->second->update();
		qt.insert(this->world.hbSys, it->second->owner);
	}
	std::vector<EntityId> toCheck;
	for (auto it = this->world.hbSys.components.begin(); it != this->world.hbSys.components.end(); it++) {
		
		if(it->second->immovable)
			continue;
		toCheck = qt.retreive(toCheck, (*this->world.getHitboxC(it->first)));
		std::map<EntityId, float> overlaps;
		for (int i = 0; i < toCheck.size(); i++) {
			//std::cout <<"checking: " << toCheck[i] << "\n";
			float area=0;
			if(overlap((*this->world.getHitboxC(it->first)), (*this->world.getHitboxC(toCheck[i])), area) && it->first != toCheck[i] )
			{
				this->world.getHitboxC(it->first)->overlaped = true;
				//this->rSys.components[it->first]->sprite.setColor(sf::Color(0, 255, 0));
				//collide(it->first, toCheck[i], *this);
				
				overlaps[toCheck[i]] = area;
			} else {
				//this->world.getRenderC(player).sprite.setColor(sf::Color(255, 255, 255));
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
			if(!passingLI)
			collide(it->first, maxC, (this->world));
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
	//qt.render(this->window);
	qt.clear();
	toCheck.clear();
	if(this->world.getPositionC(world.player)->x < 0) {
		this->world.getPositionC(world.player)->x = 0;
	}
	if(this->world.getPositionC(world.player)->y < 0) {
		this->world.getPositionC(world.player)->y = 0;
	}
	
	for (auto it = this->world.rSys.components.begin(); it != this->world.rSys.components.end(); it++) {
		it->second->update(this->frameTime);
	}
	
};

sf::Sprite currentTS;

void Game::render()
{
	
	
	for (auto it = this->world.rSys.components.begin(); it != this->world.rSys.components.end(); it++) {
		if(it->second->render) {
			if(it->first == world.player && passingLI)
				continue;
			this->window.draw(it->second->sprite);
		}
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if(xInit < xFinal) {
			if(yInit < yFinal) {
				currentTS.setPosition(xInit, yInit);
				currentTS.setScale((xFinal-xInit)/32, (yFinal-yInit)/32);
			}else{
				currentTS.setPosition(xInit, yFinal);
				currentTS.setScale((xFinal-xInit)/32, (yInit-yFinal)/32);
			}
		} else {
			if(yInit < yFinal) {
				currentTS.setPosition(xFinal, yInit);
				currentTS.setScale((xInit-xFinal)/32, (yFinal-yInit)/32);
			}else{
				currentTS.setPosition(xFinal, yFinal);
				currentTS.setScale((xInit-xFinal)/32, (yInit-yFinal)/32);
			}
		}
		window.draw(currentTS);
	}
	
	//currentTS.move(-WIN_W/2, -WIN_H/2);
	
};

void Game::newGame(bool edit)
{
	this->editMode = edit;
	
	this->running = true;
	this->window.create(sf::VideoMode(this->windowW, this->windowH), "NANOBOT", sf::Style::Default);
	this->window.setFramerateLimit(60);
	this->camera.view = window.getDefaultView();
	loadTextures();
	this->world.init();
	loadHBC(world);
	currentTS.setScale(1,1);
	currentTS.setTexture(*world.currentTex);
	this->run();
};

void Game::run()
{
	sf::Clock clock{};
	
	
	sf::Time e{clock.restart()};
	float frameStep{1/60};
	float updatePerStep{0};
	float dt;
	
	while(running)
	{
		
		e = clock.restart();
		dt = e.asMilliseconds();
		frameTime = e.asSeconds();
		if(frameStep/dt > 1) {
			updatePerStep = int(frameStep/dt);
		} else {
			updatePerStep = 1;
		}
		inputUpdate();
		//window.setTitle(std::to_string(e.asMilliseconds()));
		if(!inSI)
			window.clear(sf::Color::White);
		else
			window.clear(sf::Color::Black);
		if(!paused) {
			for(int i=0; i<updatePerStep; i++) {
			
			//
					update();
					render();
			}
			
		}
		window.display();
	}
};

void Camera::update(sf::RenderWindow& window)
{
	view.setCenter(cX, cY);
	view.zoom(zoom);
	window.setView(view);
	view = window.getDefaultView();
};

void World::init() {
	/* this->player = this->createId();
	this->pSys.components[player] =  new PositionComponent(20,20,player);
	this->rSys.components[player] = new RenderComponent(Tex::player, pSys, player);
	this->mSys.components[player] = new MovementComponent(300, 600, 12, 1000,pSys, player);
	this->hbSys.components[player] = new HitboxComponent(0,0,18,28,this->pSys,player);
	this->getRenderC(this->player)->frameWidth = 30;
	this->getRenderC(this->player)->frameHeight = 27; */
	currentTex = &Tex::box;
	

	entities::Image(0,0,Tex::fBG,*this);
	entities::Image(0,2435,Tex::bBG,*this);
	player = entities::Player(*this);
	entities::Image(0,2435,Tex::bFG,*this);
	//this->world.getRenderC(player).sprite.setScale(2,2);
	std::vector<int> test = {1,2,3,4,5,6};
	
	//this->world.getRenderC(player).sprite.setScale(2,2);
	
};

