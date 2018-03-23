#include "PlayerInput.h"

using namespace GAME;

PlayerInput::PlayerInput() {

}

PlayerInput::~PlayerInput() {

}

void PlayerInput::AddJoystick(SDL_Joystick* jstick) {

	joystick = jstick;
	for (int i = 0; i < 6; i++) {
		Tare[i] = SDL_JoystickGetAxis(joystick, i) * -1;
	}
	int y = 0;
}

bool PlayerInput::CheckForController(SDL_JoystickID id) {
	if (joystick != nullptr) {
		return true;
	}
	else {
		AddJoystick(InputManager::GetInstance()->getController(id));
	}
	return false;
}
bool PlayerInput::CheckForController() {
	if (joystick != nullptr) {
		return true;
	}
	else {
	}
	return false;
}

void PlayerInput::AddAnyController() {
	AddJoystick(InputManager::GetInstance()->getAnyController());
}

SDL_Joystick* PlayerInput::GetJoystick() {
	return joystick;
}

void PlayerInput::SetJoystick(SDL_Joystick* js) { 
	joystick = js;
}

glm::vec2 PlayerInput::LeftJoystick() {
	return AnyJoystick(0, 1);
}

glm::vec2 PlayerInput::RightJoystick() {
	return AnyJoystick(3, 4);
}

bool PlayerInput::LeftTriggerPressed() {
	/*Sint32 x_move1 = SDL_JoystickGetAxis(joystick, 0);
	Sint32 x_move2 = SDL_JoystickGetAxis(joystick, 1);

	Sint32 x_move3 = SDL_JoystickGetAxis(joystick, 3);
	Sint32 x_move5 = SDL_JoystickGetAxis(joystick, 4);
	Sint32 x_move4 = SDL_JoystickGetAxis(joystick, 5);*/

	Sint32 x_move = SDL_JoystickGetAxis(joystick, 2);

	//cout << "original x_move: " << x_move << endl;
	//x_move += Tare[2];

	//cout << "tare: " << Tare[2] << endl;
	if (x_move > -500) {
		return true;
	}
	else {
		return false;
	}
}

bool PlayerInput::RightTriggerPressed() {
	Sint32 x_move = SDL_JoystickGetAxis(joystick, 5);

	if (x_move > -500) {
		return true;
	}
	else {
		return false;
	}
}

glm::vec2 PlayerInput::AnyJoystick(int indexA, int indexB) {
	Sint32 x_move = SDL_JoystickGetAxis(joystick, indexA);
	Sint32 y_move = SDL_JoystickGetAxis(joystick, indexB);

	//cout << x_move << " " << y_move << endl;

	x_move += Tare[indexA];
	y_move += Tare[indexB];

	float modifierX = 0;
	float modifierY = 0;

	Sint16 jStickMod = 10000;

	if (x_move < 320 && x_move > -320) {
		modifierX = 0;
	}
	else if (x_move >= 320) {

		modifierX = x_move / jStickMod; //right
	}
	else if (x_move <= -320) {
		modifierX = x_move / jStickMod; //left
	}

	if (y_move < 320 && y_move > -320) {
		modifierY = 0;
	}

	else if (y_move >= 320) {
		modifierY = y_move / jStickMod; //down
	}
	else if (y_move <= -320) {
		modifierY = y_move / jStickMod; //up
	}
	//cout << x_move << " " << x_move / jStickMod << " " << y_move << " " << y_move / jStickMod << endl;
	//std::cout << "ModX: " << modifierX << " ModY: " << modifierY << std::endl;

	glm::vec2 returnDis = glm::vec2(modifierX, modifierY);
	return returnDis;
}