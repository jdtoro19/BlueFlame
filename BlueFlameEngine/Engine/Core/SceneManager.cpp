#include "SceneManager.h"

using namespace ENGINE;

SceneManager::SceneManager() : currentScene(nullptr), window(nullptr), renderer(nullptr)
{
	// Capture Mouse
	SDL_CaptureMouse(SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

SceneManager::~SceneManager()
{
	delete currentScene;
	currentScene = nullptr;
	delete window;
	window = nullptr;
	delete renderer;
	renderer = nullptr;
}

// Starts the window, renderer, and sets a default scene with nothing in it (incase no scene is initially set)
void SceneManager::Initialize(Window* w) {
	window = w;

	renderer = new Renderer();
	renderer->Initialize(w);

	SwitchScene(new DefaultScene());
}

// Calls the current scene's update 
void SceneManager::Update(const float deltaTime) {
	currentScene->Update(deltaTime);
}

void SceneManager::Render() {

	// The renderer is slit into PreRender, Render, and PostRender
	
	renderer->PreRender(window, currentScene->GetCameraList()[0], splitscreen);

	if (splitscreen) {
		// LEFT SCREEN
		glViewport(0, 0, window->GetWidth() / 2, window->GetHeight());
		renderer->Render(currentScene->GetCameraList()[0], currentScene->GetObjectList(), currentScene->GetDirLightList(), 
																						  currentScene->GetPointLightList(), 
																						  currentScene->GetSpotLightList());
		// RIGHT SCREEN
		glViewport(window->GetWidth() / 2, 0, window->GetWidth() / 2, window->GetHeight());
		renderer->Render(currentScene->GetCameraList()[1], currentScene->GetObjectList(), currentScene->GetDirLightList(), 
																						  currentScene->GetPointLightList(), 
																						  currentScene->GetSpotLightList());
	}
	else if (!splitscreen) {
		glViewport(0, 0, window->GetWidth(), window->GetHeight());
		renderer->Render(currentScene->GetCameraList()[0], currentScene->GetObjectList(), currentScene->GetDirLightList(), 
																						  currentScene->GetPointLightList(), 
																						  currentScene->GetSpotLightList());
	}

	renderer->PostRender(window);

	// Just in case something in the scene needs to render
	currentScene->Render();
}

// Used to render and 2d or UI elements
void SceneManager::Draw() {
	currentScene->Draw();
}

// Handles all input and also calls the current scene's HandleEvent
void SceneManager::HandleEvents() {
	while (SDL_PollEvent(&events))
	{
		// If X window button is pressed then quit
		if (events.type == SDL_QUIT) {
			quit = true;
		}

		// Resize window
		if (events.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
			OnResize(events.window.data1, events.window.data2);
			//renderer->SetUpFrameBuffers(window);
		}

		// If you leave the window then mouse is no longer captured
		if (events.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
			SDL_CaptureMouse(SDL_FALSE);
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}

		// Capture mouse again when user clicks in the window
		if (events.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
			SDL_CaptureMouse(SDL_TRUE);
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_V]) {
		window->ToggleFullScreen();
	}

	if (state[SDL_SCANCODE_M]) {
		EnableSplitscreen(true);
	}

	if (state[SDL_SCANCODE_N]) {
		EnableSplitscreen(false);
	}

	if (state[SDL_SCANCODE_ESCAPE]) {
		std::cout << "ESCAPE key event" << std::endl;
		quit = true;
	}

	currentScene->HandleEvents(events);
}

// Flag indicating the user wants to quit
bool SceneManager::IsQuit() {
	return quit;
}

// Set the quit bool
void SceneManager::SetIsQuit(bool quit) {
	this->quit = quit;
}

// Used to switch scenes
void SceneManager::SwitchScene(Scene* scene) {
	// The new scene gets initialized
	currentScene = nullptr;
	delete currentScene;
	currentScene = scene;
	currentScene->Initialize();
}

// Resizes window
void SceneManager::OnResize(int w, int h) {
	window->SetWindowSize(w, h);
}

void SceneManager::EnableSplitscreen(bool setSplitscreen) {
	splitscreen = setSplitscreen;
}

Renderer* SceneManager::GetRenderer() {
	return renderer;
}