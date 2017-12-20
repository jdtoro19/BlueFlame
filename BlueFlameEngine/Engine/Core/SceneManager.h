#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "../Rendering/Renderer.h"
#include "../Rendering/UIRenderer.h"
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
		void PreRender();
		void Render();
		void PostRender();
		void Draw();		
		void HandleEvents();

		bool IsQuit();
		void SetIsQuit(bool quit);
		void SwitchScene(Scene* scene);
		void AddScene(Scene* scene);
		void NextScene();
		void PreviousScene();
		void ClearSceneList();
		void StartScene();
		void OnResize(int w, int h);
		void EnableSplitscreen(bool setSplitscreen);
		void EnableFullscreen(bool setFullScreen);
		void EnablePostProcess(bool setPostProcess);
		void ShowFPS(bool setShowFPS);
		void CaptureMouse(bool capture);
		void DebugText(std::string string, GLfloat x, GLfloat y);

		Renderer* GetRenderer();
		int GetWindowWidth();
		int GetWindowHeight();
		// Used for UI elements
		float GetScreenWidth();
		float GetScreenHeight();

	private:
		Window* window;
		UIRenderer* uiRenderer;
		Renderer* renderer;
		SDL_Event events;		
		Scene* currentScene;
		std::vector<Scene*> sceneList;
		int sceneIter = 0;

		bool quit = false;
		bool splitscreen = false;
		bool showFPS = false;
		bool postprocess = false;

		float deltaTime;
	};
}
#endif

