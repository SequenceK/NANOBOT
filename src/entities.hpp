#include "game.hpp"
#ifndef ENTITIES_HPP
#define ENTITIES_HPP


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
				world.rSys.components[id] = new RenderComponent("../data/face.png", world.pSys, id);
				world.mSys.components[id] = new MovementComponent(100,100, 0.4, world.pSys, id);
				world.hbSys.components[id] = new HitboxComponent(0,0,32,32,world.pSys, id);
				//world.mSys.components[id]->ax = -100;
				//world.mSys.components[id]->moving = true;
				world.rSys.components[id]->sprite.setScale(1, 1);
				world.hbSys.components[id]->moveable = false;
				return id;
};
}
#endif