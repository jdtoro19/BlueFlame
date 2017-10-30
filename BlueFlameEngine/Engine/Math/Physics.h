#ifndef PHYSICS_H
#define PHYSICS_H

#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "IntersectData.h"

#include <limits>

namespace ENGINE {
	class Physics {
	private:
		Physics();
		~Physics();
	public:
		static bool isColliding(CollisionComponent *obj1, CollisionComponent *obj2);
		static void Collide(PhysicsComponent *pc1, CollisionComponent *cc1, PhysicsComponent *pc2, CollisionComponent *cc2, float e);
	};


}

#endif

