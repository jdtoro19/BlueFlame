#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "../Rendering/Renderer.h"
#include "DefaultScene.h"

namespace ENGINE {

	// This class handles game scenes, the renderer, events, and the window
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		void Initialize(Window* w);
		void Update(const float deltaTime);
		void Render();
		void Draw();		
		void HandleEvents();

		bool IsQuit();
		void SetIsQuit(bool quit);
		void SwitchScene(Scene* scene);
		void OnResize(int w, int h);
		void EnableSplitscreen(bool setSplitscreen);

		Renderer* GetRenderer();

	private:
		Window* window;
		Renderer* renderer;
		SDL_Event events;		
		Scene* currentScene;

		bool quit = false;
		bool splitscreen = false;
	};
}
#endif

