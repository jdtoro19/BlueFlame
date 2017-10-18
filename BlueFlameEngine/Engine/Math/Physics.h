#ifndef PHYSICS_H
#define PHYSICS_H

#include "PhysicsComponent.h"

namespace ENGINE {
	class Physics {
	private:
		Physics() {};
		~Physics() {};
	public:
		static bool isColliding(PhysicsComponent obj1, PhysicsComponent obj2);
		static void Collide(PhysicsComponent obj1, PhysicsComponent obj2);
	};
}

#endif

