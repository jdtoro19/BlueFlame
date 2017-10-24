#include "Cube.h"

using namespace ENGINE;

Cube::Cube()
{
	renderComponent = new RenderComponent();
	renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	collisionComponent = new CollisionComponent();
	collisionComponent->setCollisionType(CollisionComponent::Collision_Type::BOX, renderComponent->getVertexList());
}

Cube::~Cube() {

}

void Cube::Update(const float deltaTime) {
	collisionComponent->Update(GetWorldPosition(), GetWorldScale());
}

void Cube::Render(Shader* shader) 
{
	renderComponent->Render(shader);
}