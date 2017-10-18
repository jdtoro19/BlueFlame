#ifndef TESTSCENE_H
#define TESTSCENE_H

#include <BlueFlameEngine\Engine\BFEngine.h>

#include <BlueFlameEngine\Engine\Core\Scene.h>

#include <BlueFlameEngine\Engine\Core\Light.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Cube.h> 
#include <BlueFlameEngine\Engine\Rendering\3D\Model.h> 

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

