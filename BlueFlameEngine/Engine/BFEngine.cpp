#include "BFEngine.h"

using namespace ENGINE;

//Initialize static variables
std::unique_ptr<BFEngine> BFEngine::BFEngineInstance(nullptr);
Window* BFEngine::window(nullptr);

//Initialize member variables
BFEngine::BFEngine() : isRunning(false) {

}

BFEngine::~BFEngine() {
	TerminateGame();
}

void BFEngine::TerminateGame() {

	exit(0);

	window->Shutdown();
	window = nullptr;

	SDL_Quit();
}

BFEngine* BFEngine::GetInstance() {
	if (BFEngineInstance.get() == nullptr) {
		BFEngineInstance.reset(new BFEngine);
	}
	return BFEngineInstance.get();
}

bool BFEngine::Initialize() {

	//Create a new window
	window = new Window();
	//Iniitalize the window with the window name and size
	window->Initialize("Blue Flame Engine", 1280, 720);

	sceneManager = new SceneManager(window);

	return true;
}

void BFEngine::Run() {
	isRunning = true;

	while (isRunning) {
		Update();
		Render();
		if (sceneManager->IsQuit()) {
			isRunning = false;
		}
	}
}

void BFEngine::Update() {
	sceneManager->Update();
	sceneManager->HandleEvents();
}

void BFEngine::Render() {
	sceneManager->Render();
}

void BFEngine::Draw() {

}