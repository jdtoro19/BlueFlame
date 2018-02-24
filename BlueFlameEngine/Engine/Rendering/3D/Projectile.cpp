#include "Projectile.h"

using namespace ENGINE;

Projectile::Projectile(glm::vec3 p, glm::vec3 _force, int _dir)
{
	worldPosition = p;
	renderComponent = new RenderComponent();
	renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	collisionComponent = new CollisionComponent();
	collisionComponent->CreateCollisionVolume(CollisionComponent::Collision_Type::BOX, renderComponent->getVertexList());
	physicsComponent = new PhysicsComponent();
	physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::PERFECT_NON_ELASTIC);
	physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	SetWorldScale(0.5f);
	physicsComponent->SetPosition(p);
	dir = _dir;
	physicsComponent->SetMass(50.0f);
	collisionComponent->SetLayer(1);
	physicsComponent->SetDestructible(true);
	physicsComponent->hasGravity = false;
	rip = 200;

	actingForce = glm::vec3(0.0f, -4.0f, 0.0f);
	knockbackForce = glm::vec3(0.0f, 50.0f, 25.0f);
	stunTime = 1.0f;
	damage = 1;
	canFlipX = false;
	canFlipY = false;
	flipTimeX = 0.0f;
	flipTimeY = 0.0f;
	flipIntervalX = 0.5f;
	flipIntervalY = 0.75f;
	physicsComponent->AddForce(glm::vec3(_force.x, _force.y, _force.z * -dir));
}

Projectile::~Projectile() {

}

void Projectile::SetActingForce(glm::vec3 _force) {
	actingForce = _force;
}

void Projectile::SetKnockbackForce(glm::vec3 _force) {
	knockbackForce = _force;
}

void Projectile::SetStunTime(float time) {
	stunTime = time;
}

void Projectile::SetDamage(int d) {
	damage = d;
}

glm::vec3 Projectile::GetForce() {
	return glm::vec3(knockbackForce.x, knockbackForce.y, knockbackForce.z * -dir);
}

float Projectile::GetStunTime() {
	return stunTime;
}

int Projectile::GetDamage() {
	return damage;
}

void Projectile::Update(const float deltaTime) {
	if (deleted == false && rip > 0) {
		if (canFlipX) {
			flipTimeX += deltaTime;
		}

		if (canFlipY) {
			flipTimeY += deltaTime;
		}

		if (flipTimeX >= flipIntervalX) {
			actingForce.x = -actingForce.x;
			flipTimeX = 0.0f;
		}

		if (flipTimeY >= flipIntervalY) {
			actingForce.x = -actingForce.x;
			flipTimeY = 0.0f;
		}


		physicsComponent->AddForce(glm::vec3(actingForce.x, actingForce.y, actingForce.z * -dir));

		physicsComponent->Update(deltaTime);
		SetWorldPosition(physicsComponent->GetPosition());
		collisionComponent->Update(GetWorldPosition());
		--rip;
	}
	else if (rip == 0) {
		deleted = true;
		//delete this;
	}
}

void Projectile::FixedUpdate(const float deltaTime) {
	
}

void Projectile::Render(Shader* shader)
{
	renderComponent->Render(shader);
}