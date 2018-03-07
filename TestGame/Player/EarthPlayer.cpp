#include "EarthPlayer.h"

using namespace GAME;

EarthPlayer::EarthPlayer() {
	base->renderComponent->SetColour(0.1f, 0.1f, 0.5f);
	HeavyCD = Cooldown(5.0);
	LightCD = Cooldown(0.4);
	MediumCD = Cooldown(3.0);
}

EarthPlayer::~EarthPlayer() {

}

std::vector<Projectile*> EarthPlayer::LightAttack()
{
	std::vector<Projectile*> proj;
	if (LightCD.checkOffCD()) {
		cout << "testing light" << endl;
		if (playerState != BLOCK && playerState != STUN) {

			Projectile* p = smallRock(0, 0);
			Projectile* p2 = smallRock(-0.5, -100.0);
			Projectile* p3 = smallRock(0.5, 100.0);

			proj.push_back(p);
			proj.push_back(p2);
			proj.push_back(p3);

			LightCD.startCD();
		}
	}
	else {
		cout << "Ability on CD for " << std::to_string(HeavyCD.secondsLeft()) << endl;
	}
	return proj;
}
std::vector<Projectile*> EarthPlayer::MediumAttack()
{
	std::vector<Projectile*> proj;
	if (MediumCD.checkOffCD()) {
		cout << "FROM BELOW!" << endl;
		if (playerState != BLOCK && playerState != STUN) {

			Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z), glm::vec3(0.0f, -200.0f, 300.0f), targetAngle, dir);
			p->SetActingForce(glm::vec3(0.0f, 10.0f, -7.5f));
			p->SetKnockbackForce(glm::vec3(0.0f, 100.0f, 0.0f));
			p->SetStunTime(1.0f);
			p->SetWorldScale(0.5f);
			p->SetShader(pShader);
			p->setLifetime(0.8);
			BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);

			MediumCD.startCD();

			proj.push_back(p);
		}
	}
	else {
		cout << "Ability on CD for " << std::to_string(HeavyCD.secondsLeft()) << endl;
	}
	return proj;
}
std::vector<Projectile*> EarthPlayer::HeavyAttack()
{
	std::vector<Projectile*> proj;
	if (HeavyCD.checkOffCD()) {
		cout << "Build a wall!" << endl;
		if (playerState != BLOCK && playerState != STUN) {

			Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y - 1.0f, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z), glm::vec3(0.0f, 100.0f, 0.0f), targetAngle, dir);
			p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
			p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 0.0f));
			p->SetStunTime(0.0f);
			p->SetWorldScale(2,2,0.25);
			p->SetShader(pShader);
			p->addMaxDistance(2.0f);
			p->setLifetime(5.0);
			BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);

			HeavyCD.startCD();

			proj.push_back(p);
		}
	}
	else {
		cout << "Ability on CD for " << std::to_string(HeavyCD.secondsLeft()) << endl;
	}
	return proj;
}
std::vector<Projectile*> EarthPlayer::SpecialAttack()
{
	std::vector<Projectile*> proj;
	if (true) { //will be a special meter check
		cout << "TERRA ETERNAL" << endl;
		if (playerState != BLOCK && playerState != STUN) {

			Projectile* e = SpecialRock(0.0f);

			proj.push_back(e);

			Projectile* a = SpecialRock(2.0f);

			proj.push_back(a);

			Projectile* r = SpecialRock(4.0f);

			proj.push_back(r);

			Projectile* t = SpecialRock(-2.0f);

			proj.push_back(t);

			Projectile* h = SpecialRock(-4.0f);

			proj.push_back(h);
		}
	}
	else {
		cout << "Ability on CD for " << std::to_string(HeavyCD.secondsLeft()) << endl;
	}
	return proj;
}
void EarthPlayer::InheritedUpdate(const float deltaTime)
{

}
void EarthPlayer::InheritedHandleEvents(SDL_Event events)
{

}
void EarthPlayer::InheritedHandleStates(const Uint8 *state)
{
	if (state[SDL_SCANCODE_Y]) {
		HeavyAttack();
	}
}

Projectile* EarthPlayer::smallRock(float offset, float xxx) {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x + offset, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z), glm::vec3(xxx, 100.0f, 180.0f), targetAngle, dir);
	p->SetActingForce(glm::vec3(0.0f, -5.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 5.0f, 300.0f));
	p->SetStunTime(0.5f);
	p->SetWorldScale(0.35f);
	p->SetShader(pShader);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);

	return p;
}


Projectile* EarthPlayer::SpecialRock(float offset) {
	Projectile* p = new Projectile(glm::vec3(offset, GetWorldPosition().y - 0.5f, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z), glm::vec3(0.0f, 0.0f, 20.0f), targetAngle, dir);
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetStunTime(2.0f);
	p->SetWorldScale(2.0f, 1.0f, 0.5f);
	p->SetShader(pShader);
	p->setLifetime(8.0f);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(p);

	return p;
}