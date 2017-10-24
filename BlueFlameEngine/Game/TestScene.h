#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "../Engine/Core/Scene.h"

#include "../Engine/Core/Light.h" 
#include "../Engine/Rendering/3D/Cube.h" 
#include "../Engine/Rendering/3D/Model.h"
#include "../Engine/Math/Physics.h"

using namespace ENGINE;

namespace GAME {

	class TestScene : public Scene
	{
	public:
		TestScene();
		~TestScene();

		void Update(const float deltaTime);
		void Render();
		void HandleEvents(SDL_Event events);

	private:
		Cube* cube;
		Cube* cube2;

		Light* dirLight;

		Model* model;

		float moveSpeed = 1;

		float deltaTime;
	};
}
#endif

