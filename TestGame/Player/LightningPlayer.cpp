#include "LightningPlayer.h"

using namespace GAME;

LightningPlayer::LightningPlayer() {
	ring->renderComponent->SetColour(0.1f, 0.1f, 0.5f);
	HeavyCD = Cooldown(4.0);
	LightCD = Cooldown(0.4);
	MediumCD = Cooldown(2.5);
	SpecialDuration = Cooldown(2.0);
	SpecialCharge = Cooldown(1.0);
}

LightningPlayer::~LightningPlayer() {

}

std::vector<Projectile*> LightningPlayer::LightAttack()
{
	std::vector<Projectile*> proj;
	if (LightCD.checkOffCD()) {
		cout << "shocker" << endl;
		if (playerState != BLOCK && playerState != STUN && !charging) {
			/*
			Projectile* f = Shocker(0, 1.0f, 400.0f, 0.0f, 0.5f, 50.0f);
			proj.push_back(f);

			Projectile* o = Shocker(0.5f, 0.5f, 200.0f, 5.0f, 0.5f, 50.0f);
			proj.push_back(o);

			Projectile* r = Shocker(-0.5f, 0, 0.0f, 20.0f, 0.5f, 50.0f);
			proj.push_back(r);*/

			Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - (collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z)), targetAngle, dir);
			p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 0.0f));
			p->SetActingForce(glm::vec3(0.0f, 0.0f, 15.0f)); //no accel
			p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 400.0f)); //static
			p->SetStunTime(0.75f);
			p->SetWorldScale(0.25f, 0.25f, 0.5f);
			p->SetLifetime(3.0f);

			proj.push_back(p);
			LightCD.startCD();
		}
	}
	else {
		cout << "Ability on CD for " << std::to_string(HeavyCD.secondsLeft()) << endl;
	}
	return proj;
}
std::vector<Projectile*> LightningPlayer::MediumAttack() //lightning strike. DOWN B!
{
	std::vector<Projectile*> proj;
	if (MediumCD.checkOffCD()) {
		cout << "Lightning Strike" << endl;
		if (playerState != BLOCK && playerState != STUN && !charging) {

			Projectile* p = LightningCloud(-4.0f);
			proj.push_back(p);

			Projectile* a = Strike(0, 0, -4.0f);
			proj.push_back(a);

			MediumCD.startCD();

		}
	}
	else {
		cout << "Ability on CD for " << std::to_string(HeavyCD.secondsLeft()) << endl;
	}
	return proj;
}
std::vector<Projectile*> LightningPlayer::HeavyAttack() 
{
	std::vector<Projectile*> proj;
	if (HeavyCD.checkOffCD()) {
		cout << "Forked Lightning" << endl;
		if (playerState != BLOCK && playerState != STUN && !charging) {

			Projectile* f = ForkedLightning(0);
			proj.push_back(f);
			
			Projectile* o = ForkedLightning(-80.0);
			proj.push_back(o);

			Projectile* r = ForkedLightning(80.0);
			proj.push_back(r);

			Projectile* k = ForkedLightning(-160.0);
			proj.push_back(k);

			Projectile* d = ForkedLightning(160.0);
			proj.push_back(d);

			HeavyCD.startCD();
		}
	}
	else {
		cout << "Ability on CD for " << std::to_string(HeavyCD.secondsLeft()) << endl;
	}
	return proj;
}
std::vector<Projectile*> LightningPlayer::SpecialAttack() //Zero Laser!
{
	std::vector<Projectile*> proj;
	if (playerState != BLOCK && playerState != STUN && !charging) {
		SpecialCharge.startCD();
		charging = true;
	}
	return proj;
}
void LightningPlayer::InheritedUpdate(const float deltaTime)
{
	if (SpecialCharge.checkOffCD() && charging) {
		charging = false;
		ZeroLasering = true;
		SpecialDuration.startCD();
	}
	if (ZeroLasering) {
		ZeroLasering = !SpecialDuration.checkOffCD(); //if the duration is still running, keep lasering
		Stun();
		Projectile* p = ZeroLaser();
	}
}
void LightningPlayer::InheritedHandleEvents(SDL_Event events)
{

}
void LightningPlayer::InheritedHandleStates(const Uint8 *state)
{
	if (state[SDL_SCANCODE_Y]) {
		HeavyAttack();
	}
}

Projectile* LightningPlayer::LightningCloud(float offset) {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + 6.0f, GetWorldPosition().z - ( collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z ) + offset), targetAngle, dir);
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f)); //static
	p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 0.0f)); //static
	p->SetStunTime(0.0f);
	p->SetWorldScale(1.0f, 0.5f, 1.0f);
	p->SetLifetime(0.8f);

	return p;
}

Projectile* LightningPlayer::Strike(float offsetx, float offsety, float offsetz) {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + 5.0f, GetWorldPosition().z - (collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z) + offsetz), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, -100.0f, 0.0f));
	p->SetActingForce(glm::vec3(0.0f, -10.0f, 0.0f)); //no accel
	p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, -400.0f)); //knocks them forward for forked lightning
	p->SetStunTime(0.75f);
	p->SetWorldScale(0.2f, 1.0f, 0.2f);
	p->SetLifetime(0.5f);

	return p;
}

Projectile* LightningPlayer::ZeroLaser() {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - (collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z)), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 400.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f)); //no accel
	p->SetKnockbackForce(glm::vec3(0.0f, 00.0f, 50.0f)); //static
	p->SetStunTime(0.2f);
	p->SetWorldScale(1.0f, 0.1f, 0.5f);
	p->SetLifetime(1.0f);

	return p;
}

Projectile* LightningPlayer::ForkedLightning(float offset) {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - (collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z)), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(offset, 0.0f, 400.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f)); //no accel
	p->SetKnockbackForce(glm::vec3(0.0f, 20.0f, 50.0f)); //static
	p->SetStunTime(0.2f);
	p->SetWorldScale(0.2f, 0.2f, 0.2f);
	p->SetLifetime(0.15f);

	return p;
}

Projectile* LightningPlayer::Shocker(float offsetx, float offsetz, float speed, float accel, float stun, float knockback) {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x + offsetx, GetWorldPosition().y, GetWorldPosition().z - (collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z) - offsetz), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, speed));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, accel)); //no accel
	p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, knockback)); //static
	p->SetStunTime(stun);
	p->SetWorldScale(0.25f, 0.25f, 0.5f);
	p->SetLifetime(3.0f);

	return p;
}