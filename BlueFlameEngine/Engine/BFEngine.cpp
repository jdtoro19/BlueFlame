#include "BFEngine.h"

using namespace ENGINE;

//Initialize static variables
std::unique_ptr<BFEngine> BFEngine::BFEngineInstance(nullptr);
Window* BFEngine::window(nullptr);

//Initialize member variables
BFEngine::BFEngine() : isRunning(false), firstLoad(true), sceneManager(nullptr) {

}

BFEngine::~BFEngine() {
	cout << "Shutting down..." << endl;

	delete sceneManager;
	sceneManager = nullptr;
	
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

	Clock::GetInstance()->init();

	//Create a new window
	window = new Window();
	//Iniitalize the window with the window name and size
	window->Initialize("Blue Flame Engine", 1280, 720);
	cout << "Window initialized to " << window->GetWidth() << " by " << window->GetHeight() << endl;
	
	sceneManager = new SceneManager();
	sceneManager->Initialize(window);
	cout << "Scene Manager Initialized" << endl;

	SetUpPlayers();

	RoundTimer = new Cooldown(60.0); //sixty second timer
	//RoundTimer->startCD();

	cout << "BFEngine Initialized" << endl;
	return true;
}

void BFEngine::Run() {
	isRunning = true;

	Timer timer;
	timer.Start();

	while (isRunning) {
		timer.UpdateFrameTicks();

		Update(timer.GetDeltaTime());
		FixedUpdate();
		PreRender();
		Render();

		if (RoundTimer->checkOffCD() == false) {
			cout << "Time left in Round: " << RoundTimer->secondsLeft() << endl;
			//use lower explicit int cast if you want round numbers
			//cout << "Time left in Round: " << (int)RoundTimer->secondsLeft() << endl;
		}
		else {
			//When the main timer is over
			//BFEngine::GetInstance()->TerminateGame();
		}

		PostRender();
		
		if (sceneManager->IsQuit()) {
			isRunning = false;
		}

		if (firstLoad) {
			cout << "Displaying window..." << endl;
			SDL_ShowWindow(window->GetWindow());
			firstLoad = false;			
		}

		// Keeep the event loop running at a sane rate
		SDL_Delay(timer.GetSleepTime(144));
		//std::cout << "main loop running at: " << (1.0f/timer.GetDeltaTime()) << " frames/sec" << std::endl;
	}
}

void BFEngine::Update(const float deltaTime) {
	sceneManager->Update(deltaTime);
	sceneManager->HandleEvents();
}

void BFEngine::FixedUpdate() {

}

void BFEngine::PreRender() {
	sceneManager->PreRender();
}

void BFEngine::Render() {
	sceneManager->Render();
}

void BFEngine::PostRender() {
	sceneManager->PostRender();
}

SceneManager* BFEngine::GetSceneManager() {
	return sceneManager;
}

void BFEngine::SetUpPlayers() {
	InputHandler::GetInstance()->InitControllers();
	numPlayers = InputHandler::GetInstance()->jCheck();

	if (numPlayers == 0) {
		//no controllers attached, bypass this step
		cout << "No controllers to set up " << endl;
	}
	else {
		//iterate through the players and set them up
		for (int i = 0; i < numPlayers; i++) {
			//for (int i = numPlayers - 1; i > -1; i--) {
			int temp = SDL_JoystickInstanceID(InputHandler::GetInstance()->joystick[i]); //dumb workaround
			indexOfPlayer[i] = temp; //translates player X into joystick X
			cout << "Setting up instance ID " << temp << endl;
			players[temp] = PlayerController(InputHandler::GetInstance()->joystick[i], temp); //initial constructor
			cout << "Successfully set up player " << i << endl;
		}

	}
}