#include "SceneManager.h"

using namespace ENGINE;

SceneManager::SceneManager() : currentScene(nullptr), window(nullptr), renderer(nullptr), uiRenderer(nullptr)
{
	// Capture Mouse
	//SDL_CaptureMouse(SDL_TRUE);
	//SDL_SetRelativeMouseMode(SDL_TRUE);
}

SceneManager::~SceneManager()
{
	delete currentScene;
	currentScene = nullptr;
	delete window;
	window = nullptr;
	delete renderer;
	renderer = nullptr;
	delete uiRenderer;
	uiRenderer = nullptr;
}

// Starts the window, renderer, and sets a default scene with nothing in it (incase no scene is initially set)
void SceneManager::Initialize(Window* w) {
	window = w;

	renderer = new Renderer();
	renderer->Initialize(window);
	cout << "Renderer Initialized" << endl;

	uiRenderer = new UIRenderer();
	uiRenderer->Initialize(window);
	cout << "UI Renderer Initialized" << endl;

	cout << "Default scene added" << endl;
	SwitchScene(new DefaultScene());
}

// Calls the current scene's update 
void SceneManager::Update(const float deltaTime) {
	this->deltaTime = deltaTime;
	currentScene->Update(deltaTime);
}

// The renderer is slit into PreRender, Render, and PostRender
void SceneManager::PreRender() {
	renderer->PreRender(window, currentScene->GetCameraList()[0], splitscreen);
}

void SceneManager::Render() {

	if (splitscreen) {
		// LEFT SCREEN
		glViewport(0, 0, window->GetWidth() / 2, window->GetHeight());
		renderer->Render(currentScene->GetCameraList()[0], currentScene->GetObjectList(), currentScene->GetDirLightList(), 
																						  currentScene->GetPointLightList(), 
																						  currentScene->GetSpotLightList());
		renderer->RenderSkybox(currentScene->GetSkybox(), currentScene->GetCameraList()[0]);

		// RIGHT SCREEN
		glViewport(window->GetWidth() / 2, 0, window->GetWidth() / 2, window->GetHeight());
		renderer->Render(currentScene->GetCameraList()[1], currentScene->GetObjectList(), currentScene->GetDirLightList(), 
																						  currentScene->GetPointLightList(), 
																						  currentScene->GetSpotLightList());
		renderer->RenderSkybox(currentScene->GetSkybox(), currentScene->GetCameraList()[1]);
	}
	else if (!splitscreen) {
		//glViewport(0, 0, 853, 480);
		glViewport(0, 0, window->GetWidth(), window->GetHeight());
		renderer->Render(currentScene->GetCameraList()[0], currentScene->GetObjectList(), currentScene->GetDirLightList(), 
																						  currentScene->GetPointLightList(), 
																						  currentScene->GetSpotLightList());
		renderer->RenderSkybox(currentScene->GetSkybox(), currentScene->GetCameraList()[0]);
	}

	// Just in case something in the scene needs to render
	currentScene->Render();
}

void SceneManager::PostRender() {
	
	glViewport(0, 0, window->GetWidth(), window->GetHeight());
	Draw();
	renderer->PostRender(window, splitscreen);
	if (showFPS) {
		DebugText("FPS: " + std::to_string((int)(1 / deltaTime)), GetScreenWidth() - 125.0f, 35.0f);
	}
	SDL_GL_SwapWindow(window->GetWindow());
}

// Used to render and 2d or UI elements
void SceneManager::Draw() {
	//glViewport(0, 0, 853, 480);
	glViewport(0, 0, window->GetWidth(), window->GetHeight());
	currentScene->Draw();
	uiRenderer->Draw(window, currentScene->GetUIObjectList());

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
			renderer->SetUpFrameBuffers(window);
		}

		// If you leave the window then mouse is no longer captured
		if (events.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
			//SDL_CaptureMouse(SDL_FALSE);
			//SDL_SetRelativeMouseMode(SDL_FALSE);
		}

		// Capture mouse again when user clicks in the window
		if (events.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
			//SDL_CaptureMouse(SDL_TRUE);
			//SDL_SetRelativeMouseMode(SDL_TRUE);
		}
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_V]) {
		window->SetFullScreen(false);
	}

	if (state[SDL_SCANCODE_B]) {
		window->SetFullScreen(true);
	}

	if (state[SDL_SCANCODE_N]) {
		EnableSplitscreen(false);
	}

	if (state[SDL_SCANCODE_M]) {
		EnableSplitscreen(true);
	}	

	if (state[SDL_SCANCODE_1]) {
		renderer->EnableGreyscale(false);
		renderer->EnableInvertedColours(false);
		renderer->EnableKernel(false);
	}

	if (state[SDL_SCANCODE_2]) {
		renderer->EnableGreyscale(true);
	}

	if (state[SDL_SCANCODE_3]) {
		renderer->EnableInvertedColours(true);
	}

	if (state[SDL_SCANCODE_4]) {
		renderer->EnableKernel(true);
	}

	if (state[SDL_SCANCODE_5]) {
		renderer->EnableBloom(true);
	}

	if (state[SDL_SCANCODE_6]) {
		renderer->EnableBloom(false);
	}

	if (state[SDL_SCANCODE_ESCAPE]) {
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
	cout << "Scene loaded" << endl;
}

void SceneManager::AddScene(Scene* scene) {
	scene->Initialize();
	sceneList.push_back(scene);
	cout << "Scene added" << endl;
}

void SceneManager::NextScene() {
	sceneIter++;
	if (sceneList[sceneIter] != NULL) {
		currentScene = sceneList[sceneIter];
		cout << "Next scene displayed" << endl;
	}
}

void SceneManager::PreviousScene() {
	sceneIter--;
	if (sceneList[sceneIter] != NULL) {
		currentScene = sceneList[sceneIter];
		cout << "Previous scene displayed" << endl;
	}
}

void SceneManager::ClearSceneList() {
	sceneIter = 0;
	if (sceneList.size() != 0) {
		sceneList.clear();
	}
}

void SceneManager::StartScene() {
	if (sceneList.size() != 0) {
		currentScene = sceneList[0];
		cout << "Scene loaded" << endl;
	}
	else {
		SwitchScene(new DefaultScene());
	}
}

// Resizes window
void SceneManager::OnResize(int w, int h) {
	window->SetWindowSize(w, h);
}

void SceneManager::EnableSplitscreen(bool setSplitscreen) {
	splitscreen = setSplitscreen;
}

void SceneManager::EnableFullscreen(bool setFullscreen) {
	window->SetFullScreen(setFullscreen);
}

void SceneManager::ShowFPS(bool setShowFPS) {
	showFPS = setShowFPS;
}

void SceneManager::CaptureMouse(bool capture) {
	if (capture) {
		SDL_CaptureMouse(SDL_TRUE);
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else if (!capture) {
		SDL_CaptureMouse(SDL_FALSE);
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

Renderer* SceneManager::GetRenderer() {
	return renderer;
}

int SceneManager::GetWindowWidth() {
	return window->GetWidth();
}

int SceneManager::GetWindowHeight() {
	return window->GetHeight();
}

float SceneManager::GetScreenWidth() {
	return uiRenderer->GetWidth();
}

float SceneManager::GetScreenHeight() {
	return uiRenderer->GetHeight();
}

void SceneManager::DebugText(std::string string, GLfloat x, GLfloat y) {
	uiRenderer->DebugText(string , x, y, 0.4f, glm::vec3(0.0, 1.0f, 0.0f));
}