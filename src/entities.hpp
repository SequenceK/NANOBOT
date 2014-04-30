#ifndef ENTITIES_HPP
#define ENTITIES_HPP
#include "game.hpp"
#include "textures.hpp"

using namespace G;


namespace entities {
	//typedef unsigned long EntityId;
/* 	class Box {
		private:
			EntityId id;
		public:
			Box(float xx, float yy, int width, int height, World& world) {
				id = world.createId();
				world.pSys.components[id] = new PositionComponent(xx, yy, id);
				world.rSys.components[id] = new RenderComponent("../data/face.png", game.pSys, id);
			}
	};
} */

	unsigned long Box(float xx, float yy, World& world) {
		EntityId id = world.createId();
		world.pSys.components[id] = new PositionComponent(xx, yy, id);
		world.rSys.components[id] = new RenderComponent(*world.currentTex, world.pSys, id);
		world.mSys.components[id] = new MovementComponent(100,100, 0.4, 0, world.pSys, id);
		world.hbSys.components[id] = new HitboxComponent(0,0,32,32,world.pSys, id);
		//world.mSys.components[id]->ax = -100;
		//world.mSys.components[id]->moving = true;
		world.rSys.components[id]->sprite.setScale(1, 1);
		world.rSys.components[id]->render = true;
		world.hbSys.components[id]->immovable = true;
		return id;
	};
	unsigned long Box(float xx, float yy, float w, float h, World& world) {
		EntityId id = world.createId();
		world.pSys.components[id] = new PositionComponent(xx, yy, id);
		world.rSys.components[id] = new RenderComponent(*world.currentTex, world.pSys, id);
		world.mSys.components[id] = new MovementComponent(100,100, 0.4, 0, world.pSys, id);
		world.hbSys.components[id] = new HitboxComponent(0,0,w,h,world.pSys, id);
		//world.mSys.components[id]->ax = -100;
		//world.mSys.components[id]->moving = true;
		world.rSys.components[id]->sprite.setScale(w/32, h/32);
		world.rSys.components[id]->render = true;
		world.hbSys.components[id]->immovable = true;
		return id;
	};

	unsigned long Player(World& world) {
		EntityId id = world.createId();
		world.pSys.components[id] = new PositionComponent(2300,1000,id);
		world.rSys.components[id] = new RenderComponent(Tex::player,world.pSys, id);
		world.mSys.components[id] = new MovementComponent(3, 10, 3, 1000,world.pSys,id);
		world.hbSys.components[id] = new HitboxComponent(0,0,18,28,world.pSys,id);
		world.getRenderC(id)->frameWidth = 30;
		world.getRenderC(id)->frameHeight = 27;
		return id;
	};
	
	unsigned long Image(float x, float y, sf::Texture & tex, World& world) {
		EntityId id = world.createId();
		world.pSys.components[id] = new PositionComponent(x,y,id);
		world.rSys.components[id] = new RenderComponent(tex,world.pSys, id);
		return id;
	};

};

#endif