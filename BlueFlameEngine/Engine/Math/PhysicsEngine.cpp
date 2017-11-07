#include "PhysicsEngine.h"
#include <iostream>

using namespace ENGINE;

std::unique_ptr<PhysicsEngine> PhysicsEngine::PhysicsEngineInstance(nullptr);


PhysicsEngine::PhysicsEngine() {
}

PhysicsEngine::~PhysicsEngine() {

}

PhysicsEngine* PhysicsEngine::GetInstance() {
	if (PhysicsEngineInstance.get() == nullptr) {
		PhysicsEngineInstance.reset(new PhysicsEngine());
	}
	return PhysicsEngineInstance.get();
}

void PhysicsEngine::AddObjectList(std::vector<GameObject*> physicsObjectList) {
	GeneratePairs(physicsObjectList);
}

void PhysicsEngine::GeneratePairs(std::vector<GameObject*> physicsObjectList) {
	contactList.clear();
	for (size_t i = 0; i < physicsObjectList.size(); ++i) {
		for (size_t j = i + 1; j < physicsObjectList.size(); ++j) {
			if (physicsObjectList.at(i)->physicsComponent->GetPhysicsType() == PhysicsComponent::Physics_Type::STATIC && 
				physicsObjectList.at(j)->physicsComponent->GetPhysicsType() == PhysicsComponent::Physics_Type::STATIC) {
				continue;
			}

			contactList.push_back(ContactData::CreateContactData(physicsObjectList.at(i), physicsObjectList.at(j)));
		}
	}
}

void PhysicsEngine::Update(float deltaTime) {
	for (size_t i = 0; i < contactList.size(); ++i) {
		if (isColliding(contactList.at(i))) {
			PenatrationCorrection(contactList.at(i));
			ApplyImpulse(contactList.at(i));
		}
	}
}

bool PhysicsEngine::isColliding(ContactData *c) {

	// If statements differentiate which type of colliders want to collide, and does the appropriate collision check

	if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE && 
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {
		//glm::vec3 distance = glm::sqrt((glm::length(c->B->collisionComponent->GetBoundingSphere().centre -
			//c->A->collisionComponent->GetBoundingSphere().centre)))
		return false;
	}
	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {

		return false;
	}
	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {

		return false;
	}
	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {

		if (c->A->collisionComponent->GetBoundingBox().max.x < c->B->collisionComponent->GetBoundingBox().min.x 
			|| c->A->collisionComponent->GetBoundingBox().min.x > c->B->collisionComponent->GetBoundingBox().max.x) return false;

		if (c->A->collisionComponent->GetBoundingBox().max.y < c->B->collisionComponent->GetBoundingBox().min.y 
			|| c->A->collisionComponent->GetBoundingBox().min.y > c->B->collisionComponent->GetBoundingBox().max.y) return false;

		if (c->A->collisionComponent->GetBoundingBox().max.z < c->B->collisionComponent->GetBoundingBox().min.z 
			|| c->A->collisionComponent->GetBoundingBox().min.z > c->B->collisionComponent->GetBoundingBox().max.z) return false;

		return true;

	}
	else {
		return false;
	}
}

void PhysicsEngine::PenatrationCorrection(ContactData *c) {

	// Setting the normal to a vector between centre points of both objects
	c->normal = c->B->physicsComponent->GetPosition() - c->A->physicsComponent->GetPosition();

	// Half extents of both objects in the x axis
	float aExtent = (c->A->collisionComponent->GetBoundingBox().max.x - c->A->collisionComponent->GetBoundingBox().min.x) / 2.0f;
	float bExtent = (c->B->collisionComponent->GetBoundingBox().max.x - c->B->collisionComponent->GetBoundingBox().min.x) / 2.0f;

	// The overlap in the x axis of the objects
	float x_overlap = aExtent + bExtent - glm::abs(c->normal.x);

	// If the overlap exists, continue
	if (x_overlap > 0) {

		// Half extents of both objects in the y axis
		aExtent = (c->A->collisionComponent->GetBoundingBox().max.y - c->A->collisionComponent->GetBoundingBox().min.y) / 2.0f;
		bExtent = (c->B->collisionComponent->GetBoundingBox().max.y - c->B->collisionComponent->GetBoundingBox().min.y) / 2.0f;

		// The overlap in the y axis of the objects
		float y_overlap = aExtent + bExtent - glm::abs(c->normal.y);

		// If the overlap exists, continue
		if (y_overlap > 0) {

			// Half extents of both objects in the z axis
			aExtent = (c->A->collisionComponent->GetBoundingBox().max.z - c->A->collisionComponent->GetBoundingBox().min.z) / 2.0f;
			bExtent = (c->B->collisionComponent->GetBoundingBox().max.z - c->B->collisionComponent->GetBoundingBox().min.z) / 2.0f;

			// The overlap in the z axis of the objects
			float z_overlap = aExtent + bExtent - glm::abs(c->normal.z);

			// If the overlap exists, continue
			if (z_overlap > 0) {

				// Checking if x axis is the smallest penatration
				if (x_overlap < y_overlap && x_overlap < z_overlap) {

					// Point normal towards B, since distance points from A->B
					if (c->normal.x < 0) {
						c->normal = glm::vec3(-1.0f, 0.0f, 0.0f);
					}

					// Point normal towards A, since distance points from A->B
					else {
						c->normal = glm::vec3(1.0f, 0.0f, 0.0f);
					}

					// Since x axis is the smallest, it will be our penatration distance
					c->penatration = x_overlap;

				}

				// Checking if y axis is the smallest penatration
				else if (y_overlap < x_overlap && y_overlap < z_overlap) {

					// Point normal towards B, since distance points from A->B
					if (c->normal.y < 0) {
						c->normal = glm::vec3(0.0f, -1.0f, 0.0f);
					}

					// Point normal towards A, since distance points from A->B
					else {
						c->normal = glm::vec3(0.0f, 1.0f, 0.0f);
					}

					// Since y axis is the smallest, it will be our penatration distance
					c->penatration = y_overlap;

				}

				// Checking if z axis is the smallest penatration
				else if (z_overlap < x_overlap && z_overlap < y_overlap) {

					// Point normal towards B, since distance points from A->B
					if (c->normal.z < 0) {
						c->normal = glm::vec3(0.0f, 0.0f, -1.0f);
					}

					// Point normal towards A, since distance points from A->B
					else {
						c->normal = glm::vec3(0.0f, 0.0f, 1.0f);
					}

					// Since z axis is the smallest, it will be our penatration distance
					c->penatration = z_overlap;

				}
			}
		}
	}

	// Calculates positional correction based on penatration of both objects and their inverse masses, along the normal
	glm::vec3 correction = c->penatration / (c->A->physicsComponent->GetInverseMass() + c->B->physicsComponent->GetInverseMass()) * c->normal;

	// Pushes back objects based on correction vector and inverse mass
	c->A->physicsComponent->SetPosition(c->A->physicsComponent->GetPosition() - c->A->physicsComponent->GetInverseMass() * correction);
	c->B->physicsComponent->SetPosition(c->B->physicsComponent->GetPosition() + c->B->physicsComponent->GetInverseMass() * correction);
}

void PhysicsEngine::ApplyImpulse(ContactData *c) {

	// Initializing our relative velocity and impulses
	// All three are used when resolving collisions
	glm::vec3 rv;
	glm::vec3 impulse;
	glm::vec3 frictionImpulse;

	// Calculates relative velocity between both objects
	rv = c->B->physicsComponent->GetVelocity() - c->A->physicsComponent->GetVelocity();

	// Checks to see if the two colliding bodies are both Dynamic
	// If true, recalculate the collision normal to get a more realstic collision
	// Note: Must be done after correcting penatration
	//if (c->A->physicsComponent->GetPhysicsType() == PhysicsComponent::Physics_Type::DYNAMIC && c->B->physicsComponent->GetPhysicsType() == PhysicsComponent::Physics_Type::DYNAMIC) {
		//c->normal = c->B->physicsComponent->GetPosition() - c->A->physicsComponent->GetPosition();
	//}

	// Projects the relative velocity along the collision normal
	float velAlongNormal = glm::dot(rv, c->normal);

	// Does not resolve if velocities are seperating
	if (velAlongNormal > 0) {
		return;
	}

	// Calculates restitution based on the lowest value of restitution of our colliding objects
	float e = glm::min(c->A->physicsComponent->GetRestitution(), c->B->physicsComponent->GetRestitution());

	// Calculates impulse scalar
	float j = -(1 + e) * velAlongNormal;
	j /= c->A->physicsComponent->GetInverseMass() + c->B->physicsComponent->GetInverseMass();

	// If both objects haven't penatrated slightly,
	// the normal hasn't been calculated, and make the impulse zero
	if (c->penatration != 0.0f) {
		impulse = j * c->normal;
	}
	else {
		impulse = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	// Changes the velocity of  objects based on  impulse and inverse mass
	c->A->physicsComponent->SetVelocity(c->A->physicsComponent->GetVelocity() - (c->A->physicsComponent->GetInverseMass() * impulse));
	c->B->physicsComponent->SetVelocity(c->B->physicsComponent->GetVelocity() + (c->B->physicsComponent->GetInverseMass() * impulse));

	// Calculates the tangent vector
	glm::vec3 tangent = rv - (glm::dot(rv, c->normal) * c->normal);
	glm::normalize(tangent);

	// Calculates the tangent impulse scalar
	float jt = -glm::dot(rv, tangent);
	jt /= c->A->physicsComponent->GetInverseMass() + c->B->physicsComponent->GetInverseMass();

	// Ignore tiny friction impulses
	if (jt == 0.0f) {
		return;
	}

	// Calculate the relative coefficient of static friction
	float mu = glm::sqrt(glm::length(c->A->physicsComponent->GetStaticFriction()) + glm::length(c->B->physicsComponent->GetStaticFriction()));

	// Clamps the magnitude of friction based on static friction and creates the friction impulse vector
	if (glm::abs(jt) < j * mu) {
		frictionImpulse = jt * tangent;
	}
	else {
		// Calculate the relative coefficient of kinetic friction
		float mu2 = glm::sqrt(glm::length(c->A->physicsComponent->GetKineticFriction()) + glm::length(c->B->physicsComponent->GetKineticFriction()));
		frictionImpulse = -j * tangent * mu2;
	}

	// Changes the velocity of  objects based on friction impulse and inverse mass
	c->A->physicsComponent->SetVelocity(c->A->physicsComponent->GetVelocity() - c->A->physicsComponent->GetInverseMass() * frictionImpulse);
	c->B->physicsComponent->SetVelocity(c->B->physicsComponent->GetVelocity() + c->B->physicsComponent->GetInverseMass() * frictionImpulse);
}

void PhysicsEngine::Collide(ContactData *c) {


	// Initializing our collision normal and penatration distance
	// Both will be used when resolving collisions
	glm::vec3 normal;
	float penatration = 0.0f;

	// Setting the normal to a vector between centre points of both objects
	normal = c->B->physicsComponent->GetPosition() - c->A->physicsComponent->GetPosition();

	// Half extents of both objects in the x axis
	float aExtent = (c->A->collisionComponent->GetBoundingBox().max.x - c->A->collisionComponent->GetBoundingBox().min.x) / 2.0f;
	float bExtent = (c->B->collisionComponent->GetBoundingBox().max.x - c->B->collisionComponent->GetBoundingBox().min.x) / 2.0f;

	// The overlap in the x axis of the objects
	float x_overlap = aExtent + bExtent - glm::abs(normal.x);

	// If the overlap exists, continue
	if (x_overlap > 0) {

		// Half extents of both objects in the y axis
		aExtent = (c->A->collisionComponent->GetBoundingBox().max.y - c->A->collisionComponent->GetBoundingBox().min.y) / 2.0f;
		bExtent = (c->B->collisionComponent->GetBoundingBox().max.y - c->B->collisionComponent->GetBoundingBox().min.y) / 2.0f;

		// The overlap in the y axis of the objects
		float y_overlap = aExtent + bExtent - glm::abs(normal.y);

		// If the overlap exists, continue
		if (y_overlap > 0) {

			// Half extents of both objects in the z axis
			aExtent = (c->A->collisionComponent->GetBoundingBox().max.z - c->A->collisionComponent->GetBoundingBox().min.z) / 2.0f;
			bExtent = (c->B->collisionComponent->GetBoundingBox().max.z - c->B->collisionComponent->GetBoundingBox().min.z) / 2.0f;

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
	rv = c->B->physicsComponent->GetVelocity() - c->A->physicsComponent->GetVelocity();

	// Projects the relative velocity along the collision normal
	float velAlongNormal = glm::dot(rv, normal);

	// Does not resolve if velocities are seperating
	if (velAlongNormal > 0) {
		return;
	}

	// Calculates restitution based on the lowest value of restitution of our colliding objects
	float e = glm::min(c->A->physicsComponent->GetRestitution(), c->B->physicsComponent->GetRestitution());

	// Calculates impulse scalar
	float j = -(1 + e) * velAlongNormal;
	j /= c->A->physicsComponent->GetInverseMass() + c->B->physicsComponent->GetInverseMass();

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
	glm::vec3 correction = penatration / (c->A->physicsComponent->GetInverseMass() + c->B->physicsComponent->GetInverseMass()) * normal;

	// Pushes back objects based on correction vector and inverse mass
	c->A->physicsComponent->SetPosition(c->A->physicsComponent->GetPosition() - c->A->physicsComponent->GetInverseMass() * correction);
	c->B->physicsComponent->SetPosition(c->B->physicsComponent->GetPosition() + c->B->physicsComponent->GetInverseMass() * correction);

	// 
	c->A->physicsComponent->SetVelocity(c->A->physicsComponent->GetVelocity() - c->A->physicsComponent->GetInverseMass() * impulse);
	c->B->physicsComponent->SetVelocity(c->B->physicsComponent->GetVelocity() + c->B->physicsComponent->GetInverseMass() * impulse);


	// Friction Application
	// ============= INTRODUCES BUGS OF FLYING OBJECTS =================
	// ====================== WORK IN PROGRESS =========================

	
	//rv = pc2->GetVelocity() - pc1->GetVelocity();

	glm::vec3 tangent = rv - (glm::dot(rv, normal) * normal);
	glm::normalize(tangent);

	float jt = -glm::dot(rv, tangent);
	jt  /= c->A->physicsComponent->GetInverseMass() + c->B->physicsComponent->GetInverseMass();

	if (jt == 0.0f) {
		return;
	}

	float mu = glm::sqrt(glm::length(c->A->physicsComponent->GetStaticFriction()) + glm::length(c->B->physicsComponent->GetStaticFriction()));

	glm::vec3 frictionImpulse;
	if (glm::abs(jt) < j * mu) {
		frictionImpulse = jt * tangent;
	}
	else {
		float mu2 = glm::sqrt(glm::length(c->A->physicsComponent->GetKineticFriction()) + glm::length(c->B->physicsComponent->GetKineticFriction()));
		frictionImpulse = -j * tangent * mu2;
	}

	c->A->physicsComponent->SetVelocity(c->A->physicsComponent->GetVelocity() - c->A->physicsComponent->GetInverseMass() * frictionImpulse);
	c->B->physicsComponent->SetVelocity(c->B->physicsComponent->GetVelocity() + c->B->physicsComponent->GetInverseMass() * frictionImpulse);

	if (c->A->physicsComponent->GetVelocity().x <= 0.05f && c->A->physicsComponent->GetVelocity().y <= 0.05f && c->A->physicsComponent->GetVelocity().z <= 0.05f && 
		c->A->physicsComponent->GetVelocity().x >= -0.05f && c->A->physicsComponent->GetVelocity().y >= -0.05f && c->A->physicsComponent->GetVelocity().z >= -0.05f) {
		c->A->physicsComponent->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	}
	// ================================================================
}