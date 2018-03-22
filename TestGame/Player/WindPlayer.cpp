#include "WindPlayer.h"

using namespace GAME;

WindPlayer::WindPlayer() {
	base = new PlayerBase("Resources/Models/Robot_Base_Greybox/Robot_Var_002_Gurran.obj");
	base->SetWorldPosition(0.0f, 0.0f, 0.0f);
	base->SetWorldScale(0.012f);
	base->renderComponent->SetColour(0.2f, 0.7f, 0.0f);

	ring->renderComponent->SetColour(0.1f, 0.1f, 0.5f);
	SetStats();
}

WindPlayer::~WindPlayer() {

}

void WindPlayer::SetStats() {
	shieldHealth = maxHealth;
	moveSpeed = 1.0f;
	specialMeter = 0;
}

std::vector<Projectile*> WindPlayer::LightAttack()
{
	std::vector<Projectile*> projectiles;

	if (playerState == NORMAL && lightComboPosition == 0 && playerState != BLOCK && playerState != STUN) {

		if (worldPosition.y > 0.1) {
			physicsComponent->SetVelocity(glm::vec3(0, 5, 0));
		}

		lightComboTimer = 0.5f;
		lightComboPosition++;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 200.0f));
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 25.0f, 10.0f));
		p->SetStunTime(0.3f);
		projectiles.push_back(p);
		return projectiles;
	}
	if (lightComboTimer > 0 && lightComboPosition == 1 && playerState != BLOCK && playerState != STUN) {

		if (worldPosition.y > 0.1) {
			physicsComponent->SetVelocity(glm::vec3(0, 5, 0));
		}

		lightComboTimer = 0.5f;
		lightComboPosition++;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(125.0f, 0.0f, 200.0f));
		p->SetActingForce(glm::vec3(-10.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 100.0f, 10.0f));
		p->SetStunTime(0.3f);
		p->SetWorldScale(0.5f);
		projectiles.push_back(p);
		return projectiles;
	}
	if (lightComboTimer > 0 && lightComboPosition == 2 && playerState != BLOCK && playerState != STUN) {

		if (worldPosition.y > 0.1) {
			physicsComponent->SetVelocity(glm::vec3(0, 5, 0));
		}

		lightComboTimer = 0.5f;
		lightComboPosition++;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(-200.0f, 335.0f, 375.0f));
		p->SetActingForce(glm::vec3(20.0f, -35.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, -100.0f, 0.0f));
		p->SetStunTime(0.5f);
		p->SetWorldScale(0.5f);
		projectiles.push_back(p);
		return projectiles;
	}
	
	return projectiles;
}
std::vector<Projectile*> WindPlayer::MediumAttack()
{
	std::vector<Projectile*> projectiles;

	if (playerState == NORMAL && mediumComboPosition == 0 && playerState != BLOCK && playerState != STUN) {

		mediumComboTimer = 0.5f;
		mediumComboPosition++;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(50.0f, 0.0f, 150.0f));
		p->SetActingForce(glm::vec3(-2.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(20.0f, 25.0f, 5.0f));
		p->SetStunTime(0.4f);
		p->SetWorldScale(0.5f);
		projectiles.push_back(p);
		return projectiles;
	}
	if (mediumComboTimer > 0 && mediumComboTimer < 0.2f && mediumComboPosition == 1 && playerState != BLOCK && playerState != STUN) {

		mediumComboTimer = 0.5f;
		mediumComboPosition++;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(50.0f, 0.0f, 150.0f));
		p->SetActingForce(glm::vec3(-2.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(20.0f, 25.0f, 5.0f));
		p->SetStunTime(0.4f);
		p->SetWorldScale(0.5f);
		projectiles.push_back(p);
		return projectiles;
	}
	if (mediumComboTimer > 0 && mediumComboTimer < 0.2f && mediumComboPosition == 2 && playerState != BLOCK && playerState != STUN) {

		mediumComboTimer = 1.0f;
		mediumComboPosition++;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 130.0f));
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 150.0f, 5.0f));
		p->SetStunTime(0.6f);
		p->SetWorldScale(0.5f);
		projectiles.push_back(p);
		return projectiles;
	}
	
	return projectiles;
}
std::vector<Projectile*> WindPlayer::HeavyAttack()
{
	std::vector<Projectile*> projectiles;

	if (playerState == NORMAL && heavyComboPosition == 0 && playerState != BLOCK && playerState != STUN) {

		heavyComboTimer = 0.7f;
		heavyComboPosition++;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 75.0f));
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 20.0f));
		p->SetStunTime(0.8f);
		p->SetWorldScale(1.0f, 0.2f, 1.0f);
		projectiles.push_back(p);
		return projectiles;
	}
	if (heavyComboTimer > 0 && heavyComboTimer < 0.2f && heavyComboPosition == 1 && playerState != BLOCK && playerState != STUN) {

		heavyComboTimer = 1.2f;
		heavyComboPosition++;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 75.0f));
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 25.0f));
		p->SetStunTime(1.5f);
		p->SetWorldScale(0.2f, 1.0f, 1.0f);
		projectiles.push_back(p);
		return projectiles;
	}
	if (lightComboPosition == 3) {

		lightComboTimer = 1.0f;
		lightComboPosition++;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 25.0f));
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 70.0f, 25.0f));
		p->SetStunTime(2.0f);
		p->SetWorldScale(1.5f, 1.5f, 0.2f);
		projectiles.push_back(p);
		return projectiles;
	}
	
	return projectiles;
}
std::vector<Projectile*> WindPlayer::SpecialAttack()
{
	std::vector<Projectile*> projectiles;

	if ((playerState == NORMAL && worldPosition.y < 0.1f && specialMeter > 0) || mediumComboPosition == 3) {

		if (mediumComboPosition != 3) {
			specialMeter -= 10;
			if (specialMeter < 0) {
				specialMeter = 0;
			}
		}

		physicsComponent->SetPosition(glm::vec3(physicsComponent->GetPosition().x, physicsComponent->GetPosition().y + 2, physicsComponent->GetPosition().z));
		base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), 0.2f);

		physicsComponent->SetVelocity(glm::vec3(0, 5, 0));

		ComboReset();
	}

	if (heavyComboPosition == 2 && specialMeter > 0) {

		specialMeter -= 25;
		if (specialMeter < 0) {
			specialMeter = 0;
		}

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 200.0f));
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 100.0f));
		p->SetStunTime(0.1f);
		p->SetWorldScale(3.0f, 0.2f, 1.0f);
		projectiles.push_back(p);
		return projectiles;
	}

	if (playerState == NORMAL && worldPosition.y > 0.1f && !airAttack && specialMeter > 0) {

		specialMeter -= 50;
		if (specialMeter < 0) {
			specialMeter = 0;
		}

		physicsComponent->SetVelocity(glm::vec3(0, 5, 0));

		airAttack = true;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(0.0f, 200.0f, 200.0f));
		p->SetActingForce(glm::vec3(0.0f, -25.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 100.0f, 60.0f));
		p->SetStunTime(0.5f);
		p->SetWorldScale(6.0f, 0.2f, 1.0f);
		projectiles.push_back(p);
		return projectiles;
	}

	if (lightComboPosition == 3 && specialMeter > 0) {
		ComboReset();

		specialMeter -= 50;
		if (specialMeter < 0) {
			specialMeter = 0;
		}

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(00.0f, 0.0f, 500.0f));
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 25.0f));
		p->SetStunTime(0.3f);
		p->SetWorldScale(0.5f);
		projectiles.push_back(p);
		return projectiles;
	}

	return projectiles;
}
void WindPlayer::InheritedUpdate(const float deltaTime)
{
	if (worldPosition.y < 0.1f) {
		airAttack = false;
	}

	if (playerState == ATTACK) 
	{
		physicsComponent->SetVelocity(glm::vec3(0.0f, physicsComponent->GetVelocity().y, 0.0f));
	}

	if (mediumComboPosition == 1) {
		physicsComponent->SetVelocity(glm::vec3(moveSpeed * 1.1f *  deltaTime * 500 * dir, physicsComponent->GetVelocity().y, physicsComponent->GetVelocity().z));
		base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), -0.2f);
	}
	if (mediumComboPosition == 2) {
		physicsComponent->SetVelocity(glm::vec3(-moveSpeed * 1.1f * deltaTime * 500 * dir, physicsComponent->GetVelocity().y, physicsComponent->GetVelocity().z));
		base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), 0.2f);
	}
}
void WindPlayer::InheritedHandleEvents(SDL_Event events)
{

}
void WindPlayer::InheritedHandleStates(const Uint8 *state)
{

}