#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <SDL/SDL.h>
#include <BlueFlameEngine\Engine\Core\GameObject.h>
#include <BlueFlameEngine\Engine\Core\RenderComponent.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Projectile.h>
#include "PlayerBase.h"

using namespace ENGINE;

namespace GAME {

	// Player class
	class Player : public GameObject
	{
	public:
		Player();
		~Player();

		// Enum for player movement
		enum PLAYERMOVEMENT {FORWARD, BACKWARD, RIGHT, LEFT};
		// Enum for player states
		enum PLAYERSTATES {NORMAL, ATTACK, BLOCK, STUN, JUMP, DODGE, DEAD};

		void Update(const float deltaTime);
		void HandleEvents(SDL_Event events);
		void Render(Shader* shader);

		// Player movemrnt 
		void Movement(PLAYERMOVEMENT movement, const float deltaTime);

		// Player Model Functions
		void UpdateModel(const float deltaTime);
		void ResetModel();

		void Stun();
		void Block();

	private:
		// Player model parts
		PlayerBase* base;
		PlayerBase* ring;
		PlayerBase* shield;
		bool down = false;

	public:
		// Player Stats
		int health = 0;
		int maxHealth = 100;
		float moveSpeed = 2.0f;
		// Player State
		PLAYERSTATES playerState = NORMAL;
		float stunTimer = 0;
		float stunTimerSet = 1.0f;
	};

}

#endif

