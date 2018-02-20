#pragma once
#ifndef PLAYERBASE_H
#define PLAYERBASE_H

#include <BlueFlameEngine\Engine\Core\GameObject.h>
#include <BlueFlameEngine\Engine\Core\RenderComponent.h>

using namespace ENGINE;

namespace GAME {

	class PlayerBase : public GameObject
	{
	public:
		PlayerBase();
		~PlayerBase();

		void Update(const float deltaTime);
		void Render(Shader* shader);

		RenderComponent* renderComponent;
	};
}

#endif

