#include "Crosshair.h"

using namespace GAME;

Crosshair::Crosshair()
{
	crosshair = new ImageUI();
	crosshair->SetImage("Resources/Textures/CharacterSelectScreen/crosshair.png");
	crosshair->SetPosition(940, 600);
	//crosshair->SetHeight(250);
	//crosshair->SetWidth(250);

	// Make player input
	playerInput = new PlayerInput();
	canMove = false;
}

Crosshair::Crosshair(float x, float y)
{
	crosshair = new ImageUI();
	crosshair->SetImage("Resources/Textures/CharacterSelectScreen/crosshair.png");
	crosshair->SetPosition(x, y);

	// Make player input
	playerInput = new PlayerInput();
	canMove = false;
}

Crosshair::Crosshair(const char* filepath, float x, float y)
{
	crosshair = new ImageUI();
	crosshair->SetImage(filepath);
	crosshair->SetPosition(x, y);

	// Make player input
	playerInput = new PlayerInput();
	canMove = false;
}

Crosshair::~Crosshair()
{

}

ImageUI* Crosshair::GetImage()
{
	return crosshair;
}

void Crosshair::Update(const float deltaTime)
{
	// If player can move update movement 
	if (canMove) {
		if (playerInput->CheckForController()) {//if they have a controller
			glm::vec2 mods = playerInput->LeftJoystick();

			if (mods.x > 0.01f) {
				crosshair->SetPosition(crosshair->GetPosition().x + deltaTime * 500, crosshair->GetPosition().y);
			}
			if (mods.x < -0.01f) {
				crosshair->SetPosition(crosshair->GetPosition().x - deltaTime * 500, crosshair->GetPosition().y);
			}
			if (mods.y > 0.01f) {
				crosshair->SetPosition(crosshair->GetPosition().x, crosshair->GetPosition().y + deltaTime * 500);
			}
			if (mods.y < -0.01f) {
				crosshair->SetPosition(crosshair->GetPosition().x, crosshair->GetPosition().y - deltaTime * 500);
			}
		}
	}
}

void Crosshair::HandleEvents(SDL_Event events)
{

}

void Crosshair::HandleStates(const Uint8 *state)
{

}
