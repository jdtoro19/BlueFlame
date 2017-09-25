#include "TestScene.h"

using namespace GAME;

TestScene::TestScene()
{
	cube = new Cube();
	cube->renderComponent->SetColour(0.31f, 0.5f, 1.0f);
	cube->SetWorldPosition(-3.0f, 0.0f, 0.0f);
	cube->SetWorldScale(0.5f, 0.5f, 0.5f);

	cube2 = new Cube();	
	cube2->renderComponent->SetColour(1.0f, 0.5f, 0.31f);
	cube2->SetWorldPosition(-2.0f, 0.0f, 0.0f);

	model = new Model("Resources/Models/miku/miku.obj");
	model->SetWorldPosition(0.0f, 1.0f, 0.0f);
	model->SetWorldScale(0.02f);

	dirLight = new Light(LightComponent::Light_Type::DIRECTIONAL);

	AddObject(cube);
	AddObject(cube2);
	AddObject(model);
	AddLightObject(dirLight);
}

TestScene::~TestScene()
{
}

void TestScene::Update() 
{
	if (objectList.size() != NULL) {
		for (size_t i = 0; i < objectList.size(); ++i) {
			objectList.at(i)->Update();
		}
	}

	if (lightObjectList.size() != NULL) {
		for (size_t i = 0; i < lightObjectList.size(); ++i) {
			lightObjectList.at(i)->Update();
		}
	}
}

void TestScene::Render() 
{
	// Rendering handled by SceneManager.....for now...
}

void TestScene::HandleEvents(SDL_Event events)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	// CUBE 1
	// Movement
	if (state[SDL_SCANCODE_W]) {
		cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(0.0f, 0.005f, 0.0f));
	}
	if (state[SDL_SCANCODE_S]) {
		cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(0.0f, -0.005f, 0.0f));
	}
	if (state[SDL_SCANCODE_D]) {
		cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(0.005f, 0.0f, 0.0f));
	}
	if (state[SDL_SCANCODE_A]) {
		cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(-0.005f, 0.0f, 0.0f));
	}
	// Rotation
	if (state[SDL_SCANCODE_E]) {
		cube->SetWorldRotation(glm::vec3(0.0f, 0.0f, 1.0f), cube->GetWorldRotationAngle() - 0.005f);
	}
	if (state[SDL_SCANCODE_Q]) {
		cube->SetWorldRotation(glm::vec3(0.0f, 0.0f, 1.0f), cube->GetWorldRotationAngle() + 0.005f);
	}

	// Model
	// Movement
	if (state[SDL_SCANCODE_I]) {
		model->SetWorldPosition(model->GetWorldPosition() + glm::vec3(0.0f, 0.0f, -0.005f));
	}
	if (state[SDL_SCANCODE_K]) {
		model->SetWorldPosition(model->GetWorldPosition() + glm::vec3(0.0f, 0.0f, 0.005f));
	}
	if (state[SDL_SCANCODE_L]) {
		model->SetWorldPosition(model->GetWorldPosition() + glm::vec3(0.005f, 0.0f, 0.0f));
	}
	if (state[SDL_SCANCODE_J]) {
		model->SetWorldPosition(model->GetWorldPosition() + glm::vec3(-0.005f, 0.0f, 0.0f));
	}
	// Rotation
	if (state[SDL_SCANCODE_O]) {
		model->SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), model->GetWorldRotationAngle() - 0.005f);
	}
	if (state[SDL_SCANCODE_U]) {
		model->SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), model->GetWorldRotationAngle() + 0.005f);
	}
}

