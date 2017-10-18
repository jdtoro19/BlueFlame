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
			std::cout << SDL_JoystickNumButtons(InputHandler::joystick[i]) << " buttons were found." << std::endl;
			std::cout << SDL_JoystickNumBalls(InputHandler::joystick[i]) << " balls were found." << std::endl;
			std::cout << SDL_JoystickNumHats(InputHandler::joystick[i]) << " hats were found." << std::endl;
		}
	}

	
	
}

int InputHandler::jCheck() {
	return InputHandler::connectedControllers;
}



