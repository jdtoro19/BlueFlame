#include "Physics.h"
#include <iostream>

using namespace ENGINE;

Physics::Physics() {

}

Physics::~Physics() {

}

bool Physics::isColliding(CollisionComponent *obj1, CollisionComponent *obj2) {
	if (obj1->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE && obj1->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {
		return false;
	}
	else if (obj1->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE && obj1->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {
		return false;
	}
	else if (obj1->GetCollisionType() == CollisionComponent::Collision_Type::BOX && obj1->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {
		return false;
	}
	else if (obj1->GetCollisionType() == CollisionComponent::Collision_Type::BOX && obj1->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {
		return ((obj1->getBoundingBox().minX <= obj2->getBoundingBox().maxX && obj1->getBoundingBox().maxX >= obj2->getBoundingBox().minX) &&
			(obj1->getBoundingBox().minY <= obj2->getBoundingBox().maxY && obj1->getBoundingBox().maxY >= obj2->getBoundingBox().minY) &&
			(obj1->getBoundingBox().minZ <= obj2->getBoundingBox().maxZ && obj1->getBoundingBox().maxZ >= obj2->getBoundingBox().minZ));
	}
}

void Physics::Collide(PhysicsComponent *obj1, PhysicsComponent *obj2) {
	if (obj1->getPhysicsType() == PhysicsComponent::Physics_Type::DYNAMIC && obj2->getPhysicsType() == PhysicsComponent::Physics_Type::DYNAMIC) {

	}
	else if (obj1->getPhysicsType() == PhysicsComponent::Physics_Type::STATIC || obj2->getPhysicsType() == PhysicsComponent::Physics_Type::STATIC) {
		obj1->Stop();
		obj2->Stop();
	}
}