#include "BFEngine.h"

using namespace ENGINE;

//Initialize static variables
std::unique_ptr<BFEngine> BFEngine::BFEngineInstance(nullptr);
Window* BFEngine::window(nullptr);

//Initialize member variables
BFEngine::BFEngine() : isRunning(false), sceneManager(nullptr) {

}

BFEngine::~BFEngine() {

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

	//Create a new window
	window = new Window();
	//Iniitalize the window with the window name and size
	window->Initialize("Blue Flame Engine", 1280, 720);
	InputHandler::GetInstance()->InitControllers();
	sceneManager = new SceneManager();
	sceneManager->Initialize(window);

	setUpPlayers(); //i hope this works

	return true;
}

void BFEngine::Run() {
	isRunning = true;

	Timer timer;
	timer.Start();

	while (isRunning) {
		timer.UpdateFrameTicks();

		Update(timer.GetDeltaTime());
		Render();
		Draw();
		
		if (sceneManager->IsQuit()) {
			isRunning = false;
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

void BFEngine::Render() {
	sceneManager->Render();
}

void BFEngine::Draw() {
	sceneManager->Draw();
}

SceneManager* BFEngine::GetSceneManager() {
	return sceneManager;
}

void BFEngine::setUpPlayers() {
	InputHandler::GetInstance()->InitControllers();
	numPlayers = InputHandler::GetInstance()->jCheck();

	if (numPlayers == 0) {
		//no controllers attached, bypass this step
		std::cout << "No controllers to set up " << std::endl;
	}
	else {
		//iterate through the players and set them up
		for (int i = 0; i < numPlayers; i++) {
			//for (int i = numPlayers - 1; i > -1; i--) {
			int temp = SDL_JoystickInstanceID(InputHandler::GetInstance()->joystick[i]); //dumb workaround
			indexOfPlayer[i] = temp; //translates player X into joystick X
			cout << "Setting up instance ID " << temp << endl;
			players[temp] = PlayerController(InputHandler::GetInstance()->joystick[i], temp); //initial constructor
			std::cout << "Successfully set up player " << i << std::endl;
		}

	}
}