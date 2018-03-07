#include "IcePlayer.h"

using namespace GAME;

IcePlayer::IcePlayer() {
	base->renderComponent->SetColour(0.1f, 0.1f, 0.5f);
}

IcePlayer::~IcePlayer() {

}

std::vector<Projectile*> IcePlayer::LightAttack()
{
	// PUSH BACK INTO THIS 
	std::vector<Projectile*> projectiles;

	cout << "Light Attack" << endl;
	if (lightComboPosition == 0 && playerState != BLOCK && playerState != STUN) {

		lightComboPosition = 1;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x - 0.25f, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z), glm::vec3(0.0f, 0.0f, 100.0f), targetAngle, 1, 0.05f);
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 20.0f));
		p->SetScaleChange(glm::vec3(0.05f, 0.05f, 0.25f), glm::vec3(0.2f, 0.2f, 0.75f));
		p->SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), -targetAngle);
		p->SetStunTime(0.2f);
		p->SetShader(pShader);
		BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);
		projectiles.push_back(p);
		//return projectiles;
	} else if (lightComboPosition == 1 && playerState != BLOCK && playerState != STUN) {

		lightComboPosition = 0;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x + 0.25f, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z), glm::vec3(0.0f, 0.0f, 100.0f), targetAngle, 1, 0.05f);
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 20.0f));
		p->SetScaleChange(glm::vec3(0.05f, 0.05f, 0.25f), glm::vec3(0.2f, 0.2f, 0.75f));
		p->SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), -targetAngle);
		p->SetStunTime(0.2f);
		p->SetShader(pShader);
		BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);
		projectiles.push_back(p);
		//return projectiles;
	}
	
	return projectiles;
}
std::vector<Projectile*> IcePlayer::MediumAttack()
{
	std::vector<Projectile*> projectiles;

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + 6.0f, GetWorldPosition().z + 5.0f * -1), glm::vec3(0.0f, -2.0f, 0.0f), targetAngle, 1, 0.75f);
	p->SetActingForce(glm::vec3(0.0f, -4.0f, 0.0f));
	p->SetWorldScale(0.25f, 1.0f, 0.25f);
	p->SetKnockbackForce(glm::vec3(0.0f));
	p->SetStunTime(2.5f);
	p->SetShader(pShader);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);
	projectiles.push_back(p);
	Projectile* p1 = new Projectile(glm::vec3(GetWorldPosition().x - 1.0f, GetWorldPosition().y + 6.0f, GetWorldPosition().z + 6.0f * -1), glm::vec3(0.0f, -2.0f, 0.0f), targetAngle, 1, 0.75f);
	p1->SetActingForce(glm::vec3(0.0f, -4.0f, 0.0f));
	p1->SetWorldScale(0.25f, 1.0f, 0.25f);
	p1->SetKnockbackForce(glm::vec3(0.0f));
	p1->SetStunTime(2.5f);
	p1->SetShader(pShader);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p1);
	projectiles.push_back(p1);
	Projectile* p2 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + 6.0f, GetWorldPosition().z + 7.0f * -1), glm::vec3(0.0f, -2.0f, 0.0f), targetAngle, 1, 0.75f);
	p2->SetActingForce(glm::vec3(0.0f, -4.0f, 0.0f));
	p2->SetWorldScale(0.25f, 1.0f, 0.25f);
	p2->SetKnockbackForce(glm::vec3(0.0f));
	p2->SetStunTime(2.5f);
	p2->SetShader(pShader);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p2);
	projectiles.push_back(p2);
	Projectile* p3 = new Projectile(glm::vec3(GetWorldPosition().x + 1.0f, GetWorldPosition().y + 6.0f, GetWorldPosition().z + 6.0f * -1), glm::vec3(0.0f, -2.0f, 0.0f), targetAngle, 1, 0.75f);
	p3->SetActingForce(glm::vec3(0.0f, -4.0f, 0.0f));
	p3->SetWorldScale(0.25f, 1.0f, 0.25f);
	p3->SetKnockbackForce(glm::vec3(0.0f));
	p3->SetStunTime(2.5f);
	p3->SetShader(pShader);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p3);
	projectiles.push_back(p3);

	return projectiles;
}
std::vector<Projectile*> IcePlayer::HeavyAttack()
{
	std::vector<Projectile*> projectiles;

	if (lightComboPosition == 0 && playerState != BLOCK && playerState != STUN) {
		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z), glm::vec3(0.0f, 0.0f, 40.0f), targetAngle, 1, 1.0f);
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 40.0f));
		p->SetScaleChange(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(1.0f, 1.0f, 1.0f));
		p->SetStunTime(1.0f);
		p->SetShader(pShader);
		BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);
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