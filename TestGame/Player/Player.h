#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <BlueFlameEngine\Engine\Core\GameObject.h>
#include <BlueFlameEngine\Engine\Rendering\3D\ModelComponent.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Projectile.h>

using namespace ENGINE;

namespace GAME {

	class Player : public GameObject
	{
	public:
		Player();
		~Player();

		void Update(const float deltaTime);
		void Render(Shader* shader);
	};

}

#endif

