#include "InputHandler.h"
#include "SDL/SDL_gamecontroller.h"
#include <iostream>
#include <..\..\BlueFlameEngine\Engine\BFEngine.h>

using namespace ENGINE;

std::unique_ptr<InputHandler> InputHandler::Input(nullptr);


InputHandler::InputHandler() {

}

InputHandler::~InputHandler() {

}

InputHandler* InputHandler::GetInstance() {
	if (Input.get() == nullptr) {
		Input.reset(new InputHandler);
	}
	return Input.get();
}

void InputHandler::InitControllers() {
	InputHandler::connectedControllers = SDL_NumJoysticks();
	if (InputHandler::connectedControllers == 0) {
		std::cout << "No joysticks were found." << std::endl;
	}
	else {
		std::cout << "we got one!" << std::endl;
		for (int i = 0; i < InputHandler::connectedControllers; i++) {
			InputHandler::joystick[i] = SDL_JoystickOpen(i);

			//tareing the joystick
			InputHandler::TareX[i] = SDL_JoystickGetAxis(InputHandler::GetInstance()->joystick[i], 0) * -1;
			InputHandler::TareY[i] = SDL_JoystickGetAxis(InputHandler::GetInstance()->joystick[i], 1) * -1;

			InputHandler::controller[i] = SDL_GameControllerOpen(i);
			if (InputHandler::controller[i]) {
				std::cout << "the controller did connect" << std::endl;
			}
			else {
				fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
			}
			if (SDL_IsGameController(i)) {
				std::cout << "available for game controller use" << std::endl;
			}
			else {
				fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
			}

			//std::cout << "the controller did connect and its name is " << SDL_IsGameController(i) << std::endl;
			std::cout << SDL_JoystickNumButtons(InputHandler::joystick[i]) << " buttons were found." << std::endl;
			std::cout << SDL_JoystickNumBalls(InputHandler::joystick[i]) << " balls were found." << std::endl;
			std::cout << SDL_JoystickNumHats(InputHandler::joystick[i]) << " hats were found." << std::endl;
			std::cout << SDL_JoystickNumAxes(InputHandler::joystick[i]) << " axes were found." << std::endl;
		}
	}



}

int InputHandler::jCheck() {
	return InputHandler::connectedControllers;
}

bool InputHandler::areJoysticksLive() {
	if (connectedControllers == 0) {
		return false;
	}
	else {
		return true;
	}
}

glm::vec3 InputHandler::playerMotion(int p) {
	Sint32 x_move = SDL_JoystickGetAxis(BFEngine::GetInstance()->players[p].pStick, 0);
	Sint32 y_move = SDL_JoystickGetAxis(BFEngine::GetInstance()->players[p].pStick, 1);

	//tareing the joysticks

	//if (x_move + InputHandler::GetInstance()->TareX[0])

	x_move += BFEngine::GetInstance()->players[p].TareX;
	y_move += BFEngine::GetInstance()->players[p].TareY;

	float modifierX = 0;
	float modifierY = 0;

	Sint16 jStickMod = 1000;

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

	glm::vec3 returnDis = glm::vec3(modifierX, modifierY, 0);
	return returnDis;
}
