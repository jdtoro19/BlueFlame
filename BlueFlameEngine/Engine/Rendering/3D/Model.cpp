#include "Model.h"

using namespace ENGINE;

Model::Model()
{
}

Model::Model(std::string path)
{
	model = new ModelComponent(path);
	isModel = true;
	collisionComponent = new CollisionComponent();
	collisionComponent->CreateCollisionVolume(CollisionComponent::Collision_Type::BOX, model->meshes);
	collisionComponent->SetBoxPadding(glm::vec3(0.3f, 1.0f, 0.95f));
	physicsComponent = new PhysicsComponent();
}

Model::~Model()
{
}

void Model::Update(const float deltaTime) {
	physicsComponent->Update(deltaTime);
	collisionComponent->Update(GetWorldPosition(), GetWorldScale());
	SetWorldPosition(physicsComponent->getPosition());
}

void Model::Render(Shader* shader) {
	model->Render(shader);
}