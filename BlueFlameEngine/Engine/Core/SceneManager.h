#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "../Camera/Camera.h"
#include "../Rendering/Renderer.h"
#include "Scene.h"
#include <map>

namespace ENGINE {

	class SceneManager
	{
	public:
		SceneManager(Window* w);
		~SceneManager();

		void SwitchScene(Scene* scene);

		void OnResize(int w, int h);
		
		Renderer* GetRenderer();

		void Update(const float deltaTime);
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

		bool quit = false;

		float lastX = 1080.0f / 2.0f;
		float lastY = 720.0f / 2.0f;
		bool firstMouse = true;
	};
}
#endif

