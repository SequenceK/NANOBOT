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
			Box(float xx, float yy, int width, int height, Game& game) {
				id = game.createId();
				game.pSys.components[id] = new PositionComponent(xx, yy, id);
				game.rSys.components[id] = new RenderComponent("../data/face.png", game.pSys, id);
			}
	};
} */

unsigned long Box(float xx, float yy, Game& game) {
				EntityId id = game.createId();
				game.pSys.components[id] = new PositionComponent(xx, yy, id);
				game.rSys.components[id] = new RenderComponent("../data/face.png", game.pSys, id);
				game.mSys.components[id] = new MovementComponent(100,100, 0.1, game.pSys, id);
				game.hbSys.components[id] = new HitboxComponent(0,0,32,32,game.pSys, id);
				//game.mSys.components[id]->ax = -100;
				game.mSys.components[id]->moving = true;
				return id;
};
}
#endif