#pragma once 
#ifndef WINDPLAYER_H
#define WINDPLAYER_H

#include "Player.h"

using namespace ENGINE;

namespace GAME {

	class WindPlayer : public Player 
	{
	public:
		WindPlayer();
		~WindPlayer();

		// Attack Functions
		Projectile* LightAttack();
		Projectile* MediumAttack();
		Projectile* HeavyAttack();
		Projectile* SpecialAttack();

		void InheritedUpdate(const float deltaTime);
		void InheritedHandleEvents(SDL_Event events);
		void InheritedHandleStates(const Uint8 *state);
	};

}

#endif
