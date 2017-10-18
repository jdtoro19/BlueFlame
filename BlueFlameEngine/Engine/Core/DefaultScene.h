#ifndef DEFAULTSCENE_H
#define DEFAULTSCENE_H

#include "Scene.h"

namespace ENGINE {

	class DefaultScene : public Scene
	{
	public:
		DefaultScene();
		~DefaultScene();

		void Update(const float deltaTime);
		void Render();
		void HandleEvents(SDL_Event events);

	};
}
#endif

