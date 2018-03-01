#include "Projectile.h"

using namespace ENGINE;

Projectile::Projectile(glm::vec3 p, glm::vec3 _force, float _angle, int _dir)
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
	angle = _angle;
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
	flipIntervalX = 0.5f;
	flipIntervalY = 0.75f;
	flipTimeX = flipIntervalX / 2.0f;
	flipTimeY = flipIntervalY / 2.0f;
	glm::vec3 force = glm::rotateY(_force, angle);
	physicsComponent->AddForce(glm::vec3(force.x, force.y, force.z * -dir));
}

Projectile::~Projectile() {

}

void Projectile::SetActingForce(glm::vec3 _force) {
	actingForce = glm::rotateY(_force, angle);
	actingForce.z *= -dir;
}

void Projectile::SetKnockbackForce(glm::vec3 _force) {
	knockbackForce = glm::rotateY(_force, angle);
	knockbackForce.z *= -dir;
}

void Projectile::SetStunTime(float time) {
	stunTime = time;
}

void Projectile::SetDamage(int d) {
	damage = d;
}

glm::vec3 Projectile::GetForce() {
	return knockbackForce;
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


		physicsComponent->AddForce(actingForce);

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