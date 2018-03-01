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
	SetUpControllers();

	cout << "BFEngine Initialized" << endl;

	return true;
}

void BFEngine::Run() 
{
	// Set the engine to running
	isRunning = true;

	// Set up timer
	Timer::GetInstance().Start();

	double t = 0.0;
	double dt = 1 / 50.0f;
	
	double currentTime = Timer::GetInstance().GetSeconds();
	double accumulator = 0.0;

	while (isRunning)
	{
		// Update timer
		Timer::GetInstance().Update();

		double newTime = Timer::GetInstance().GetSeconds();
		double frameTime = newTime - currentTime;
		if (frameTime > 0.25f)
			frameTime = 0.25f;
		currentTime = newTime;

		accumulator += frameTime;

		/*while (accumulator >= dt) {
			FixedUpdate();
			accumulator -= dt;
			t += dt;
		}*/

		// Call game loop functions
		
		Update(Timer::GetInstance().GetDeltaTime());
		PreRender();
		Render();
		PostRender();
		
		// Check if the scene manager has quit
		if (sceneManager->IsQuit()) {
			isRunning = false;
		}		

		// Only display window after the first render has been called
		if (firstLoad) {
			cout << "Displaying window..." << endl;
			SDL_ShowWindow(window->GetWindow());
			firstLoad = false;
		}

		// Clamp frame rate
		//SDL_Delay(1.0f/144.0f);
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

void BFEngine::SetUpControllers()
{
	// Initialize Input Handler
	InputHandler::GetInstance()->InitControllers();
	numPlayers = InputHandler::GetInstance()->jCheck();

	// Check if controllers were detected
	if (numPlayers != 0) 
	{
		// Iterate through the players and set them up
		for (int i = 0; i < numPlayers; i++) 
		{
			int temp = SDL_JoystickInstanceID(InputHandler::GetInstance()->joystick[i]);
			indexOfPlayer[i] = temp; // Translates player X into joystick X
			cout << "Setting up instance ID " << temp << endl;
			players[temp] = PlayerController(InputHandler::GetInstance()->joystick[i], temp);
			cout << "Successfully set up player " << i << endl;
		}
	}
}