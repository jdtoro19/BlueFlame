#include "IcePlayer.h"

using namespace GAME;

IcePlayer::IcePlayer() {
	ring->renderComponent->SetColour(0.4f, 0.7f, 1.0f);
	HeavyCD = Cooldown(5.0);
	LightCD = Cooldown(0.3);
	MediumCD = Cooldown(2.0);
	moveWhileShooting = true;
}

IcePlayer::~IcePlayer() {

}

std::vector<Projectile*> IcePlayer::LightAttack()
{
	std::vector<Projectile*> projectiles;
		if (lightComboTimer <= 0.0f && lightComboPosition == 0 && playerState != BLOCK && playerState != STUN) {
			
			lightComboTimer = 0.5f;
			lightComboPosition++;

			Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x - 0.25f, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
			p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 100.0f));
			p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
			p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 30.0f));
			p->SetFirstDelay(0.05f, glm::vec3(0.0f), glm::vec3(0.05f, 0.05f, 0.25f), glm::vec3(0.2f, 0.2f, 0.75f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
			p->SetStunTime(0.3f);
			projectiles.push_back(p);
		}
		else if (lightComboTimer > 0.0f && lightComboPosition == 1 && playerState != BLOCK && playerState != STUN) {

			lightComboTimer = 0.5f;
			lightComboPosition++;

			Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x + 0.25f, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
			p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 100.0f));
			p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
			p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 30.0f));
			p->SetFirstDelay(0.05f, glm::vec3(0.0f), glm::vec3(0.05f, 0.05f, 0.25f), glm::vec3(0.2f, 0.2f, 0.75f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
			p->SetStunTime(0.3f);
			projectiles.push_back(p);
		}

		else if (lightComboTimer > 0.0f && lightComboPosition == 2 && playerState != BLOCK && playerState != STUN) {

			lightComboTimer = 0.5f;
			lightComboPosition = 0;

			Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z  - 0.5f * dir), targetAngle, dir);
			p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 100.0f));
			p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
			p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 20.0f));
			p->SetFirstDelay(0.15f, glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.25f, 0.25f, 0.25f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
			p->SetSecondDelay(0.15f, glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.75f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
			p->SetStunTime(0.3f);
			projectiles.push_back(p);

			Projectile* p1 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z - 0.5f * dir), targetAngle, dir);
			p1->SetImpulseForce(glm::vec3(0.0f, 0.0f, 100.0f));
			p1->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
			p1->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 20.0f));
			p1->SetFirstDelay(0.15f, glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.25f, 0.25f, 0.25f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
			p1->SetSecondDelay(0.15f, glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.75f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
			p1->SetStunTime(0.3f);
			projectiles.push_back(p1);
		}

	return projectiles;
}
std::vector<Projectile*> IcePlayer::MediumAttack()
{
	std::vector<Projectile*> projectiles;
	if (MediumCD.checkOffCD()) {
		if (mediumComboPosition == 0 && playerState != BLOCK && playerState != STUN) {
			/*Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + 1.0f, GetWorldPosition().z - 1.0f * -dir), targetAngle, dir);
			p->SetImpulseForce(glm::vec3(0.0f, -2.0f, 0.0f));
			p->SetActingForce(glm::vec3(0.0f, -4.0f, 0.0f));
			p->SetKnockbackForce(glm::vec3(0.0f));
			p->SetFirstDelay(0.5f, glm::vec3(1.0f, 6.0f, 1.0f), glm::vec3(0.25f, 0.95f, 0.25f), glm::vec3(0.25f, 0.95f, 0.25f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
			p->SetSecondDelay(0.75f, glm::vec3(-1.0f, 0.0f, 8.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
			p->SetStunTime(2.5f);
			p->SetShader(pShader);
			BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);
			projectiles.push_back(p);
			Projectile* p1 = new Projectile(glm::vec3(GetWorldPosition().x - 1.0f, GetWorldPosition().y + 1.0f, GetWorldPosition().z * dir), targetAngle, dir);
			p1->SetImpulseForce(glm::vec3(0.0f, -2.0f, 0.0f));
			p1->SetActingForce(glm::vec3(0.0f, -4.0f, 0.0f));
			p1->SetKnockbackForce(glm::vec3(0.0f));
			p1->SetFirstDelay(0.5f, glm::vec3(1.0f, 6.0f, -1.0f), glm::vec3(0.25f, 0.95f, 0.25f), glm::vec3(0.25f, 0.95f, 0.25f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
			p1->SetSecondDelay(0.75f, glm::vec3(1.0f, 0.0f, 8.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
			p1->SetStunTime(2.5f);
			p1->SetShader(pShader);
			BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p1);
			projectiles.push_back(p1);
			Projectile* p2 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + 1.0f, GetWorldPosition().z + 1.0f * -dir), targetAngle, dir);
			p2->SetImpulseForce(glm::vec3(0.0f, -2.0f, 0.0f));
			p2->SetActingForce(glm::vec3(0.0f, -4.0f, 0.0f));
			p2->SetKnockbackForce(glm::vec3(0.0f));
			p2->SetFirstDelay(0.5f, glm::vec3(-1.0f, 6.0f, -1.0f), glm::vec3(0.25f, 0.95f, 0.25f), glm::vec3(0.25f, 0.95f, 0.25f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
			p2->SetSecondDelay(0.75f, glm::vec3(1.0f, 0.0f, 6.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
			p2->SetStunTime(2.5f);
			p2->SetShader(pShader);
			BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p2);
			projectiles.push_back(p2);
			Projectile* p3 = new Projectile(glm::vec3(GetWorldPosition().x + 1.0f, GetWorldPosition().y + 1.0f, GetWorldPosition().z * dir), targetAngle, dir);
			p3->SetImpulseForce(glm::vec3(0.0f, -2.0f, 0.0f));
			p3->SetActingForce(glm::vec3(0.0f, -4.0f, 0.0f));
			p3->SetKnockbackForce(glm::vec3(0.0f));
			p3->SetFirstDelay(0.5f, glm::vec3(-1.0f, 6.0f, 1.0f), glm::vec3(0.25f, 0.95f, 0.25f), glm::vec3(0.25f, 0.95f, 0.25f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
			p3->SetSecondDelay(0.75f, glm::vec3(-1.0f, 0.0f, 6.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
			p3->SetStunTime(2.5f);
			p3->SetShader(pShader);
			BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p3);
			projectiles.push_back(p3);*/

			/*Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y - 0.5f, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
			p->SetImpulseForce(glm::vec3(0.0f, 150.0f, 120.0f));
			p->SetActingForce(glm::vec3(0.0f, -5.0f, 0.0f));
			p->SetKnockbackForce(glm::vec3(0.0f));
			p->SetFirstDelay(0.6f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(0.6f, 0.6f, 0.6f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
			p->SetShader(pShader);
			BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);
			projectiles.push_back(p);*/

			Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
			p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 100.0f));
			p->SetActingForce(glm::vec3(0.0f));
			p->SetKnockbackForce(glm::vec3(0.0f, 140.0f, 0.0f));
			p->SetFirstDelay(0.2f, glm::vec3(0.0f, -0.7f, 0.5f), glm::vec3(0.5f, 0.25f, 0.5f), glm::vec3(1.5f, 0.5f, 0.75f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
			p->SetClipping(PROJECTILE_CLIP::YES_PLAYER_PROJECTILE);
			projectiles.push_back(p);
			mediumComboPosition++;
			mediumComboTimer = 1.0f;
			
			movementCD.setNewDuration(1.5f);
			movementCD.startCD();

		}
		else if (mediumComboPosition == 1 && playerState != BLOCK && playerState != STUN) {
			Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y - 0.5f, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
			p->SetImpulseForce(glm::vec3(0.0f, 100.0f, 200.0f));
			p->SetActingForce(glm::vec3(0.0f, -3.0f, 0.0f));
			p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 30.0f));
			p->SetFirstDelay(0.5f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(0.6f, 0.6f, 0.6f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
			projectiles.push_back(p);
			mediumComboPosition = 0;
			MediumCD.startCD();
		}
	}

	return projectiles;
}
std::vector<Projectile*> IcePlayer::HeavyAttack()
{
	std::vector<Projectile*> projectiles;

	if (heavyComboPosition == 0 && playerState != BLOCK && playerState != STUN) {
		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + 0.1f, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 40.0f));
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 40.0f));
		p->SetFirstDelay(1.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetSecondDelay(1.0f, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetStunTime(1.0f);
		projectiles.push_back(p);
	}

	return projectiles;
}
std::vector<Projectile*> IcePlayer::SpecialAttack()
{
	std::vector<Projectile*> projectiles;

	return projectiles;
}
void IcePlayer::InheritedUpdate(const float deltaTime)
{

}
void IcePlayer::InheritedHandleEvents(SDL_Event events)
{

}
void IcePlayer::InheritedHandleStates(const Uint8 *state)
{

}