#include "Cube.h"

using namespace ENGINE;

Cube::Cube()
{
	renderComponent = new RenderComponent();
	renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	collisionComponent = new CollisionComponent();
	collisionComponent->CreateCollisionVolume(CollisionComponent::Collision_Type::BOX, renderComponent->getVertexList());
	physicsComponent = new PhysicsComponent();
}

Cube::~Cube() {

}

void Cube::Jump(glm::vec3 vel) {
	physicsComponent->Start();
	physicsComponent->setVelocity(glm::vec3(vel));
}

void Cube::AddVelocity(glm::vec3 vel) {
	physicsComponent->Start();
	physicsComponent->setVelocity(glm::vec3(vel));
}

void Cube::Update(const float deltaTime) {
	collisionComponent->Update(GetWorldPosition(), GetWorldScale());
	physicsComponent->Update(deltaTime, GetWorldPosition());
	SetWorldPosition(physicsComponent->getPosition());
}

void Cube::Render(Shader* shader) 
{
	renderComponent->Render(shader);
}