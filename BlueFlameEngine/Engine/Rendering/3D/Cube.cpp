#include "Cube.h"

using namespace ENGINE;

Cube::Cube()
{
	renderComponent = new RenderComponent();
	renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	collisionComponent = new CollisionComponent();
	collisionComponent->CreateCollisionVolume(CollisionComponent::Collision_Type::BOX, renderComponent->getVertexList());
	physicsComponent = new PhysicsComponent();
	// GRAVITY
	physicsComponent->SetAcceleration(glm::vec3(0.0f, -25.0f, 0.0f));
}

Cube::~Cube() {

}

void Cube::Jump(glm::vec3 vel) {
	physicsComponent->SetVelocity(glm::vec3(vel));
}

void Cube::AddVelocity(glm::vec3 vel) {
	physicsComponent->SetVelocity(glm::vec3(vel));
}

void Cube::Update(const float deltaTime) {
	physicsComponent->Update(deltaTime);
	SetWorldPosition(physicsComponent->GetPosition());
	SetWorldScale(collisionComponent->GetScale());
	collisionComponent->Update(GetWorldPosition());
}

void Cube::Render(Shader* shader)
{
	renderComponent->Render(shader);
}

void Cube::Fire() {

}