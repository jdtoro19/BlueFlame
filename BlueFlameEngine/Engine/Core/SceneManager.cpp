#include "SceneManager.h"

using namespace ENGINE;

SceneManager::SceneManager(Window* w)
{
	window = w;

	renderer = new Renderer();

	camera = new Camera(glm::vec3(-1.0f, 1.0f, 3.0f));

	SDL_CaptureMouse(SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_ShowCursor(SDL_DISABLE);

	currentScene = new TestScene();

	renderer->AddObject(currentScene->GetObjectList());
	renderer->AddLightObject(currentScene->GetLightObjectList());
}

SceneManager::~SceneManager()
{
}

bool SceneManager::IsQuit() {
	return quit;
}

void SceneManager::Update() {
	currentScene->Update();
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

	if (state[SDL_SCANCODE_ESCAPE]) {
		std::cout << "ESCAPE key event" << std::endl;
		quit = true;
	}
	currentScene->HandleEvents(events);
}

