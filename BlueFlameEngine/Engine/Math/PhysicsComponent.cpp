#include "PhysicsComponent.h"

using namespace ENGINE;

PhysicsComponent::PhysicsComponent() {
	acceleration = glm::vec3(0.0f, -0.5f, 0.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
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

glm::vec3 PhysicsComponent::getPosition() {
	return position;
}

void PhysicsComponent::Update(float deltaTime, glm::vec3 pos) {
	position = pos;
	velocity += acceleration * deltaTime;
	position += velocity * deltaTime + 1.0f / 2.0f * acceleration * deltaTime * deltaTime;
}

void PhysicsComponent::Render() {

}