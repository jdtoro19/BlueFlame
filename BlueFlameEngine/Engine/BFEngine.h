#ifndef BFEngine_H
#define BFEngine_H

#include <memory>
#include "Core/Window.h"
#include "Core/SceneManager.h"
#include "Core/Timer.h"
#include "InputHandling/InputHandler.h"
#include "InputHandling/PlayerController.h"
#include "Timers/MasterClock.h"
#include "Timers/Cooldown.h"

namespace ENGINE {

	//SINGLETON CLASS
	class BFEngine {
	public:
		//Delete default constructors
		BFEngine(const BFEngine&) = delete;
		BFEngine(BFEngine&&) = delete;
		BFEngine& operator=(const BFEngine&) = delete;
		BFEngine& operator=(BFEngine&&) = delete;

		// Get the current version of this class
		static BFEngine* GetInstance();

		bool Initialize();
		void Run();
		void Update(const float deltaTime);
		void FixedUpdate();
		void PreRender();
		void Render();
		void PostRender();

		SceneManager* GetSceneManager();

		static void TerminateGame();		

		//going to count from joysticks right now
		PlayerController players[4];
		int numPlayers = NULL;
		//use this to get player 0, player 1, etc. Otherwise you're taking joystick index
		int indexOfPlayer[4]; 

	private:
		//Private Constructor and Destructor so no other class can create it
		BFEngine();
		~BFEngine();
		
		static std::unique_ptr<BFEngine> BFEngineInstance;
		friend std::default_delete<BFEngine>;

		void SetUpPlayers();

		bool isRunning;
		bool firstLoad;

		static Window* window;

		SceneManager* sceneManager;

		Cooldown* RoundTimer;
	};
}

#endif
