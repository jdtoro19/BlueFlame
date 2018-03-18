#include "EarthPlayer.h"

using namespace GAME;

EarthPlayer::EarthPlayer() {
	base = new PlayerBase("Resources/Models/Flint (Earth)/Models/Earth_0001.obj");
	base->SetWorldPosition(0.0f, 0.0f, 0.0f);
	base->SetWorldScale(0.012f);
	base->renderComponent->SetColour(0.2f, 0.7f, 0.0f);

	ring->renderComponent->SetColour(0.8, 0.5f, 0.3f);
	HeavyCD = Cooldown(5.0);
	LightCD = Cooldown(0.4);
	MediumCD = Cooldown(3.0);
	specialMeter = 0;
}

EarthPlayer::~EarthPlayer() {

}

std::vector<Projectile*> EarthPlayer::LightAttack()
{
	std::vector<Projectile*> proj;
	if (LightCD.checkOffCD()) {
		cout << "testing light" << endl;
		if (playerState != BLOCK && playerState != STUN) {

			if (specialActivated && specialMeter >= 25) {
				Projectile* p2 = smallRock(-0.5, -100.0);
				Projectile* p3 = smallRock(0.5, 100.0);
				proj.push_back(p2);
				proj.push_back(p3);
				specialMeter -= 25;
				specialActivated = false;
			}
			//else
			Projectile* p = smallRock(0, 0);
			proj.push_back(p);


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
	if (MediumCD.checkOffCD() || (specialActivated && specialMeter >= 60)) {
		cout << "FROM BELOW!" << endl;
		if (playerState != BLOCK && playerState != STUN && playerState != JUMP) {
			if (specialActivated && specialMeter >= 60) {
				Projectile* p0 = stalagmite(1.0f, 0.8f);
				Projectile* p2 = stalagmite(2.0f, 0.9f);
				Projectile* p3 = stalagmite(3.0f, 1.0f);
				Projectile* p4 = stalagmite(4.0f, 1.1f);
				Projectile* p5 = stalagmite(5.0f, 1.2f);

				proj.push_back(p0);
				proj.push_back(p2);
				proj.push_back(p3);
				proj.push_back(p4);
				proj.push_back(p5);
				specialMeter -= 60;
				specialActivated = false;
			}
			else {
				Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
				p->SetImpulseForce(glm::vec3(0.0f, -200.0f, 300.0f));
				p->SetActingForce(glm::vec3(0.0f, 10.0f, -7.5f));
				p->SetKnockbackForce(glm::vec3(0.0f, 80.0f, -10.0f));
				p->SetStunTime(1.0f);
				p->SetWorldScale(0.5f);
				p->SetLifetime(0.8);
				p->SetClipping(PROJECTILE_CLIP::YES_PLAYER);
				p->SetElement(PROJECTILE_ELEMENT::EARTH);
				p->SetStrength(PROJECTILE_STRENGTH::MEDIUM);
				proj.push_back(p);
				MediumCD.startCD();
			}



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
		if (playerState != BLOCK && playerState != STUN && playerState != JUMP) {


			Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), 0.0f, dir);
			p->SetActingForce(glm::vec3(0.0f, -1.5f, 3.0f));
			p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 0.0f));
			p->SetStunTime(0.0f);
			p->SetWorldScale(2.0f, 0.25f, 0.25f);
			p->SetElement(PROJECTILE_ELEMENT::EARTH);
			p->SetStrength(PROJECTILE_STRENGTH::HEAVY);
			p->SetClipping(PROJECTILE_CLIP::YES_WALL_PROJECTILE);
			p->SetLifetime(5.0f);

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
	cout << "TERRA ETERNAL" << endl;
	if (playerState != BLOCK && playerState != STUN && playerState != JUMP) {

		if (!specialActivated) { //set up for one of your special attacks
			specialActivated = true;
			cout << "special primed! " << specialMeter << endl;
		}
		else if (specialMeter >= 75) { //finisher
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

			specialActivated = false;
			specialMeter -= 75;
		}
	}
	return proj;
}
void EarthPlayer::InheritedUpdate(const float deltaTime)
{
	//specialMeter++;
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
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x + offset, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(xxx, 100.0f, 180.0f));
	p->SetActingForce(glm::vec3(0.0f, -5.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 5.0f, 40.0f));
	p->SetStunTime(0.5f);
	p->SetWorldScale(0.35f);
	p->SetElement(PROJECTILE_ELEMENT::EARTH);
	p->SetStrength(PROJECTILE_STRENGTH::LIGHT);
	return p;
}


Projectile* EarthPlayer::SpecialRock(float offset) {
	Projectile* p = new Projectile(glm::vec3(offset, GetWorldPosition().y - 1.5f, -1.5f * -dir), 0.0f, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 60.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetFirstDelay(1.0f, glm::vec3(0.0f, 1.0f, 0.5f), glm::vec3(1.95f, 1.45f, 0.45f), glm::vec3(2.0f, 1.5f, 0.5f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetStunTime(2.0f);
	p->SetElement(PROJECTILE_ELEMENT::EARTH);
	p->SetStrength(PROJECTILE_STRENGTH::SPECIAL);
	p->SetClipping(YES_PLAYER_PROJECTILE);
	p->SetLifetime(8.0f);

	return p;
}

Projectile* EarthPlayer::stalagmite(float offset, float delay) {
	glm::vec3 startPos = glm::vec3(GetWorldPosition().x + (offset * targetAngle), -0.6f, GetWorldPosition().z - (offset * dir) - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir);
	Projectile* p = new Projectile(startPos, targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetActingForce(glm::vec3(0.0f, 5.0f, 0.0f));
	p->SetFirstDelay(delay, glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 30.0f, 25.0f));
	p->SetStunTime(0.5f);
	p->SetWorldScale(0.35f);
	p->SetLifetime(delay + 0.5f);
	p->SetElement(PROJECTILE_ELEMENT::EARTH);
	p->SetStrength(PROJECTILE_STRENGTH::SPECIAL);
	p->SetClipping(PROJECTILE_CLIP::YES_PLAYER);
	std::cout << "here we go" << endl;
	return p;
}