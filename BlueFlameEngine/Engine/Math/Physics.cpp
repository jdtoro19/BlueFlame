#include "Physics.h"

using namespace ENGINE;

Physics::Physics() {

}

Physics::~Physics() {

}

bool Physics::isColliding(CollisionComponent obj1, CollisionComponent obj2) {
	return ((obj1.getBoundingBox().minX <= obj2.getBoundingBox().maxX && obj1.getBoundingBox().maxX >= obj2.getBoundingBox().minX) &&
		(obj1.getBoundingBox().minY <= obj2.getBoundingBox().maxY && obj1.getBoundingBox().maxY >= obj2.getBoundingBox().minY) &&
		(obj1.getBoundingBox().minZ <= obj2.getBoundingBox().maxZ && obj1.getBoundingBox().maxZ >= obj2.getBoundingBox().minZ));
}

void Physics::Collide(PhysicsComponent obj1, PhysicsComponent obj2) {
	if (obj1.getPhysicsType() == PhysicsComponent::Physics_Type::STATIC || obj2.getPhysicsType() == PhysicsComponent::Physics_Type::STATIC) {
		obj1.setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		obj1.setAcceleration(glm::vec3(0.0f, 0.0f, 0.0f));
		obj2.setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		obj2.setAcceleration(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}