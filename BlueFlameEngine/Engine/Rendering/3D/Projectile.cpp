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
	SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), -_angle);
	physicsComponent->SetPosition(p);
	dir = _dir;
	angle = _angle;
	physicsComponent->SetMass(50.0f);
	collisionComponent->SetLayer(2);
	physicsComponent->SetDestructible(true);
	physicsComponent->hasGravity = false;
	rip = 200;
	actingForce = glm::vec3(0.0f, -4.0f, 0.0f);
	knockbackForce = glm::vec3(40.0f, 150.0f, 0.0f);
	delay = -1.0f;
	delayTimer = -1.0f;
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
	beginScale = glm::vec3(0.0f);
	endScale = glm::vec3(0.0f);
	endPosition = glm::vec3(0.0f);

	lifetime = Cooldown(5.0); //default lifetime
	lifetime.startCD();
}

Projectile::Projectile(glm::vec3 p, glm::vec3 _force, float _angle, int _dir, float _delay)
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
	SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), -_angle);
	physicsComponent->SetPosition(p);
	dir = _dir;
	angle = _angle;
	physicsComponent->SetMass(50.0f);
	collisionComponent->SetLayer(2);
	physicsComponent->SetDestructible(true);
	physicsComponent->hasGravity = false;
	rip = 200;
	actingForce = glm::vec3(0.0f, -4.0f, 0.0f);
	knockbackForce = glm::vec3(40.0f, 150.0f, 0.0f);
	delay = _delay;
	delayTimer = 0.0f;
	stunTime = 1.0f;
	damage = 1;
	canFlipX = false;
	canFlipY = false;
	flipIntervalX = 0.5f;
	flipIntervalY = 0.75f;
	flipTimeX = flipIntervalX / 2.0f;
	flipTimeY = flipIntervalY / 2.0f;
	impulseForce = glm::rotateY(_force, angle);
	beginScale = glm::vec3(0.0f);
	endScale = glm::vec3(0.0f);
	endPosition = glm::vec3(0.0f);

	lifetime = Cooldown(5.0); //default lifetime
	lifetime.startCD();
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

void Projectile::SetDelayTime(float _time) {
	delay = _time;
}

void Projectile::SetStunTime(float time) {
	stunTime = time;
}

void Projectile::SetDamage(int d) {
	damage = d;
}

void Projectile::SetScaleChange(glm::vec3 _beginScale, glm::vec3 _endScale) {
	beginScale = _beginScale;
	endScale = _endScale;
	SetWorldScale(beginScale);
	//physicsComponent->SetPosition(physicsComponent->GetPosition().z + (endScale - 1.0f));
}

void Projectile::SetPositionChange(glm::vec3 _endPosition) {
	endPosition = _endPosition + worldPosition;
	SetWorldPosition(worldPosition);
	//physicsComponent->SetPosition(physicsComponent->GetPosition().z + (endScale - 1.0f));
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

void Projectile::addMaxDistance(float distance) {
    maximumDistance = true;
    maxD = distance;
    originalPos = worldPosition;
}

void Projectile::setLifetime(double seconds) {
    lifetime.setNewDuration(seconds);
    lifetime.startCD();
}

void Projectile::Update(const float deltaTime) {
	if (deleted == false && !lifetime.checkOffCD()) {

		if (maximumDistance == false || glm::length(worldPosition - originalPos) <= maxD) {

			if (delay > 0.0f && delayTimer <= delay) {
				delayTimer += deltaTime;
				if (beginScale != glm::vec3(0.0f) && endScale != glm::vec3(0.0f)) {
					SetWorldScale(glm::lerp(beginScale, endScale, delayTimer / delay));
				}
				if (endPosition != glm::vec3(0.0f)) {
					SetWorldPosition(glm::lerp(worldPosition, endPosition, delayTimer / delay));
				}
			}
			else if (delayTimer == -1.0f && delay == -1.0f) {
				if (beginScale != glm::vec3(0.0f) && endScale != glm::vec3(0.0f)) {
					SetWorldScale(glm::lerp(beginScale, endScale, (float)lifetime.lerp()));
					SetWorldPosition(worldPosition); 
				}
			}

			if (delayTimer > delay) {
				physicsComponent->AddForce(glm::vec3(impulseForce.x, impulseForce.y, impulseForce.z * -dir));
				delayTimer = 0.0f;
				delay = 0.0f;
			}


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

			if (delay <= 0.0f && delayTimer <= 0.0f) {
				physicsComponent->AddForce(actingForce);
			}

			physicsComponent->Update(deltaTime);
		}
		SetWorldPosition(physicsComponent->GetPosition());
		collisionComponent->Update(GetWorldPosition());
	}
	else if (lifetime.checkOffCD()) {
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