#pragma once
#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <SDL\SDL.h>
#include <memory>
#include "SDL/SDL_gamecontroller.h"
#include <vector>
#include <glm\glm.hpp>

namespace ENGINE {
	class InputHandler {
	private:
		unsigned int prevTicks;
		unsigned int currTicks;
	public:
		//Instance getter to allow them to get the current version of this class
		static InputHandler* GetInstance(); //we're using a singleton class so we only have one input handler
		void InitControllers();
		InputHandler();
		~InputHandler();
		int jCheck();
		SDL_Joystick *joystick[4];
		Sint16 TareX[4];
		Sint16 TareY[4];
		bool areJoysticksLive();
		bool upDown = false;
		glm::vec3 playerMotion(int p);
		

	private:
		int connectedControllers = 0;
		static std::unique_ptr<InputHandler> Input;

		
		SDL_GameController *controller[4];
	};
}

#endif
