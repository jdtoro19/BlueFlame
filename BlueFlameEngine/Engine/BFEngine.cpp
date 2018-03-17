#include "BFEngine.h"

using namespace ENGINE;

//Initialize static variables
std::unique_ptr<BFEngine> BFEngine::BFEngineInstance(nullptr);
Window* BFEngine::window(nullptr);

//Initialize member variables
BFEngine::BFEngine() : isRunning(false), firstLoad(true), sceneManager(nullptr), windowName("Blue Flame Engine"), width(1280), height(720)
{

}

BFEngine::~BFEngine() 
{
	cout << "Shutting down..." << endl;

	delete sceneManager;
	sceneManager = nullptr;
	
	TerminateGame();
}

void BFEngine::TerminateGame() 
{
	exit(0);

	window->Shutdown();
	window = nullptr;	

	SDL_Quit();
}

BFEngine* BFEngine::GetInstance() 
{
	if (BFEngineInstance.get() == nullptr) {
		BFEngineInstance.reset(new BFEngine);
	}
	return BFEngineInstance.get();
}

bool BFEngine::Initialize() 
{
	// Create a new window
	window = new Window();
	// Iniitalize the window with the window name and size
	window->Initialize(windowName, width, height);
	cout << "Window initialized to " << window->GetWidth() << " by " << window->GetHeight() << endl;
	
	// Initialize Scene Manager
	sceneManager = new SceneManager();
	sceneManager->Initialize(window);
	cout << "Scene Manager Initialized" << endl;

	// Initialize clock
	Clock::GetInstance()->init();
	cout << "Clock Initialized" << endl;

	// Set up controllers
	InputManager::GetInstance()->initalizeControllers();

	cout << "BFEngine Initialized" << endl;

	return true;
}

void BFEngine::Run() 
{
	// Set the engine to running
	isRunning = true;

	// Set up timer
	Timer::GetInstance().Start();

	while (isRunning) 
	{
		// Check if the scene manager has quit
		if (sceneManager->IsQuit()) {
			isRunning = false;
		}

		// Update timer
		Timer::GetInstance().Update();

		// Call game loop functions
		FixedUpdate();
		Update(Timer::GetInstance().GetDeltaTime());
		PreRender();
		Render();
		PostRender();	

		// Only display window after the first render has been called
		if (firstLoad) {
			cout << "Displaying window..." << endl;
			SDL_ShowWindow(window->GetWindow());
			firstLoad = false;
		}
	}
}

void BFEngine::FixedUpdate() 
{

}

void BFEngine::Update(const float deltaTime) 
{
	sceneManager->Update(deltaTime);
	sceneManager->HandleEvents();
}

void BFEngine::PreRender() 
{
	sceneManager->PreRender();
}

void BFEngine::Render() 
{
	sceneManager->Render();
}

void BFEngine::PostRender() 
{
	sceneManager->PostRender();
}

SceneManager* BFEngine::GetSceneManager() 
{
	return sceneManager;
}

void BFEngine::SetWindowName(std::string name)
{
	windowName = name;
}

void BFEngine::SetWindowDimensions(int width, int height)
{
	this->width = width;
	this->height = height;
}