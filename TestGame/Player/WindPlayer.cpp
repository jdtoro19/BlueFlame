#include "WindPlayer.h"

using namespace GAME;

WindPlayer::WindPlayer() {
	base->renderComponent->SetColour(0.1f, 0.1f, 0.5f);
}

WindPlayer::~WindPlayer() {

}

Projectile* WindPlayer::LightAttack()
{
	cout << "Light Attack" << endl;
	if (lightComboTimer <= 0 && lightComboPosition == 0 && playerState != BLOCK && playerState != STUN) {
		
		lightComboTimer = 0.3f;
		lightComboPosition++;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z), glm::vec3(0.0f, 0.0f, 500.0f), 1);
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 25.0f, 25.0f));
		p->SetStunTime(0.3f);
		p->SetWorldScale(0.5f);
		p->SetShader(pShader);
		BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);

		return p;
	}
	if (lightComboTimer > 0 && lightComboPosition == 1 && playerState != BLOCK && playerState != STUN) {

		lightComboTimer = 0.3f;
		lightComboPosition++;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z), glm::vec3(300.0f, 0.0f, 600.0f), 1);
		p->SetActingForce(glm::vec3(-50.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 100.0f, 25.0f));
		p->SetStunTime(0.3f);
		p->SetWorldScale(0.5f);
		p->SetShader(pShader);
		BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);

		return p;
	}
	if (lightComboTimer > 0 && lightComboPosition == 2 && playerState != BLOCK && playerState != STUN) {

		lightComboTimer = 0.3f;
		lightComboPosition++;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z), glm::vec3(-300.0f, 500.0f, 600.0f), 1);
		p->SetActingForce(glm::vec3(50.0f, -70.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, -500.0f, 500.0f));
		p->SetStunTime(0.5f);
		p->SetWorldScale(0.5f);
		p->SetShader(pShader);
		BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);

		return p;
	}
	else 
	{
		return NULL;
	}
}
Projectile* WindPlayer::MediumAttack()
{
	return NULL;
}
Projectile* WindPlayer::HeavyAttack()
{
	return NULL;
}
Projectile* WindPlayer::SpecialAttack()
{
	return NULL;
}
void WindPlayer::InheritedUpdate(const float deltaTime)
{

}
void WindPlayer::InheritedHandleEvents(SDL_Event events)
{

}
void WindPlayer::InheritedHandleStates(const Uint8 *state)
{

}