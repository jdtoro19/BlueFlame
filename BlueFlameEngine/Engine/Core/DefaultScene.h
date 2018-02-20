#pragma once
#ifndef DEFAULTSCENE_H
#define DEFAULTSCENE_H

#include "Scene.h"
#include "../Rendering/2D/TextUI.h"

namespace ENGINE {

	class DefaultScene : public Scene
	{
	public:
		DefaultScene();
		~DefaultScene();

		bool Initialize();
		void Update(const float deltaTime);
		void HandleEvents(SDL_Event events);

	private:
		TextUI* text;
	};
}
#endif

