#pragma once

#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <SDL\SDL.h>
#include <..\..\..\BlueFlame\BlueFlameEngine\Engine\Rendering\3D\Cube.h>


namespace ENGINE {
	class PlayerController {
		//this class contains all the variables that will be used to control the player
		//this class is instantiated a number of times equal to the players in the game;
	public:

		PlayerController();
		PlayerController(SDL_Joystick* ref, int index);
		~PlayerController();
		void setTeam(int team); //0 for red, 1 for blue
		void playerControls(GameObject* thisThing);
		GameObject *pObject();
		int inverted();
		int instanceID = NULL;
		//pointer to the joystick reference
		SDL_Joystick *pStick = NULL;
		//taring joysticks properly
		Sint16 TareX = NULL;
		Sint16 TareY = NULL;

		int shotDelay = NULL;

		void tick();

		void weShot();

		glm::vec3 getTeamColor();
	private:
		// Defines several possible input options
		enum Input_Type {
			KEYBOARD,
			CONTROLLER
		};

		Input_Type pInput;

		//defines teams
		enum Team {
			RED,
			BLUE
		};

		Team pTeam;		

		//holds the object that we want to use
		GameObject* PlayerObject = NULL;

		//what number the player is, 0-3 (1-4)
		int PIndex = NULL;

		int invertControls = NULL; //used for the other team. 1 or -1		
	};
		
};
#endif