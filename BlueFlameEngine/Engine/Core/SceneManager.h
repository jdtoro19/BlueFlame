#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "../Camera/Camera.h"
#include "../Rendering/Renderer.h"
#include "Scene.h"
#include <map>

#include "../../Game/DefaultScene.h"
#include "../../Game/TestScene.h"

using namespace GAME;

namespace ENGINE {

	class SceneManager
	{
	public:
		SceneManager(Window* w);
		~SceneManager();

		void SwitchScene(int);
		void NextScene();
		void PreviousScene();

		void Update();
		void Render();
		void HandleEvents();
		bool IsQuit();

	private:
		Camera* camera;
		Renderer* renderer;
		glm::mat4 projection;
		SDL_Event events;	
		Window* window;

		Scene* currentScene;
		std::map <int, Scene*> scenes;

		bool quit = false;

		float lastX = 1080.0f / 2.0f;
		float lastY = 720.0f / 2.0f;
		bool firstMouse = true;
	};
}
#endif

