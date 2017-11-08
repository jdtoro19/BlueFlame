#ifndef BFEngine_H
#define BFEngine_H

#include <memory>
#include "Core/Window.h"
#include "Core/SceneManager.h"
#include "Core/Timer.h"
#include "InputHandling/InputHandler.h"
#include "InputHandling/PlayerController.h"

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
		void Render();
		void Draw();

		static void TerminateGame();

		SceneManager* GetSceneManager();

		//going to count from joysticks right now
		PlayerController players[4];
		int numPlayers = NULL;

		int indexOfPlayer[4]; //use this to get player 0, player 1, etc. Otherwise you're taking joystick index

	private:
		//Private Constructor and Destructor so no other class can create it
		BFEngine();
		~BFEngine();
		
		static std::unique_ptr<BFEngine> BFEngineInstance;
		friend std::default_delete<BFEngine>;

		void setUpPlayers();

		bool isRunning;

		static Window* window;

		SceneManager* sceneManager;
	};
}

#endif
