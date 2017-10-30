#include "PhysicsComponent.h"

#include <iostream>

using namespace ENGINE;

PhysicsComponent::PhysicsComponent() {
	acceleration = glm::vec3(0.0f, -5.0f, 0.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	isStopped = false;
}

PhysicsComponent::~PhysicsComponent() {

}

void PhysicsComponent::setPhysicsType(Physics_Type pt) {
	physicsType = pt;
}

PhysicsComponent::Physics_Type PhysicsComponent::getPhysicsType() {
	return physicsType;
}

void PhysicsComponent::setVelocity(glm::vec3 vel) {
	velocity = vel;
}

void PhysicsComponent::setPosition(glm::vec3 pos) {
	position = pos;
}

void PhysicsComponent::setAcceleration(glm::vec3 accel) {
	acceleration = accel;
}

void PhysicsComponent::SetVelocity(glm::vec3 vel) {
	velocity = vel;
}

void PhysicsComponent::SetMass(float _mass) {
	mass = _mass;
}

void PhysicsComponent::Stop(glm::vec3 otherPos) {
	isStopped = true;
}

void PhysicsComponent::Start() {
	isStopped = false;
}

glm::vec3 PhysicsComponent::getPosition() {
	return position;
}

glm::vec3 PhysicsComponent::GetVelocity() {
	return velocity;
}

float PhysicsComponent::GetMass() {
	return mass;
}

void PhysicsComponent::Update(float deltaTime) {
	if (physicsType == DYNAMIC) {
		if (isStopped == false) {
			velocity += acceleration * deltaTime;
			position += velocity * deltaTime + 1.0f / 2.0f * acceleration * deltaTime * deltaTime;
		}
		else if (isStopped == true) {
			velocity = glm::vec3(0.0f, 0.0f, 0.0f);
			std::cout << "STOPPED" << std::endl;
		}
	}
}

void PhysicsComponent::Render() {

}