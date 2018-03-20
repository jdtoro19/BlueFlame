#pragma once
#ifndef BFEngine_H
#define BFEngine_H

#include <memory>
#include "Core/Window.h"
#include "Core/SceneManager.h"
#include "Core/Timer.h"
#include "Timers/MasterClock.h"
#include "InputHandling/InputManager.h"

namespace ENGINE {

	// SINGLETON CLASS
	class BFEngine 
	{
	public:
		// Delete default constructors
		BFEngine(const BFEngine&) = delete;
		BFEngine(BFEngine&&) = delete;
		BFEngine& operator=(const BFEngine&) = delete;
		BFEngine& operator=(BFEngine&&) = delete;

		bool Initialize();
		void Run();
		void UpdateState();
		void FixedUpdate(const float deltaTime);
		void Update(const float deltaTime);		
		void PreRender();
		void Render();
		void PostRender();

		// Get the current version of this class
		static BFEngine* GetInstance();		

		// Get the scene manager
		SceneManager* GetSceneManager();

		// Window option setters
		void SetWindowName(std::string name);
		void SetWindowDimensions(int width, int height);

		// Clean up
		static void TerminateGame();

	private:
		// Private Constructor and Destructor so no other class can create it
		BFEngine();
		~BFEngine();
		
		static std::unique_ptr<BFEngine> BFEngineInstance;
		friend std::default_delete<BFEngine>;

		// Running check
		bool isRunning;
		bool firstLoad;

		// Window
		static Window* window;

		// Scene Manager
		SceneManager* sceneManager;

		// Window options
		std::string windowName;
		int width;
		int height;

		// Interpolation between fixed game loop and rendering loop
		double interpolation;
	};
}

#endif
