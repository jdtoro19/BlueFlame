#include "PhysicsEngine.h"
#include <iostream>

using namespace ENGINE;

std::unique_ptr<PhysicsEngine> PhysicsEngine::PhysicsEngineInstance(nullptr);


PhysicsEngine::PhysicsEngine() {

}

PhysicsEngine::~PhysicsEngine() {

}

void PhysicsEngine::Update(float deltaTime) {
	for (size_t i = 0; i < contactList.size(); ++i) {
		if (isColliding(contactList.at(i))) {
			PenatrationCorrection(contactList.at(i));
			ApplyImpulse(contactList.at(i));
		}
	}
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

bool PhysicsEngine::isColliding(ContactData *c) {

	// If statements differentiate which type of colliders want to collide, and does the appropriate collision check

	if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {
		float r = c->A->collisionComponent->GetBoundingSphere().radius + c->B->collisionComponent->GetBoundingSphere().radius;
		r *= r;

		if (r < ((c->A->collisionComponent->GetBoundingSphere().centre.x + c->B->collisionComponent->GetBoundingSphere().centre.x) *
			(c->A->collisionComponent->GetBoundingSphere().centre.x + c->B->collisionComponent->GetBoundingSphere().centre.x)) +
			((c->A->collisionComponent->GetBoundingSphere().centre.y + c->B->collisionComponent->GetBoundingSphere().centre.y) *
			(c->A->collisionComponent->GetBoundingSphere().centre.y + c->B->collisionComponent->GetBoundingSphere().centre.y)) +
			((c->A->collisionComponent->GetBoundingSphere().centre.z + c->B->collisionComponent->GetBoundingSphere().centre.z) *
			(c->A->collisionComponent->GetBoundingSphere().centre.z + c->B->collisionComponent->GetBoundingSphere().centre.z))) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {

		float x = glm::max(c->B->collisionComponent->GetBoundingBox().min.x, glm::min(c->A->collisionComponent->GetBoundingSphere().centre.x, c->B->collisionComponent->GetBoundingBox().max.x));
		float y = glm::max(c->B->collisionComponent->GetBoundingBox().min.y, glm::min(c->A->collisionComponent->GetBoundingSphere().centre.y, c->B->collisionComponent->GetBoundingBox().max.y));
		float z = glm::max(c->B->collisionComponent->GetBoundingBox().min.z, glm::min(c->A->collisionComponent->GetBoundingSphere().centre.z, c->B->collisionComponent->GetBoundingBox().max.z));

		float distance = glm::sqrt((x - c->A->collisionComponent->GetBoundingSphere().centre.x) * (x - c->A->collisionComponent->GetBoundingSphere().centre.x) +
			(y - c->A->collisionComponent->GetBoundingSphere().centre.y) * (y - c->A->collisionComponent->GetBoundingSphere().centre.y) +
			(z - c->A->collisionComponent->GetBoundingSphere().centre.z) * (z - c->A->collisionComponent->GetBoundingSphere().centre.z));

		if (distance < c->A->collisionComponent->GetBoundingSphere().radius) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {

		float x = glm::max(c->A->collisionComponent->GetBoundingBox().min.x, glm::min(c->B->collisionComponent->GetBoundingSphere().centre.x, c->A->collisionComponent->GetBoundingBox().max.x));
		float y = glm::max(c->A->collisionComponent->GetBoundingBox().min.y, glm::min(c->B->collisionComponent->GetBoundingSphere().centre.y, c->A->collisionComponent->GetBoundingBox().max.y));
		float z = glm::max(c->A->collisionComponent->GetBoundingBox().min.z, glm::min(c->B->collisionComponent->GetBoundingSphere().centre.z, c->A->collisionComponent->GetBoundingBox().max.z));

		float distance = glm::sqrt((x - c->B->collisionComponent->GetBoundingSphere().centre.x) * (x - c->B->collisionComponent->GetBoundingSphere().centre.x) +
			(y - c->B->collisionComponent->GetBoundingSphere().centre.y) * (y - c->B->collisionComponent->GetBoundingSphere().centre.y) +
			(z - c->B->collisionComponent->GetBoundingSphere().centre.z) * (z - c->B->collisionComponent->GetBoundingSphere().centre.z));

		if (distance < c->B->collisionComponent->GetBoundingSphere().radius) {
			return true;
		}
		else {
			return false;
		}
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

	if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE && 
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {

		// Creating normal to the vector between centre points of both objects
		glm::vec3 normal = c->B->physicsComponent->GetPosition() - c->A->physicsComponent->GetPosition();

		// Calculates the sum of each spheres radius
		float r = c->A->collisionComponent->GetBoundingSphere().radius + c->B->collisionComponent->GetBoundingSphere().radius;

		// Calculates the distance (magnitude) of the normal vector
		float distance = glm::length(normal);

		// Checks if spheres are directly on top of each other
		// Pushes back sphere in a consistent manner
		if (distance == 0.0f) {
			c->penatration = c->A->collisionComponent->GetBoundingSphere().radius;
			c->normal = glm::vec3(0.0f, 1.0f, 0.0f);
		}
		// If spheres are not directly on top of each other
		// Penatration is equal to the sum of the radii - the distance between them
		// Normal is then normalized with the already calculated magnitude
		else {
			c->penatration = r - distance;
			c->normal = normal / distance;
		}
	}

	// ======== SPHERE VS BOX COLLISION ==========
	// ============ WORK IN PROGRESS ============= 
	/*
	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {
		glm::vec3 n = c->B->physicsComponent->GetPosition() - c->A->physicsComponent->GetPosition();

		glm::vec3 closest = n;

		float xExtent = (c->B->collisionComponent->GetBoundingBox().max.x - c->B->collisionComponent->GetBoundingBox().min.x) / 2.0f;
		float yExtent = (c->B->collisionComponent->GetBoundingBox().max.y - c->B->collisionComponent->GetBoundingBox().min.y) / 2.0f;
		float zExtent = (c->B->collisionComponent->GetBoundingBox().max.z - c->B->collisionComponent->GetBoundingBox().min.z) / 2.0f;

		closest.x = glm::clamp(-xExtent, xExtent, closest.x);
		closest.y = glm::clamp(-yExtent, yExtent, closest.y);
		closest.z = glm::clamp(-zExtent, zExtent, closest.z);

		bool inside = false;

		if (n == closest) {
			inside = true;

			if (glm::abs(n.x) < glm::abs(n.y) && glm::abs(n.x) < glm::abs(n.z)) {
				if (closest.x > 0) {
					closest.x = -xExtent;
				}
				else {
					closest.x = xExtent;
				}
			}
			else if (glm::abs(n.y) < glm::abs(n.x) && glm::abs(n.y) < glm::abs(n.z)) {
				if (closest.y > 0) {
					closest.y = -yExtent;
				}
				else {
					closest.y = yExtent;
				}
			}
			else if (glm::abs(n.z) < glm::abs(n.x) && glm::abs(n.z) < glm::abs(n.y)) {
				if (closest.z > 0) {
					closest.z = -zExtent;
				}
				else {
					closest.z = zExtent;
				}
			}

			glm::vec3 normal = n - closest;
			float d = glm::length(normal);
			float r = c->A->collisionComponent->GetBoundingSphere().radius;

			d = glm::sqrt(d);

			if (inside) {
				c->normal = -n;
				c->penatration = r - d;
			}
			else {
				c->normal = n;
				c->penatration = r - d;
			}
		}
	}
	// ======== SPHERE VS BOX COLLISION ==========
	// ============ WORK IN PROGRESS ============= 
	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {
		glm::vec3 n = c->B->physicsComponent->GetPosition() - c->A->physicsComponent->GetPosition();

		glm::vec3 closest = n;

		float xExtent = (c->A->collisionComponent->GetBoundingBox().max.x - c->A->collisionComponent->GetBoundingBox().min.x) / 2.0f;
		float yExtent = (c->A->collisionComponent->GetBoundingBox().max.y - c->A->collisionComponent->GetBoundingBox().min.y) / 2.0f;
		float zExtent = (c->A->collisionComponent->GetBoundingBox().max.z - c->A->collisionComponent->GetBoundingBox().min.z) / 2.0f;

		closest.x = glm::clamp(-xExtent, xExtent, closest.x);
		closest.y = glm::clamp(-yExtent, yExtent, closest.y);
		closest.z = glm::clamp(-zExtent, zExtent, closest.z);

		bool inside = false;

		if (n == closest) {
			inside = true;

			if (glm::abs(n.x) < glm::abs(n.y) && glm::abs(n.x) < glm::abs(n.z)) {
				if (closest.x > 0) {
					closest.x = xExtent;
				}
				else {
					closest.x = -xExtent;
				}
			}
			else if (glm::abs(n.y) < glm::abs(n.x) && glm::abs(n.y) < glm::abs(n.z)) {
				if (closest.y > 0) {
					closest.y = yExtent;
				}
				else {
					closest.y = -yExtent;
				}
			}
			else if (glm::abs(n.z) < glm::abs(n.x) && glm::abs(n.z) < glm::abs(n.y)) {
				if (closest.z > 0) {
					closest.z = zExtent;
				}
				else {
					closest.z = -zExtent;
				}
			}

			glm::vec3 normal = n - closest;
			float d = glm::length(normal);
			float r = c->B->collisionComponent->GetBoundingSphere().radius;

			d = glm::sqrt(d);

			if (inside) {
				c->normal = -n;
				c->penatration = r - d;
			}
			else {
				c->normal = n;
				c->penatration = r - d;
			}
		}
	}
	*/
	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX && 
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {
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
	}
	// Calculates positional correction based on penatration of both objects and their inverse masses, along the normal
	const float slop = 0.01f; // Penatration allowed
	const float percent = 0.75f; // {enatration percentage to be corrected
	glm::vec3 correction = (glm::max(c->penatration - slop, 0.0f) / (c->A->physicsComponent->GetInverseMass() + c->B->physicsComponent->GetInverseMass())) * c->normal * percent;

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