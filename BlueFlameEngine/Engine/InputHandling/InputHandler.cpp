#include "InputHandler.h"
#include "SDL/SDL_gamecontroller.h"
#include <iostream>

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
		std::cout << "ERROR: no joysticks were found." << std::endl;
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

