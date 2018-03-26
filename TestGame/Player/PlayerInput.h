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

		std::string ReturnJoystickStateForNetworking();
		std::vector<int> networkedJoystickInputs;
		std::vector<int> lastNetworkKeyPressed;
		void ParseNetworkInputs(std::string inputs);
		void makeNetworked();
		bool isNetworked();
		inline void setPlayerNum(int num) { playerNum = num; };

	private:

		SDL_Joystick* joystick = nullptr;
		Sint16 Tare[6];
		glm::vec2 AnyJoystick(int indexA, int indexB);
		int playerNum = 0;
		bool networkedPlayer = false;
		void ResetNetworkedInputs();
	};
}
#endif
