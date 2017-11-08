#include "PlayerController.h"

using namespace ENGINE;

PlayerController::PlayerController() {

}

PlayerController::PlayerController(SDL_Joystick* ref, int index) {
	pInput = KEYBOARD;
	pStick = ref;
	PIndex = index;
	pTeam = RED;
	invertControls = 1;
	instanceID = SDL_JoystickInstanceID(ref);
	TareX = SDL_JoystickGetAxis(ref, 0) * -1;
	TareY = SDL_JoystickGetAxis(ref, 1) * -1;
	shotDelay = 0;
}

PlayerController::~PlayerController() {

}

void PlayerController::playerControls(Cube* thisThing) {
	PlayerObject = thisThing;
}

void PlayerController::setTeam(int team){
	//0 for red, 1 for blue
	if (team == 0) {
		pTeam = RED;
		invertControls = 1;
	}
	else {
		pTeam = BLUE;
		invertControls = -1;
	}
}

Cube* PlayerController::pObject() {
	return PlayerObject;
}

int PlayerController::inverted() {
	return invertControls;
}

void PlayerController::tick() {
	if (shotDelay > 0) {
		--shotDelay;
	}
}

void PlayerController::weShot() {
	shotDelay = 30;
}

glm::vec3 PlayerController::getTeamColor() {
	if (pTeam == Team::RED) {
		return glm::vec3(0.5f, 0.0f, 0.5f);
	}
	else {
		return glm::vec3(0.0f, 0.5f, 0.0f);
	}
}