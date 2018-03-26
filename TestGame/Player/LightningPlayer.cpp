#include "LightningPlayer.h"

using namespace GAME;

LightningPlayer::LightningPlayer() {
	base = new PlayerBase("Resources/Models/Alex (Electric)/Models/Electric_0001.obj");
	base->SetWorldPosition(0.0f, 0.0f, 0.0f);
	base->SetWorldScale(0.012f);
	base->renderComponent->SetColour(0.2f, 0.7f, 0.0f);

	ring->renderComponent->SetColour(1.0f, 1.0f, 0.0f);
	HeavyCD = Cooldown(4.0);
	LightCD = Cooldown(0.4);
	MediumCD = Cooldown(2.5);
	SpecialDuration = Cooldown(3.0);
	specialMeter = 0;

	shootEffect = new ParticleSystem(BFEngine::GetInstance()->GetSceneManager()->GetRenderer()->GetShaderManager(), glm::vec3(1.0f, 1.0f, 0.0f));
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(shootEffect);

	dialogue = PlayerDialogue();
	dialogue.LoadPlayerDialogue("Resources/Audio/OkiCaeliAudio.txt");
}

LightningPlayer::~LightningPlayer() {

}

std::vector<Projectile*> LightningPlayer::LightAttack()
{
	std::vector<Projectile*> proj;
	if (playerState != BLOCK && playerState != STUN) {

		if (specialActivated && specialMeter >= 50) {

			for (int i = 0; i < 3; i++) {
				Projectile* p = ZeroLaser(0.05f * i, 100.0f, 2.0f);
				proj.push_back(p);
			}
			specialActivated = false;
			specialMeter -= 50;
		}
		else if (LightCD.checkOffCD()) {

			/*Projectile* f = Shocker(0, 1.0f, 400.0f, 0.0f, 0.5f, 50.0f);
			proj.push_back(f);

			Projectile* o = Shocker(0.5f, 0.5f, 200.0f, 5.0f, 0.5f, 50.0f);
			proj.push_back(o);

			Projectile* r = Shocker(-0.5f, 0, 0.0f, 20.0f, 0.5f, 50.0f);
			proj.push_back(r);*/

			Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - (collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z)), targetAngle, dir);
			p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 0.0f));
			p->SetActingForce(glm::vec3(0.0f, 0.0f, 15.0f)); //no accel
			p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 20.0f)); //static
			p->SetStunTime(0.75f);
			p->SetWorldScale(0.25f, 0.25f, 0.5f);
			p->SetLifetime(3.0f);


			proj.push_back(p);
			LightCD.startCD();
		}
		else {
			cout << "Ability on CD for " << std::to_string(HeavyCD.secondsLeft()) << endl;
		}
	}

	return proj;
}
std::vector<Projectile*> LightningPlayer::MediumAttack() //lightning strike. DOWN B!
{
	std::vector<Projectile*> proj;
	if (playerState != BLOCK && playerState != STUN) {
		if (specialActivated && specialMeter >= 50) {
			Projectile* p = LightningCloud(10.0f, 1.0f);
			proj.push_back(p);

			Projectile* f = Shocker(0, 10.0f, -400.0f, 0.0f, 0.5f, -50.0f);
			proj.push_back(f);

			specialActivated = false;
			specialMeter -= 50;
		}
		else if (MediumCD.checkOffCD()) {
			cout << "Lightning Strike" << endl;


			Projectile* p = LightningCloud(5.0f, 6.0f);
			proj.push_back(p);

			Projectile* a = Strike(0, 0, 5.0f, 5.0f);
			proj.push_back(a);

			MediumCD.startCD();
			p->SetElement(PROJECTILE_ELEMENT::LIGHTNING);
			p->SetStrength(PROJECTILE_STRENGTH::LIGHT);
			p->SetClipping(PROJECTILE_CLIP::YES_WALL_PLAYER);
		}
		else {
			cout << "Ability on CD for " << std::to_string(HeavyCD.secondsLeft()) << endl;
		}
	}

	return proj;
}
std::vector<Projectile*> LightningPlayer::HeavyAttack()
{
	std::vector<Projectile*> proj;
	if (HeavyCD.checkOffCD()) {
		cout << "Forked Lightning" << endl;
		if (playerState != BLOCK && playerState != STUN) {

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
	if (!specialActivated && !charging) { //set up for one of your special attacks
		specialActivated = true;
		cout << "special primed! " << specialMeter << endl;
	}
	else if (playerState != BLOCK && playerState != STUN) {
		if (specialActivated && specialMeter >= 100) {
			specialActivated = false;
			specialMeter -= 100;
			SpecialDuration.startCD();
			charging = true;
			for (int i = 0; i < 50; i++) {
				Projectile* p = ZeroLaser(1.0f + 0.05f * i, 400.0f, 1.0f);
				proj.push_back(p);
			}
		}

	}
	return proj;
}
void LightningPlayer::InheritedUpdate(const float deltaTime)
{
	specialMeter++;
	if (SpecialDuration.checkOffCD() && charging) {
		charging = false;
	}
	else if (charging) {
		Stun();
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

Projectile* LightningPlayer::LightningCloud(float offset, float height) {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x + (offset * targetAngle * dir), GetWorldPosition().y + height, GetWorldPosition().z - (collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir) - offset * dir), targetAngle, dir);
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f)); //static
	p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 0.0f)); //static
	p->SetStunTime(0.0f);
	p->SetWorldScale(1.0f, 0.5f, 1.0f);
	p->SetLifetime(0.4f);
	p->SetElement(PROJECTILE_ELEMENT::LIGHTNING);
	p->SetStrength(PROJECTILE_STRENGTH::LIGHT);
	p->SetClipping(PROJECTILE_CLIP::YES_WALL_PLAYER);

	return p;
}

Projectile* LightningPlayer::Strike(float offsetx, float offsety, float offsetz, float height) {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x + (offsetz * targetAngle * dir), GetWorldPosition().y + height, GetWorldPosition().z - (collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir) - offsetz * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, -100.0f, 0.0f));
	p->SetActingForce(glm::vec3(0.0f, -10.0f, 0.0f)); //no accel
	p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 50.0f)); //knocks them back. finisher
	p->SetStunTime(0.75f);
	p->SetWorldScale(0.2f, 1.0f, 0.2f);
	p->SetLifetime(0.5f);
	p->SetElement(PROJECTILE_ELEMENT::LIGHTNING);
	p->SetStrength(PROJECTILE_STRENGTH::MEDIUM);
	p->SetClipping(PROJECTILE_CLIP::YES_WALL_PLAYER);
	return p;
}

Projectile* LightningPlayer::ZeroLaser(float delay, float speed, float baseLifetime) {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - (collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z)), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, speed));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f)); //no accel
	p->SetFirstDelay(delay, glm::vec3(0), glm::vec3(1.0f, 0.1f, 0.5f), glm::vec3(1.0f, 0.1f, 0.5f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 00.0f, 40.0f)); //static
	p->SetStunTime(0.2f);
	p->SetLifetime(delay + baseLifetime);
	p->SetElement(PROJECTILE_ELEMENT::LIGHTNING);
	p->SetStrength(PROJECTILE_STRENGTH::SPECIAL);
	p->SetClipping(PROJECTILE_CLIP::YES_PLAYER);
	return p;
}

Projectile* LightningPlayer::ForkedLightning(float offset) {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - (collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z)), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(offset, 0.0f, 400.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f)); //no accel
	p->SetKnockbackForce(glm::vec3(0.0f, 10.0f, 30.0f)); //static
	p->SetStunTime(0.2f);
	p->SetWorldScale(0.2f, 0.2f, 0.2f);
	p->SetLifetime(0.15f);
	p->SetElement(PROJECTILE_ELEMENT::LIGHTNING);
	p->SetStrength(PROJECTILE_STRENGTH::HEAVY);
	p->SetClipping(PROJECTILE_CLIP::YES);
	return p;
}

Projectile* LightningPlayer::Shocker(float offsetx, float offsetz, float speed, float accel, float stun, float knockback) {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x + offsetx + (offsetz * targetAngle * dir), GetWorldPosition().y, GetWorldPosition().z - (collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z) - offsetz), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, speed));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, accel)); //no accel
	p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, knockback)); //static
	p->SetStunTime(stun);
	p->SetWorldScale(0.25f, 0.25f, 0.5f);
	p->SetLifetime(3.0f);
	p->SetElement(PROJECTILE_ELEMENT::LIGHTNING);
	p->SetStrength(PROJECTILE_STRENGTH::LIGHT);
	p->SetClipping(PROJECTILE_CLIP::YES);
	return p;
}