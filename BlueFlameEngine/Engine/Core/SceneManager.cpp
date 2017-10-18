#include "SceneManager.h"
#include "Timer.h"

#include "DefaultScene.h"

using namespace ENGINE;

SceneManager::SceneManager(Window* w)
{
	window = w;

	renderer = new Renderer();

	camera = new Camera();

	SwitchScene(new DefaultScene());
}

SceneManager::~SceneManager()
{
}

bool SceneManager::IsQuit() {
	return quit;
}

void SceneManager::Update(const float deltaTime) {
	currentScene->Update(deltaTime);
}

void SceneManager::Render() {
	renderer->Render(window, camera, projection);
}

void SceneManager::HandleEvents() {
	while (SDL_PollEvent(&events))
	{
		if (events.type == SDL_QUIT) {
			quit = true;
		}

		if (events.type == SDL_MOUSEMOTION) {
			if (firstMouse)
			{
				lastX = events.motion.x;
				lastY = events.motion.y;
				firstMouse = false;
			}

			float xoffset = events.motion.x - lastX;
			float yoffset = lastY - events.motion.y; 

			lastX = events.motion.x;
			lastY = events.motion.y;
			camera->ProcessMouseMovement(xoffset, yoffset);
		}

		if (events.type == SDL_MOUSEWHEEL) {
			camera->ProcessMouseScroll(events.wheel.y);
		}

		if (events.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
			OnResize(events.window.data1, events.window.data2);
		}

		if (events.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
			SDL_CaptureMouse(SDL_FALSE);
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}

		if (events.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
			SDL_CaptureMouse(SDL_TRUE);
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);

	// VIEW/CAMERA
	if (state[SDL_SCANCODE_T]) {
		camera->ProcessKeyboard(FORWARD, 0.005f);
	}
	if (state[SDL_SCANCODE_G]) {
		camera->ProcessKeyboard(BACKWARD, 0.005f);
	}
	if (state[SDL_SCANCODE_H]) {
		camera->ProcessKeyboard(RIGHT, 0.005f);
	}
	if (state[SDL_SCANCODE_F]) {
		camera->ProcessKeyboard(LEFT, 0.005f);
	}

	if (state[SDL_SCANCODE_V]) {
		window->ToggleFullScreen();
	}

	if (state[SDL_SCANCODE_ESCAPE]) {
		std::cout << "ESCAPE key event" << std::endl;
		quit = true;
	}
	currentScene->HandleEvents(events);
}

void SceneManager::SwitchScene(Scene* scene) {
	renderer->ClearObjects();

	currentScene = scene;

	renderer->AddObject(currentScene->GetObjectList());
	renderer->AddLightObject(currentScene->GetLightObjectList());

	if (camera) delete camera;
	camera = new Camera(currentScene->GetStartPos());	
}

void SceneManager::OnResize(int w, int h) {
	window->SetWindowSize(w, h);
	glViewport(0, 0, window->GetWidth(), window->GetHeight());
}