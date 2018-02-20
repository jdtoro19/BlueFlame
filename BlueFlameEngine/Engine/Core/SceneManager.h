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

		// ENGINE FUNCTIONS

		// Initializes the SceneManager
		void Initialize(Window* w);
		// Update SceneManager and the current scene
		void Update(const float deltaTime);
		// Clears the screen and sets up render options
		void PreRender();
		// Render objects
		void Render();
		// Render frame buffers
		void PostRender();
		// Draw UI elements
		void Draw();		
		// Process user input
		void HandleEvents();
		//

		// QUIT GAME
		
		// Check the quit bool
		bool IsQuit();
		// Set the quit bool
		void SetIsQuit(bool quit);
		//

		// SCENE MANAGEMENT
		
		// Switches the scene to the scene given
		void SwitchScene(Scene* scene);
		// Adds the given scene to the scene list
		void AddScene(Scene* scene);
		// Sets the scene to the next scene in the scene list
		void NextScene();
		// Sets the scene to the previous scene in the scene list
		void PreviousScene();
		// Clears the scene list
		void ClearSceneList();
		// Initializes first scene in scene list
		void StartScene();
		//

		// SCREEN OPTIONS
		
		// Called when the window is resized
		void OnResize(int w, int h);
		// Set if window is in splitscreen mode
		void EnableSplitscreen(bool setSplitscreen);
		// Set if window is in full screen mode
		void EnableFullscreen(bool setFullScreen);
		// Set whether to show the fps or not
		void ShowFPS(bool setShowFPS);
		// Set whether to capture the mouse or not
		void CaptureMouse(bool setCaptureMouse);
		//

		// DEBUG FUNCTIONS
		
		// Add the given string to the debug text list
		void DebugText(std::string string);
		// Convert and add the given int to the debug text list
		void DebugText(int value);
		// Convert and add the given double to the debug text list
		void DebugText(double value);
		// Convert and add the given float to the debug text list
		void DebugText(float value);
		// Convert and add the given vec3 to the debug text list
		void DebugText(glm::vec3 value);
		// Draws the given string (Only for use in the scenemanager)
		void DebugText(std::string string, GLfloat x, GLfloat y);
		// Draws all strings in the debug text list with an offset
		void DrawDebugText();
		// Draws the FPS
		void DrawDebugFPS();
		//

		// GETTERS

		// Returns the current scene
		Scene* GetCurrentScene();
		// Returns the renderer
		Renderer* GetRenderer();
		// Returns the window width
		int GetWindowWidth();
		// Returns the window height
		int GetWindowHeight();
		// Returns the width used for the UI
		// Do not use this if you want the window width
		float GetScreenWidth();
		// Returns the height used for the UI
		// Do not use this if you want the window height
		float GetScreenHeight();
		//
		
	private:
		// SceneManager variables
		Window* window;
		UIRenderer* uiRenderer;
		Renderer* renderer;
		SDL_Event events;
		bool loading = false;
		// Scene variables
		Scene* currentScene;
		std::vector<Scene*> sceneList;
		int sceneIter = 0;
		// Debug text variables
		std::vector<std::string> debugTextList;
		float debugTextOffset = 25.0f;
		// Options
		bool quit = false;
		bool splitscreen = false;
		bool showFPS = false;
		bool captureMouse = false;
		// FPS debug variables
		float fps = 0;
		float fpsTimer = 0;
		float fpsTimerSet = 0.1f;
		// Deltatime
		float deltaTime;
		// Resolution Scale
		float resolutionScale = 1.0f;
	};
}
#endif