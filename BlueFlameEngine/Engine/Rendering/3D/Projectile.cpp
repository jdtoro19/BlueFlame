#include "Projectile.h"

using namespace ENGINE;

Projectile::Projectile(glm::vec3 p, int dir)
{
	worldPosition = p;
	renderComponent = new RenderComponent();
	renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	collisionComponent = new CollisionComponent();
	collisionComponent->CreateCollisionVolume(CollisionComponent::Collision_Type::BOX, renderComponent->getVertexList());
	physicsComponent = new PhysicsComponent();
	physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::PERFECT_NON_ELASTIC);
	physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	SetWorldScale(0.25f);
	physicsComponent->SetPosition(p);
	physicsComponent->SetVelocity(glm::vec3(0.0f, 0.0, -25.0f * dir));
	physicsComponent->SetMass(50.0f);
	physicsComponent->SetDestructible(true);
	rip = 100;
}

Projectile::~Projectile() {

}

void Projectile::Jump(glm::vec3 vel) {
	//physicsComponent->SetVelocity(glm::vec3(vel));
}

void Projectile::AddVelocity(glm::vec3 vel) {
	physicsComponent->SetVelocity(glm::vec3(vel));
}

void Projectile::Update(const float deltaTime) {
	if (deleted == false && rip > 0) {
		physicsComponent->Update(deltaTime);
		SetWorldPosition(physicsComponent->GetPosition());
		collisionComponent->Update(GetWorldPosition(), GetWorldScale());
		--rip;
	}
	else if (rip == 0){
		deleted = true;
		//delete this;
	}
}

void Projectile::Render(Shader* shader)
{
	renderComponent->Render(shader);
}