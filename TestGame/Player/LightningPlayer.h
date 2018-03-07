#pragma once
#pragma once
#ifndef LIGHTNINGPLAYER_H
#define LIGHTNINGPLAYER_H

#include "Player.h"
#include <BlueFlameEngine\Engine\Timers\Cooldown.h>

using namespace ENGINE;

namespace GAME {

	class LightningPlayer : public Player
	{
	public:
		LightningPlayer();
		~LightningPlayer();

		// Attack Functions
		std::vector<Projectile*> LightAttack();
		std::vector<Projectile*> MediumAttack();
		std::vector<Projectile*> HeavyAttack();
		std::vector<Projectile*> SpecialAttack();

		void InheritedUpdate(const float deltaTime);
		void InheritedHandleEvents(SDL_Event events);
		void InheritedHandleStates(const Uint8 *state);

	private:
		//new elements
		Cooldown HeavyCD;
		Cooldown LightCD;
		Cooldown MediumCD;
		Cooldown SpecialDuration;
		Cooldown SpecialCharge;
		Projectile* LightningCloud(float offset); //creates cloud to fire heavy attack
		Projectile* Strike(float offsetx, float offsety, float offsetz); //lightning strikes
		Projectile* ZeroLaser(); //for special attack
		bool ZeroLasering = false;
		bool charging = false;
		Projectile* ForkedLightning(float offset); //used to create heavy attack
		Projectile* Shocker(float offsetx, float offsetz, float speed, float accel, float stun, float knockback); //used for light attack
	};

}

#endif
