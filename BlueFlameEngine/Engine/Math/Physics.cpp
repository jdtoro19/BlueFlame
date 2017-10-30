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
		//return ((obj1->getBoundingBox().minX <= obj2->getBoundingBox().maxX && obj1->getBoundingBox().maxX >= obj2->getBoundingBox().minX) &&
		//	(obj1->getBoundingBox().minY <= obj2->getBoundingBox().maxY && obj1->getBoundingBox().maxY >= obj2->getBoundingBox().minY) &&
		//	(obj1->getBoundingBox().minZ <= obj2->getBoundingBox().maxZ && obj1->getBoundingBox().maxZ >= obj2->getBoundingBox().minZ));
		if (obj1->getBoundingBox().maxX < obj2->getBoundingBox().minX || obj1->getBoundingBox().minX > obj2->getBoundingBox().maxX) return false;
		if (obj1->getBoundingBox().maxY < obj2->getBoundingBox().minY || obj1->getBoundingBox().minY > obj2->getBoundingBox().maxY) return false;
		if (obj1->getBoundingBox().maxZ < obj2->getBoundingBox().minZ || obj1->getBoundingBox().minZ > obj2->getBoundingBox().maxZ) return false;
		return true;
	}
	else {
		return false;
	}
}

void Physics::Collide(PhysicsComponent *pc1, CollisionComponent *cc1, PhysicsComponent *pc2, CollisionComponent *cc2, float e) {


	// Initializing our collision normal and penatration distance
	// Both will be used when resolving collisions
	glm::vec3 normal;
	float penatration = 0.0f;

	// Setting the normal to a vector between centre points of both objects
	normal = glm::vec3(pc2->getPosition().x - pc1->getPosition().x, pc2->getPosition().y - pc1->getPosition().y, pc2->getPosition().z - pc1->getPosition().z);

	// Half extents of both objects in the x axis
	float aExtent = (cc1->getBoundingBox().maxX - cc1->getBoundingBox().minX) / 2.0f;
	float bExtent = (cc2->getBoundingBox().maxX - cc2->getBoundingBox().minX) / 2.0f;

	// The overlap in the x axis of the objects
	float x_overlap = aExtent + bExtent - glm::abs(normal.x);

	// If the overlap exists, continue
	if (x_overlap > 0) {

		// Half extents of both objects in the y axis
		aExtent = (cc1->getBoundingBox().maxY - cc1->getBoundingBox().minY) / 2.0f;
		bExtent = (cc2->getBoundingBox().maxY - cc2->getBoundingBox().minY) / 2.0f;

		// The overlap in the y axis of the objects
		float y_overlap = aExtent + bExtent - glm::abs(normal.y);

		// If the overlap exists, continue
		if (y_overlap > 0) {

			// Half extents of both objects in the z axis
			aExtent = (cc1->getBoundingBox().maxZ - cc1->getBoundingBox().minZ) / 2.0f;
			bExtent = (cc2->getBoundingBox().maxZ - cc2->getBoundingBox().minZ) / 2.0f;

			// The overlap in the z axis of the objects
			float z_overlap = aExtent + bExtent - glm::abs(normal.z);

			// If the overlap exists, continue
			if (z_overlap > 0) {

				// Checking if x axis is the smallest penatration
				if (x_overlap < y_overlap && x_overlap < z_overlap) {

					// Point normal towards B, since distance points from A->B
					if (normal.x < 0) {
						normal = glm::vec3(-1.0f, 0.0f, 0.0f);
					}

					// Point normal towards A, since distance points from A->B
					else {
						normal = glm::vec3(1.0f, 0.0f, 0.0f);
					}

					// Since x axis is the smallest, it will be our penatration distance
					penatration = x_overlap;

					// Velocity changes (Work in Progress)
					if (pc2->getPhysicsType() == PhysicsComponent::Physics_Type::STATIC) {
						pc1->setVelocity(glm::vec3(0.0f, pc1->GetVelocity().y, pc1->GetVelocity().z));
					}

				}

				// Checking if y axis is the smallest penatration
				else if (y_overlap < x_overlap && y_overlap < z_overlap) {

					// Point normal towards B, since distance points from A->B
					if (normal.y < 0) {
						normal = glm::vec3(0.0f, -1.0f, 0.0f);
					}

					// Point normal towards A, since distance points from A->B
					else {
						normal = glm::vec3(0.0f, 1.0f, 0.0f);
					}

					// Since y axis is the smallest, it will be our penatration distance
					penatration = y_overlap;


					// Velocity changes (Work in Progress)
					if (pc2->getPhysicsType() == PhysicsComponent::Physics_Type::STATIC) {
						pc1->setVelocity(glm::vec3(pc1->GetVelocity().x, 0.0f, pc1->GetVelocity().z));
					}

					if (pc2->getPhysicsType() == PhysicsComponent::Physics_Type::DYNAMIC) {
						pc1->setVelocity(glm::vec3(pc2->GetVelocity().x, 0.0f, pc2->GetVelocity().z));
					}
				}

				// Checking if z axis is the smallest penatration
				else if (z_overlap < x_overlap && z_overlap < y_overlap) {

					// Point normal towards B, since distance points from A->B
					if (normal.z < 0) {
						normal = glm::vec3(0.0f, 0.0f, -1.0f);
					}

					// Point normal towards A, since distance points from A->B
					else {
						normal = glm::vec3(0.0f, 0.0f, 1.0f);
					}

					// Since z axis is the smallest, it will be our penatration distance
					penatration = z_overlap;

					// Velocity changes (Work in Progress)
					if (pc2->getPhysicsType() == PhysicsComponent::Physics_Type::STATIC) {
						pc1->setVelocity(glm::vec3(pc1->GetVelocity().x, pc1->GetVelocity().y, 0.0f));
					}
				}
			}
		}
	}

	//
	float aInvMass;
	float bInvMass;

	if (pc1->GetMass() == 0)
		aInvMass = 0;
	else
		aInvMass = 1 / pc1->GetMass();

	if (pc2->GetMass() == 0)
		bInvMass = 0;
	else
		bInvMass = 1 / pc2->GetMass();

	glm::vec3 rv;
	glm::vec3 impulse;

	rv = pc2->GetVelocity() - pc1->GetVelocity();

	float velAlongNormal = glm::dot(rv, normal);

	if (velAlongNormal > 0) {
		return;
	}

	float j = -(1 + e) * velAlongNormal;
	j /= aInvMass + bInvMass;

	impulse = j * normal;

	float percent = 0.8f;
	float slop = 0.1f;
	glm::vec3 correction = penatration / (aInvMass + bInvMass) * normal;
	pc1->setPosition(pc1->getPosition() - aInvMass * correction);
	pc2->setPosition(pc2->getPosition() + bInvMass * correction);

	pc1->SetVelocity(pc1->GetVelocity() - aInvMass * impulse);
	pc2->SetVelocity(pc2->GetVelocity() + bInvMass * impulse);

}
