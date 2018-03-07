#include "FirePlayer.h"

using namespace GAME;

FirePlayer::FirePlayer()
{
	base->renderComponent->SetColour(0.7f, 0.2f, 0.1f);
	//extraP = false;
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
		lightComboPosition++;
		lightComboTimer = 0.85f;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z),
			glm::vec3(0.0f, 0.0f, 350.0f), targetAngle, dir);
		
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 10.0f, 10.0f));
		p->SetStunTime(0.4f);
		p->SetWorldScale(0.3f);
		p->SetShader(pShader);
		BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);
		return projectiles;
	}

	if (playerState == ATTACK && lightComboPosition == 1 && lightComboTimer > 0)
	{
		lightComboPosition++;
		lightComboTimer = 0.25f;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x - collisionComponent->GetBoundingBox().r.x / 1.5f * GetWorldScale().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z),
			glm::vec3(0.0f, 0.0f, 250.0f), targetAngle, dir);

		p->SetActingForce(glm::vec3(10.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 10.0f));
		p->SetStunTime(0.4f);
		p->SetWorldScale(0.3f);
		p->SetShader(pShader);
		BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);

		//extraP = true; //extra P so that one key gives another projectile
		return projectiles;
	}
	/*
	else if (extraP && lightComboPosition == 2)
	{
		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x + collisionComponent->GetBoundingBox().r.x / 1.5f * GetWorldScale().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z),
			glm::vec3(0.0f, 0.0f, 450.0f), 1);

		p->SetActingForce(glm::vec3(-10.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 10.0f));
		p->SetStunTime(0.4f);
		p->SetWorldScale(0.3f);
		p->SetShader(pShader);
		BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);
		extraP = false;
		return p;
	}
	*/

	if (playerState == ATTACK && lightComboPosition == 2 && lightComboTimer > 0)
	{
		lightComboPosition++;
		lightComboTimer = 0.25f;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z),
			glm::vec3(0.0f, 0.0f, 250.0f), targetAngle, dir);

		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 100.0f, 10.0f));
		p->SetStunTime(0.4f);
		p->SetWorldScale(0.3f);
		p->SetShader(pShader);
		BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);

		//extraP = true; //extra P so that one key gives another projectile
		return projectiles;
	}
	/*
	else if (extraP && lightComboPosition == 3)
	{
		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + collisionComponent->GetBoundingBox().r.y / 1.5f * GetWorldScale().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z),
			glm::vec3(0.0f, 500.0f, 450.0f), 1);

		p->SetActingForce(glm::vec3(0.0f, -70.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, -400.0f, 10.0f));
		p->SetStunTime(0.4f);
		p->SetWorldScale(0.3f);
		p->SetShader(pShader);
		BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);
		extraP = false;
		return p;
	}
	*/

	return projectiles;
}

std::vector<Projectile*> FirePlayer::MediumAttack()
{
	std::vector<Projectile*> projectiles;

	return projectiles;
}

std::vector<Projectile*> FirePlayer::HeavyAttack()
{
	std::vector<Projectile*> projectiles;

	return projectiles;
}

std::vector<Projectile*> FirePlayer::SpecialAttack()
{
	std::vector<Projectile*> projectiles;

	return projectiles;
}

std::vector<Projectile*> FirePlayer::ExtraProjectiles()
{
	std::vector<Projectile*> projectiles;

	return projectiles;
}

void FirePlayer::InheritedUpdate(const float deltaTime)
{
	
}

void FirePlayer::InheritedHandleEvents(SDL_Event events)
{

}

void FirePlayer::InheritedHandleStates(const Uint8 *state)
{

}