#ifndef BFEngine_H
#define BFEngine_H

#include <memory>
#include "Core/Window.h"
#include "Core/SceneManager.h"

namespace ENGINE {

	//SINGLETON CLASS
	class BFEngine {
	public:
		//Delete default constructors
		BFEngine(const BFEngine&) = delete;
		BFEngine(BFEngine&&) = delete;
		BFEngine& operator=(const BFEngine&) = delete;
		BFEngine& operator=(BFEngine&&) = delete;

		//Instance getter to allow them to get the current version of this class
		static BFEngine* GetInstance();

		static void TerminateGame();
		
		SceneManager* GetSceneManager();

		bool Initialize();
		void Run();
		void Update(const float deltaTime);
		void Render();
		void Draw();

	private:
		//Private Constructor and Destructor so no other class can create it
		BFEngine();
		~BFEngine();
		
		static std::unique_ptr<BFEngine> BFEngineInstance;
		friend std::default_delete<BFEngine>;

		bool isRunning;

		static Window* window;

		SceneManager* sceneManager;
	};
}

#endif
