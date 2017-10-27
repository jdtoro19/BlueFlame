#ifndef PHYSICS_H
#define PHYSICS_H

#include "PhysicsComponent.h"
#include "CollisionComponent.h"

namespace ENGINE {
	class Physics {
	private:
		Physics();
		~Physics();
	public:
		static bool isColliding(CollisionComponent *obj1, CollisionComponent *obj2);
		static void Collide(PhysicsComponent *obj1, PhysicsComponent *obj2);
	};
}

#endif

