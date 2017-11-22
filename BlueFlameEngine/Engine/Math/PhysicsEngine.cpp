#include "PhysicsEngine.h"
#include <iostream>

using namespace ENGINE;

std::unique_ptr<PhysicsEngine> PhysicsEngine::PhysicsEngineInstance(nullptr);


PhysicsEngine::PhysicsEngine() {

}

PhysicsEngine::~PhysicsEngine() {

}

void PhysicsEngine::Update(float deltaTime) {
	int size = contactList.size();
	for (size_t i = 0; i < size; ++i) {
		// If Objects Collide, correct penatration and apply impulse
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
	int size = physicsObjectList.size();
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = i + 1; j < size; ++j) {

			// Checks that both objects are on the same layer
			if (physicsObjectList.at(i)->collisionComponent->GetLayer() != physicsObjectList.at(j)->collisionComponent->GetLayer()) {
				continue;
			}

			// Checks that both objects are not static
			if (physicsObjectList.at(i)->physicsComponent->GetPhysicsType() == PhysicsComponent::Physics_Type::STATIC &&
				physicsObjectList.at(j)->physicsComponent->GetPhysicsType() == PhysicsComponent::Physics_Type::STATIC) {
				continue;
			}

			// Checks that you haven't pushed a physics object that doesn't have a physics component
			if (physicsObjectList.at(i)->physicsComponent == NULL ||
				physicsObjectList.at(j)->physicsComponent == NULL) {
				continue;
			}

			// Once pair passes all tests, push into the list
			contactList.push_back(ContactData::CreateContactData(physicsObjectList.at(i), physicsObjectList.at(j)));
		}
	}
}

bool PhysicsEngine::isColliding(ContactData *c) {

	// If statements differentiate which type of colliders want to collide, and does the appropriate collision check

	if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {

		// Calculates squared distance between centres
		glm::vec3 d = c->A->collisionComponent->GetBoundingSphere().c + c->B->collisionComponent->GetBoundingSphere().c;
		float d2 = glm::dot(d, d);

		// If the squared distance is less than the squared sum of radii, spheres intersect
		float radiusSum = c->A->collisionComponent->GetBoundingSphere().r + c->B->collisionComponent->GetBoundingSphere().r;
		if (d2 <= radiusSum * radiusSum) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {

		// Find point p on AABB closest to the centre of the sphere
		glm::vec3 p;
		ClosestPointOnAABB(c->A->collisionComponent->GetBoundingSphere().c, c->B->collisionComponent->GetBoundingBox(), p);

		// If the squared distance from sphere centre to point p is less than the squared sphere radius, box and sphere intersect
		glm::vec3 v = p - c->A->collisionComponent->GetBoundingSphere().c;
		if (glm::dot(v, v) <= c->A->collisionComponent->GetBoundingSphere().r * c->A->collisionComponent->GetBoundingSphere().r) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {

		// Find point p on AABB closest to the centre of the sphere
		glm::vec3 p;
		ClosestPointOnAABB(c->B->collisionComponent->GetBoundingSphere().c, c->A->collisionComponent->GetBoundingBox(), p);

		// If the squared distance from sphere centre to point p is less than the squared sphere radius, box and sphere intersect
		glm::vec3 v = p - c->B->collisionComponent->GetBoundingSphere().c;
		if (glm::dot(v, v) <= c->B->collisionComponent->GetBoundingSphere().r * c->B->collisionComponent->GetBoundingSphere().r) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {

		// If no intersection along an axis, boxes are not colliding
		if (glm::abs(c->A->collisionComponent->GetBoundingBox().c.x - c->B->collisionComponent->GetBoundingBox().c.x) > (c->A->collisionComponent->GetBoundingBox().r.x + c->B->collisionComponent->GetBoundingBox().r.x)) return false;
		if (glm::abs(c->A->collisionComponent->GetBoundingBox().c.y - c->B->collisionComponent->GetBoundingBox().c.y) > (c->A->collisionComponent->GetBoundingBox().r.y + c->B->collisionComponent->GetBoundingBox().r.y)) return false;
		if (glm::abs(c->A->collisionComponent->GetBoundingBox().c.z - c->B->collisionComponent->GetBoundingBox().c.z) > (c->A->collisionComponent->GetBoundingBox().r.z + c->B->collisionComponent->GetBoundingBox().r.z)) return false;

		if (c->A->physicsComponent->destructible == true) {
			c->A->deleted = true;
		}

		if (c->B->physicsComponent->destructible == true) {
			c->B->deleted = true;
		}

		// If intersecting along all axes, boxes are colliding 
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
		float r = c->A->collisionComponent->GetBoundingSphere().r + c->B->collisionComponent->GetBoundingSphere().r;

		// Calculates the distance (magnitude) of the normal vector
		float distance = glm::length(normal);

		// Checks if spheres are directly on top of each other
		// Pushes back sphere in a consistent manner
		if (distance == 0.0f) {
			c->penatration = c->A->collisionComponent->GetBoundingSphere().r;
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
	}*/
	
	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX && 
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {
		// Setting the normal to a vector between centre points of both objects
		c->normal = c->B->physicsComponent->GetPosition() - c->A->physicsComponent->GetPosition();

		// Half extents of both objects in the x axis
		//float aExtent = (c->A->collisionComponent->GetBoundingBox().max.x - c->A->collisionComponent->GetBoundingBox().min.x) / 2.0f;
		//float bExtent = (c->B->collisionComponent->GetBoundingBox().max.x - c->B->collisionComponent->GetBoundingBox().min.x) / 2.0f;

		// The overlap in the x axis of the objects
		float x_overlap = c->A->collisionComponent->GetBoundingBox().r.x + c->B->collisionComponent->GetBoundingBox().r.x - glm::abs(c->normal.x);

		// If the overlap exists, continue
		if (x_overlap > 0) {

			// Half extents of both objects in the y axis
			//aExtent = (c->A->collisionComponent->GetBoundingBox().max.y - c->A->collisionComponent->GetBoundingBox().min.y) / 2.0f;
			//bExtent = (c->B->collisionComponent->GetBoundingBox().max.y - c->B->collisionComponent->GetBoundingBox().min.y) / 2.0f;

			// The overlap in the y axis of the objects
			float y_overlap = c->A->collisionComponent->GetBoundingBox().r.y + c->B->collisionComponent->GetBoundingBox().r.y - glm::abs(c->normal.y);

			// If the overlap exists, continue
			if (y_overlap > 0) {

				// Half extents of both objects in the z axis
			//	aExtent = (c->A->collisionComponent->GetBoundingBox().max.z - c->A->collisionComponent->GetBoundingBox().min.z) / 2.0f;
				//bExtent = (c->B->collisionComponent->GetBoundingBox().max.z - c->B->collisionComponent->GetBoundingBox().min.z) / 2.0f;

				// The overlap in the z axis of the objects
				float z_overlap = c->A->collisionComponent->GetBoundingBox().r.z + c->B->collisionComponent->GetBoundingBox().r.z - glm::abs(c->normal.z);

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

	// Storing inverse masses
	float invMassA = c->A->physicsComponent->GetInverseMass();
	float invMassB = c->B->physicsComponent->GetInverseMass();

	// Calculates positional correction based on penatration of both objects and their inverse masses, along the normal
	const float slop = 0.02f; // Penatration allowed
	const float percent = 0.8f; // Penatration percentage to be corrected
	glm::vec3 correction = (glm::max(c->penatration - slop, 0.0f) / (invMassA + invMassB)) * c->normal * percent;

	// Pushes back objects based on correction vector and inverse mass
	c->A->physicsComponent->SetPosition(c->A->physicsComponent->GetPosition() - invMassA * correction);
	c->B->physicsComponent->SetPosition(c->B->physicsComponent->GetPosition() + invMassB * correction);
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

	// Storing inverse masses
	float invMassA = c->A->physicsComponent->GetInverseMass();
	float invMassB = c->B->physicsComponent->GetInverseMass();

	// Calculates impulse scalar
	float j = -(1 + e) * velAlongNormal;
	j /= invMassA + invMassB;

	// If both objects haven't penatrated slightly,
	// the normal hasn't been calculated, and make the impulse zero
	//if (c->penatration != 0.0f) {
		impulse = j * c->normal;
	//}
	//else {
	//	impulse = glm::vec3(0.0f, 0.0f, 0.0f);
	//}

	// Changes the velocity of  objects based on  impulse and inverse mass
	c->A->physicsComponent->SetVelocity(c->A->physicsComponent->GetVelocity() - (invMassA * impulse));
	c->B->physicsComponent->SetVelocity(c->B->physicsComponent->GetVelocity() + (invMassB * impulse));

	// Calculates the tangent vector
	glm::vec3 tangent = rv - (glm::dot(rv, c->normal) * c->normal);
	glm::normalize(tangent);

	// Calculates the tangent impulse scalar
	float jt = -glm::dot(rv, tangent);
	jt /= invMassA + invMassB;

	// Ignore tiny friction impulses
	if (jt == 0) {
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
	c->A->physicsComponent->SetVelocity(c->A->physicsComponent->GetVelocity() - invMassA * frictionImpulse);
	c->B->physicsComponent->SetVelocity(c->B->physicsComponent->GetVelocity() + invMassB * frictionImpulse);
}

void PhysicsEngine::ClosestPointOnAABB(glm::vec3 p, Box b, glm::vec3 & q){
	// If the point is outside the box, clamp it to the box
	// Else, keep it as is
	for (int i = 0; i < 3; i++) {
		float v = p[i];
		v = glm::max(v, b.c[i] - b.r[i]);
		v = glm::min(v, b.c[i] + b.r[i]);
		q[i] = v;
	}
}

float PhysicsEngine::SqDistancePointAABB(glm::vec3 p, Box b) {
	float sqDist = 0.0f;

	// Count any excess distance outside box extents for each axis
	for (int i = 0; i < 3; i++) {
		float v = p[i];
		if (v < b.c[i] - b.r[i]) sqDist += (b.c[i] - b.r[i] - v) * (b.c[i] - b.r[i] - v);
		if (v > b.c[i] + b.r[i]) sqDist += (v - b.c[i] + b.r[i]) * (v - b.c[i] + b.r[i]);
	}
	return sqDist;
}
