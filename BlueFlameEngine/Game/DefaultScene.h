#ifndef DEFAULTSCENE_H
#define DEFAULTSCENE_H

#include "../Engine/Core/Scene.h"

using namespace ENGINE;

namespace GAME {

	class DefaultScene : public Scene
	{
	public:
		DefaultScene();
		~DefaultScene();

		void Update();
		void Render();
		void HandleEvents(SDL_Event events);

	private:
	};
}
#endif

