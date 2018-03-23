#pragma once
#ifndef PLAYERINPUT_H
#define PLAYERINPUT_H

#include <glm\glm.hpp>
#include <BlueFlameEngine\Engine\InputHandling\InputManager.h>

using namespace ENGINE;

namespace GAME {

	// Handles player joysticks
	class PlayerInput {

	public:
		PlayerInput();
		~PlayerInput();

		//joystick stuff
		void AddJoystick(SDL_Joystick* jstick);
		bool CheckForController(SDL_JoystickID id);
		bool CheckForController();
		void AddAnyController();
		SDL_Joystick* GetJoystick();
		void SetJoystick(SDL_Joystick* js);

		glm::vec2 LeftJoystick();
		glm::vec2 RightJoystick();
		bool LeftTriggerPressed();
		bool RightTriggerPressed();

	private:

		SDL_Joystick* joystick = nullptr;
		Sint16 Tare[6];
		glm::vec2 AnyJoystick(int indexA, int indexB);
	};
}
#endif
