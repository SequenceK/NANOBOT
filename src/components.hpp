
#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <cmath>
#include <iostream>


typedef unsigned long EntityId;

enum Touching {
		NONE = 0x0000, UP=0x0001, LEFT=0x0100, DOWN=0x0010, RIGHT=0x1000
	};
	
template <typename C> class System {
		public:
			std::map<EntityId, C * > components;
};

class Component {
		public:
			Component(EntityId id) : owner(id) {};
			EntityId owner;
};

class PositionComponent : public Component {
	public:
		float x=0, y=0;
		float dx=0, dy=0;
		//bool changed;
		PositionComponent(float xx, float yy, EntityId id) : x(xx), y(yy), dx(xx), dy(yy), Component(id) {};
		void setPosition(float xx, float yy) {
			//std::cout << dy << " " << y << "\n";
			//if( xx != x) {
				dx = x;
			//}
			//if( yy != y) {
				dy = y;
			//}
			x = xx;
			y = yy;
		};
};

class RenderComponent : public Component {
	public :
		
		struct Animation {
			std::vector<int> frames={0};
			int currentFrame=0;
			int currentIt = 0;
			float speed=1;
			bool loop=true;
			bool played=false;
		};
		
		enum Facing { LEFT=false, RIGHT=true };
	
		Animation currentAnimation;
		bool playingAnimation = false;
		bool facing = RIGHT;
		bool render = true;
		PositionComponent * position;
		sf::Sprite sprite;
		//sf::Texture texture;
		//std::map<std::String, std::vector<std::vector<int>> animations;
		int frameWidth=0, frameHeight=0;
		float offsetX=0, offsetY=0;
		float frameTimer=0;
		RenderComponent(sf::Texture & texture, System<PositionComponent>& posSys, EntityId id) : Component(id), position(posSys.components[owner]) {
			sprite.setTexture(texture);
			sprite.setPosition(position->x + offsetX, position->y + offsetY);
			if(frameWidth!=0&&frameHeight!=0){
				sprite.setTextureRect(sf::IntRect(0,0,frameWidth, frameHeight));
			}
		};
		RenderComponent(float ox, float oy,sf::Texture & texture, System<PositionComponent>& posSys, EntityId id) : offsetX(ox), offsetY(oy), Component(id), position(posSys.components[owner]) {
			sprite.setTexture(texture);
			sprite.setPosition(position->x + offsetX, position->y + offsetY);
		};
		
		void update(float dt) {
			if(!render)
				return;
			if(playingAnimation) {
				frameTimer += dt;
				if(frameTimer >= 1/currentAnimation.speed) {
					currentAnimation.currentFrame = currentAnimation.frames[currentAnimation.currentIt];
					currentAnimation.currentIt++;
					int fY = int(currentAnimation.currentFrame/(sprite.getTexture()->getSize().x/frameWidth));
					sprite.setTextureRect(sf::IntRect((facing?0:frameWidth) + frameWidth*currentAnimation.currentFrame,frameHeight*fY,(facing?1:-1)*frameWidth,frameHeight));
					if(currentAnimation.currentIt >= currentAnimation.frames.size())
					{
						currentAnimation.currentIt = 0;
						if(!currentAnimation.loop) {
							playingAnimation = false;
						}
					}
					frameTimer = 0;
				}
			}
			sprite.setPosition(position->x + offsetX, position->y + offsetY);
		};
		
		void playAnimation(std::vector<int> f, float s, bool loop, bool force=false){
			if(!playingAnimation || force) {
				currentAnimation.frames = f;
				currentAnimation.speed = s;
				currentAnimation.loop = loop;
				currentAnimation.currentIt = 0;
				playingAnimation = true;
				frameTimer = 1/s;
			}
			
		};
		
		
		
		//void addAnimation(std::String name, std::vector<int> frames, int framespeed, bool loop);
		
		
};

class MovementComponent : public Component {
	public :
		PositionComponent * position;
		float vx=0, vy=0, ax=0, ay=0, maxVx=0, maxVy=0, dragx = 0, dragy = 0;
		bool moving = false;
		MovementComponent(float mvx, float mvy, float dx, float dy, System<PositionComponent>& posSys, EntityId id) : maxVx(mvx), maxVy(mvy), dragx(dx), dragy(dy), Component(id), position(posSys.components[owner]) {
		};
		void update(float dt) {
			/* if(!moving) {
				ax = 0;
				ay = 0;
			} */
			
			if(ax != 0) {
				vx += ax;
			} else if(dragx != 0) {
				if(vx - dragx > 0) {
					vx = vx - dragx;
				} else if (vx + dragx < 0) {
					vx += dragx;
				} else {
					vx = 0;
				}
			}
			if(ay != 0) {
				vy += ay;
			} else if(dragy != 0) {
				if(vy - dragy > 0) {
					vy = vy - dragy;
				} else if (vy + dragy < 0) {
					vy += dragy;
				} else {
					vy = 0;
				}
			}
			//vx += (ax - vx*dragx);
			//vy += (ay - vy*dragy);
			/* vx = vx - vx*drag;
			vy = vy - vy*drag; */
			//std::cout << ax <<"\n";
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
			position->setPosition(position->x + vx, position->y + vy);
		};
};

class HitboxComponent : public Component {
	public :
		PositionComponent * position;
		float offsetX=0,offsetY=0;
		float width, height;
		bool overlaped;
		bool immovable;
		int touching = NONE;
		HitboxComponent(float ox, float oy, float w, float h, System<PositionComponent>& posSys, EntityId id, bool m=false) : 
		Component(id), position(posSys.components[id]),	offsetX(ox), offsetY(oy), width(w), height(h), immovable(m) {
		};
		void update(){
			touching = NONE;
		};
		
};



#endif