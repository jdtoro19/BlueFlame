#include "Cube.h"

using namespace ENGINE;

Cube::Cube()
{
	renderComponent = new RenderComponent();
	renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	collisionComponent = new CollisionComponent();
	collisionComponent->setCollisionType(CollisionComponent::Collision_Type::BOX, renderComponent->getVertexList());
	physicsComponent = new PhysicsComponent();
	physicsComponent->setPosition(GetWorldPosition());
}

Cube::~Cube() {

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