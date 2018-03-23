#include "FirePlayer.h"

using namespace GAME;

FirePlayer::FirePlayer()
{
	base = new PlayerBase("Resources/Models/Kal (Fire)/Models/Fire_0001.obj");
	base->SetWorldPosition(0.0f, 0.0f, 0.0f);
	base->SetWorldScale(0.012f);
	base->renderComponent->SetColour(0.2f, 0.7f, 0.0f);
	ring->renderComponent->SetColour(0.7f, 0.2f, 0.1f);
	specialMeter = 0;
}

FirePlayer::~FirePlayer()
{

}

std::vector<Projectile*> FirePlayer::LightAttack()
{
	// PUSH BACK TO THIS
	std::vector<Projectile*> projectiles;

	if (playerState == NORMAL && lightComboPosition == 0 && lightComboTimer <= 0)
	{
		if (worldPosition.y > 0.1) {
			physicsComponent->SetVelocity(glm::vec3(0, 7, 0));
		}

		lightComboTimer = 0.5f;
		lightComboPosition++;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::FIRE);
		p->SetImpulseForce(glm::vec3(0.0f, 100.0f, 350.0f));
		p->SetActingForce(glm::vec3(0.0f,-10.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(15.0f, 45.0f, 5.0f));
		p->SetStunTime(0.2f);
		p->SetWorldScale(0.3f);
		p->SetDamage(7);
		projectiles.push_back(p);
		return projectiles;
	}
	
	if (playerState == ATTACK && lightComboPosition == 1 && lightComboTimer > 0)
	{
		if (worldPosition.y > 0.1) {
			physicsComponent->SetVelocity(glm::vec3(0, 6, 0));
		}

		lightComboPosition++;
		lightComboTimer = 0.5f;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::FIRE);
		p->SetImpulseForce(glm::vec3(120.0f, 100.0f, 300.0f));
		p->SetActingForce(glm::vec3(-10.0f, -5.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(-35.0f, 25.0f, 5.0f));
		p->SetStunTime(0.2f);
		p->SetWorldScale(0.3f);
		p->SetDamage(7);
		projectiles.push_back(p);
		return projectiles;
	}

	if (playerState == ATTACK && lightComboPosition == 2 && lightComboTimer > 0)
	{
		if (worldPosition.y > 0.1) {
			physicsComponent->SetVelocity(glm::vec3(0, 6, 0));
		}

		lightComboPosition++;
		lightComboTimer = 0.5f;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::FIRE);
		p->SetImpulseForce(glm::vec3(-150.0f, 100.0f, 300.0f));
		p->SetActingForce(glm::vec3(10.0f, -5.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 25.0f, 5.0f));
		p->SetStunTime(0.2f);
		p->SetWorldScale(0.3f);
		p->SetDamage(7);
		projectiles.push_back(p);
		return projectiles;
	}

	return projectiles;
}

std::vector<Projectile*> FirePlayer::MediumAttack()
{
	std::vector<Projectile*> projectiles;

	if (playerState == NORMAL && mediumComboPosition == 0 && mediumComboTimer <= 0)
	{
		//FIRST PRESS JUST SHOOTS STRAIGHT
		mediumComboPosition++;
		mediumComboTimer = 0.85f;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::FIRE);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 350.0f));
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 10.0f, 10.0f));
		p->SetStunTime(0.4f);
		p->SetWorldScale(0.3f);
		p->SetDamage(8);
		projectiles.push_back(p);
		return projectiles;
	}

	if (playerState == ATTACK && mediumComboPosition == 1 && mediumComboTimer > 0)
	{
		//SECOND PRESS SHOOTS 2 PROJECTILES

		//first projectile shoots one that curves right, but it starts a little to the left
		mediumComboPosition++;
		mediumComboTimer = 0.25f;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x - collisionComponent->GetBoundingBox().r.x / 1.5f * GetWorldScale().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::FIRE);
		p->SetImpulseForce(glm::vec3(-10.0f, 0.0f, 150.0f));
		p->SetActingForce(glm::vec3(1.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 5.0f));
		p->SetStunTime(0.4f);
		p->SetWorldScale(0.3f);
		p->SetDamage(7);
		projectiles.push_back(p);

		//second projectile curves left, but it starts a little to the right
		p = new Projectile(glm::vec3(GetWorldPosition().x + collisionComponent->GetBoundingBox().r.x / 1.5f * GetWorldScale().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::FIRE);
		p->SetImpulseForce(glm::vec3(10.0f, 0.0f, 150.0f));
		p->SetActingForce(glm::vec3(-1.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 5.0f));
		p->SetStunTime(0.4f);
		p->SetWorldScale(0.3f);
		p->SetDamage(7);
		projectiles.push_back(p);
		return projectiles;
	}

	if (playerState == ATTACK && mediumComboPosition == 2 && mediumComboTimer > 0)
	{
		//THIRD PRESS SHOOTS 2 PROJECTILES /// A LAUNCHER / STRIKE DOWN COMBO

		//first projectile shoots straight, no delay, and launches up
		mediumComboPosition++;
		mediumComboTimer = 1.0f;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::FIRE);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 150.0f));
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 120.0f, 10.0f));
		p->SetStunTime(0.4f);
		p->SetWorldScale(0.3f);
		p->SetDamage(7);
		projectiles.push_back(p);

		//second projectile delays for 0.5 seconds, then hits player downwards
		p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + collisionComponent->GetBoundingBox().r.y / 1.5f * GetWorldScale().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::FIRE);
		p->SetImpulseForce(glm::vec3(0.0f, 180.0f, 375.0f));
		p->SetFirstDelay(0.4f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f), glm::vec3(0.5f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetActingForce(glm::vec3(0.0f, -15.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, -200.0f, 15.0f));
		p->SetStunTime(0.4f);
		p->SetDamage(10);
		projectiles.push_back(p);
		return projectiles;
	}

	return projectiles;
}

std::vector<Projectile*> FirePlayer::HeavyAttack()
{
	std::vector<Projectile*> projectiles;

	if (playerState == NORMAL && heavyComboPosition == 0 && heavyComboTimer <= 0)
	{
		heavyComboPosition++;
		heavyComboTimer = 0.5f;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + collisionComponent->GetBoundingBox().r.y / 1.5f * GetWorldScale().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::FIRE);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 450.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 150.0f, 0.0f));
		p->SetStunTime(0.4f);
		p->SetWorldScale(0.8f);
		p->SetDamage(14);

		projectiles.push_back(p);
		return projectiles;
	}

	if (playerState == ATTACK && heavyComboPosition == 1 && heavyComboTimer > 0)
	{
		heavyComboPosition++;
		heavyComboTimer = 0.5f;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + collisionComponent->GetBoundingBox().r.y / 1.5f * GetWorldScale().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::FIRE);
		p->SetImpulseForce(glm::vec3(0.0f, 100.0f, 450.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 0.0f));
		p->SetStunTime(0.4f);
		p->SetWorldScale(0.8f);
		p->SetDamage(14);

		projectiles.push_back(p);
		return projectiles;
	}

	if (playerState == ATTACK && heavyComboPosition == 2 && heavyComboTimer > 0)
	{
		heavyComboPosition++;
		heavyComboTimer = 1.2f;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + collisionComponent->GetBoundingBox().r.y / 1.5f * GetWorldScale().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::FIRE);
		p->SetImpulseForce(glm::vec3(0.0f, 150.0f, 450.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 0.0f));
		p->SetStunTime(0.4f);
		p->SetWorldScale(0.8f);
		p->SetDamage(14);

		projectiles.push_back(p);
		return projectiles;
	}

	return projectiles;
}

std::vector<Projectile*> FirePlayer::SpecialAttack()
{
	std::vector<Projectile*> projectiles;

	if (heavyComboPosition == 3 && specialMeter >= 75) {

		specialMeter -= 75;
		if (specialMeter < 0) {
			specialMeter = 0;
		}

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::FIRE);
		p->SetClipping(ENGINE::PROJECTILE_CLIP::YES_PROJECTILE);
		p->SetImpulseForce(glm::vec3(0.0f, -60.0f, 200.0f));
		p->SetFirstDelay(2.0f, glm::vec3(0.0f, 3.5f, 0.0f), glm::vec3(0.5f), glm::vec3(2.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 50.0f));
		p->SetStunTime(3.0f);
		p->SetDamage(90);

		projectiles.push_back(p);
		return projectiles;
	}

	if (playerState == NORMAL && specialComboPosition == 0 && specialComboTimer <= 0 && specialMeter >= 25)
	{
		tempSpecialProjs.clear();
		specialComboPosition++;
		specialComboTimer = 1.5f;

		specialMeter -= 25;
		if (specialMeter < 0) {
			specialMeter = 0;
		}

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::FIRE);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 250.0f));
		p->SetFirstDelay(1.0f, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.3f), glm::vec3(0.3f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetSecondDelay(10000.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetActingForce(glm::vec3(0.0f, -2.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, -200.0f, 15.0f));
		p->SetStunTime(0.4f);
		p->SetDamage(15);

		tempSpecialProjs.push_back(p);
		projectiles.push_back(p);
		return projectiles;
	}
	if (playerState == ATTACK && specialComboPosition == 1 && specialComboTimer > 0 && specialMeter >= 25)
	{
		specialComboPosition++;
		specialComboTimer = 1.5f;

		specialMeter -= 25;
		if (specialMeter < 0) {
			specialMeter = 0;
		}

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::FIRE);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 250.0f));
		p->SetFirstDelay(1.0f, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.3f), glm::vec3(0.3f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetSecondDelay(10000.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetActingForce(glm::vec3(0.0f, -2.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, -200.0f, 15.0f));
		p->SetStunTime(0.4f);
		p->SetDamage(15);

		tempSpecialProjs.push_back(p);
		projectiles.push_back(p);
		return projectiles;
	}
	if (playerState == ATTACK && specialComboPosition == 2 && specialComboTimer > 0 && specialMeter >= 25)
	{
		specialComboPosition++;
		specialComboTimer = 1.5f;

		specialMeter -= 25;
		if (specialMeter < 0) {
			specialMeter = 0;
		}

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::FIRE);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 250.0f));
		p->SetFirstDelay(1.0f, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.3f), glm::vec3(0.3f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetSecondDelay(10000.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetActingForce(glm::vec3(0.0f, -2.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, -200.0f, 15.0f));
		p->SetStunTime(0.4f);
		p->SetDamage(15);

		tempSpecialProjs.push_back(p);
		projectiles.push_back(p);
		return projectiles;
	}
	if (playerState == ATTACK && specialComboPosition == 3 && specialComboTimer > 0 && specialMeter >= 25)
	{
		specialComboPosition++;
		specialComboTimer = 1.5f;

		specialMeter -= 25;
		if (specialMeter < 0) {
			specialMeter = 0;
		}

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::FIRE);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 250.0f));
		p->SetFirstDelay(1.0f, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.3f), glm::vec3(0.3f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetSecondDelay(10000.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetActingForce(glm::vec3(0.0f, -2.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, -200.0f, 15.0f));
		p->SetStunTime(0.4f);
		p->SetDamage(15);

		tempSpecialProjs.push_back(p);
		projectiles.push_back(p);
		return projectiles;
	}

	if (lightComboPosition == 3 && specialMeter >= 20)
	{
		specialComboTimer = 1.5f;

		specialMeter -= 20;
		if (specialMeter < 0) {
			specialMeter = 0;
		}

		physicsComponent->SetPosition(glm::vec3(physicsComponent->GetPosition().x, physicsComponent->GetPosition().y + 2, physicsComponent->GetPosition().z));
		base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), 0.2f);
		physicsComponent->SetVelocity(glm::vec3(0, 5, 0));

		ComboReset();

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::FIRE);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 250.0f));
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 100.0f, 0.0f));
		p->SetStunTime(0.4f);
		p->SetDamage(5);

		projectiles.push_back(p);
		return projectiles;
	}

	return projectiles;
}

void FirePlayer::InheritedUpdate(const float deltaTime)
{
	//stop the player from moving when attacking
	if (playerState == ATTACK)
	{
		physicsComponent->SetVelocity(glm::vec3(0.0f, physicsComponent->GetVelocity().y, 0.0f));
	}

	if (specialComboPosition == 1) {
		physicsComponent->SetVelocity(glm::vec3(moveSpeed * 300 * deltaTime * dir, physicsComponent->GetVelocity().y, physicsComponent->GetVelocity().z));
		base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), -0.2f);
	}
	else if (specialComboPosition == 2) {
		physicsComponent->SetVelocity(glm::vec3(moveSpeed * 300 * deltaTime * dir, physicsComponent->GetVelocity().y, physicsComponent->GetVelocity().z));
		base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), -0.2f);
	}
	else if (specialComboPosition == 3) {
		physicsComponent->SetVelocity(glm::vec3(moveSpeed * 300 * deltaTime * dir, physicsComponent->GetVelocity().y, physicsComponent->GetVelocity().z));
		base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), -0.2f);
	}
	else if (specialComboPosition == 4) {
		physicsComponent->SetVelocity(glm::vec3(moveSpeed * 300 * deltaTime * dir, physicsComponent->GetVelocity().y, physicsComponent->GetVelocity().z));
		base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), -0.2f);
	}

	if (specialComboTimer <= 0)
	{
		for (int i = 0; i < tempSpecialProjs.size(); i++)
		{
			tempSpecialProjs.at(i)->StopDelay();
		}
	}
}

void FirePlayer::InheritedHandleEvents(SDL_Event events)
{

}

void FirePlayer::InheritedHandleStates(const Uint8 *state)
{

}