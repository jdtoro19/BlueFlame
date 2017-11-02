#include "Physics.h"
#include <iostream>

using namespace ENGINE;

Physics::Physics() {

}

Physics::~Physics() {

}

bool Physics::isColliding(CollisionComponent *cc1, CollisionComponent *cc2) {

	// If statements differentiate which type of colliders want to collide, and does the appropriate collision check

	if (cc1->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE && cc2->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {

		return false;
	}
	else if (cc1->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE && cc2->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {

		return false;
	}
	else if (cc1->GetCollisionType() == CollisionComponent::Collision_Type::BOX && cc2->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {

		return false;
	}
	else if (cc1->GetCollisionType() == CollisionComponent::Collision_Type::BOX && cc2->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {

		if (cc1->GetBoundingBox().max.x < cc2->GetBoundingBox().min.x || cc1->GetBoundingBox().min.x > cc2->GetBoundingBox().max.x) return false;
		if (cc1->GetBoundingBox().max.y < cc2->GetBoundingBox().min.y || cc1->GetBoundingBox().min.y > cc2->GetBoundingBox().max.y) return false;
		if (cc1->GetBoundingBox().max.z < cc2->GetBoundingBox().min.z || cc1->GetBoundingBox().min.z > cc2->GetBoundingBox().max.z) return false;
		return true;

	}
	else {
		return false;
	}
}

void Physics::Collide(PhysicsComponent *pc1, CollisionComponent *cc1, PhysicsComponent *pc2, CollisionComponent *cc2) {


	// Initializing our collision normal and penatration distance
	// Both will be used when resolving collisions
	glm::vec3 normal;
	float penatration = 0.0f;

	// Setting the normal to a vector between centre points of both objects
	normal = glm::vec3(pc2->getPosition().x - pc1->getPosition().x, pc2->getPosition().y - pc1->getPosition().y, pc2->getPosition().z - pc1->getPosition().z);

	// Half extents of both objects in the x axis
	float aExtent = (cc1->GetBoundingBox().max.x - cc1->GetBoundingBox().min.x) / 2.0f;
	float bExtent = (cc2->GetBoundingBox().max.x - cc2->GetBoundingBox().min.x) / 2.0f;

	// The overlap in the x axis of the objects
	float x_overlap = aExtent + bExtent - glm::abs(normal.x);

	// If the overlap exists, continue
	if (x_overlap > 0) {

		// Half extents of both objects in the y axis
		aExtent = (cc1->GetBoundingBox().max.y - cc1->GetBoundingBox().min.y) / 2.0f;
		bExtent = (cc2->GetBoundingBox().max.y - cc2->GetBoundingBox().min.y) / 2.0f;

		// The overlap in the y axis of the objects
		float y_overlap = aExtent + bExtent - glm::abs(normal.y);

		// If the overlap exists, continue
		if (y_overlap > 0) {

			// Half extents of both objects in the z axis
			aExtent = (cc1->GetBoundingBox().max.z - cc1->GetBoundingBox().min.z) / 2.0f;
			bExtent = (cc2->GetBoundingBox().max.z - cc2->GetBoundingBox().min.z) / 2.0f;

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

				}
			}
		}
	}

	// Impulse Resolution of Collisions
	// WORKS GOOD 

	// Initializing our relative velocity and impulse
	// Both are used when resolving collisions
	glm::vec3 rv;
	glm::vec3 impulse;

	// Calculates relative velocity between both objects
	rv = pc2->GetVelocity() - pc1->GetVelocity();

	// Projects the relative velocity along the collision normal
	float velAlongNormal = glm::dot(rv, normal);

	// Does not resolve if velocities are seperating
	if (velAlongNormal > 0) {
		return;
	}

	// Calculates restitution based on the lowest value of restitution of our colliding objects
	float e = glm::min(pc1->GetRestitution(), pc2->GetRestitution());

	// Calculates impulse scalar
	float j = -(1 + e) * velAlongNormal;
	j /= pc1->GetInverseMass() + pc2->GetInverseMass();

	// ========================= WORK IN PROGRESS ===============================
	// Checks that there is penatration and the normal has been made axis aligned 
	// Calculates impulse vector along aligned normal
	if (penatration != 0.0f) {
		impulse = j * normal;
	}
	else {
		impulse = glm::vec3(0.0f,0.0f,0.0f);
	}
	// ==========================================================================


	// Calculates positional correction based on penatration of both objects and their inverse masses, along the normal
	glm::vec3 correction = penatration / (pc1->GetInverseMass() + pc2->GetInverseMass()) * normal;

	// Pushes back objects based on correction vector and inverse mass
	pc1->SetPosition(pc1->getPosition() - pc1->GetInverseMass() * correction);
	pc2->SetPosition(pc2->getPosition() + pc2->GetInverseMass() * correction);

	// 
	pc1->SetVelocity(pc1->GetVelocity() - pc1->GetInverseMass() * impulse);
	pc2->SetVelocity(pc2->GetVelocity() + pc2->GetInverseMass() * impulse);


	// Friction Application
	// ============= INTRODUCES BUGS OF FLYING OBJECTS =================
	// ====================== WORK IN PROGRESS =========================

	
	rv = pc2->GetVelocity() - pc1->GetVelocity();

	glm::vec3 tangent = rv - glm::dot(rv, normal) * normal;
	glm::normalize(tangent);

	float jt = -glm::dot(rv, tangent);
	jt  /= pc1->GetInverseMass() + pc2->GetInverseMass();

	float mu = 0.5f;

	glm::vec3 frictionImpulse;
	if (glm::abs(jt) < j * mu) {
		frictionImpulse = jt * tangent;
	}
	else {
		float mu2 = 0.3f;
		frictionImpulse = -j * tangent * mu2;
	}

	pc1->SetVelocity(pc1->GetVelocity() - pc1->GetInverseMass() * frictionImpulse);
	pc2->SetVelocity(pc2->GetVelocity() + pc2->GetInverseMass() * frictionImpulse);

	if (pc1->GetVelocity().x <= 0.05f && pc1->GetVelocity().y <= 0.05f && pc1->GetVelocity().z <= 0.05f && pc1->GetVelocity().x >= -0.05f && pc1->GetVelocity().y >= -0.05f && pc1->GetVelocity().z >= -0.05f) {
		pc1->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	}
	// ================================================================
}