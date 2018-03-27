#include "PlayerInput.h"
#include <string>

using namespace GAME;

PlayerInput::PlayerInput() {
	ResetNetworkedInputs();
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
		if (SDL_JoystickNumHats(joystick) < 0) {
			printf(SDL_GetError());
			int x = 0;
		}
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
	if (x_move > -500 && joystick != NULL) {
		return true;
	}
	else {
		return false;
	}
}

bool PlayerInput::RightTriggerPressed() {
	Sint32 x_move = SDL_JoystickGetAxis(joystick, 5);

	if (x_move > -500 && joystick != NULL) {
		return true;
	}
	else {
		return false;
	}
}

glm::vec2 PlayerInput::AnyJoystick(int indexA, int indexB) {
	if (joystick != NULL) {

	}
	else {
		return glm::vec2(0, 0);
	}
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

std::string PlayerInput::ReturnJoystickStateForNetworking() {
	std::string networkChunk = std::to_string(playerNum) + "|"; //0 is player num
	std::string temp = "";

	glm::vec2 primaryStick = LeftJoystick();
	temp = std::to_string(primaryStick.x); //1 is primary stick x
	networkChunk += temp.substr(0, 5);
	temp = std::to_string(primaryStick.y);
	networkChunk += temp.substr(0, 5); //2 is primary stick y

	glm::vec2 secondaryStick = RightJoystick();
	temp = std::to_string(secondaryStick.x); //3 is secondary stick x
	networkChunk += temp.substr(0, 5);
	temp = std::to_string(secondaryStick.y); //4 is secondary stick y
	networkChunk += temp.substr(0, 5);

	bool ltp = LeftTriggerPressed();
	networkChunk += std::to_string(ltp); //5 is left trigger state

	bool rtp = RightTriggerPressed();
	networkChunk += std::to_string(rtp); //6 is right trigger state

	for (int i = 0; i < 10; i++) { //7 is a, 8 is b, 9 is x, 10 is y, 11 is left bumper, 12 is right bumper, 13 is left joystick push, 14 is right joystick push, 15 is start, 16 is back, 17 is select
		int bState = SDL_JoystickGetButton(joystick, i);
		networkChunk += std::to_string(bState);
	}

	//send the position of the hat because fuuuuuuuuuuuuck me
	int bState;
	if (SDL_JoystickGetHat(joystick, 0) & SDL_HAT_UP) {
		bState = 1;
	}
	else {
		bState = 0;
	}
	networkChunk += std::to_string(bState);

	return networkChunk;
}

void PlayerInput::ParseNetworkInputs(std::string inputs) {

	if (inputs.size() >= 35) {
		networkedJoystickInputs.clear();
		std::string outputs = inputs;
		std::string substringy = outputs.substr(0, 1);
		try
		{
			int xxx = std::stoi(substringy);
			networkedJoystickInputs.push_back(xxx);
		}
		catch (const std::exception&)
		{
			networkedJoystickInputs.push_back(0);
		}
		outputs = outputs.substr(2, 99);

		for (int i = 0; i < 4; i++) {
			std::string substringy = outputs.substr(0, 5);
			try
			{
				int xxx = std::stoi(substringy);
				networkedJoystickInputs.push_back(xxx);
			}
			catch (const std::exception&)
			{
				networkedJoystickInputs.push_back(0);
			}
			outputs = outputs.substr(5, 99);
		}
		for (int i = 0; i < 13; i++) {
			std::string substringy = outputs.substr(0, 1);
			try
			{
				int xxx = std::stoi(substringy);
				networkedJoystickInputs.push_back(xxx);
			}
			catch (const std::exception&)
			{
				networkedJoystickInputs.push_back(0);
			}

			outputs = outputs.substr(1, 99);
		}
		for each (int x in networkedJoystickInputs) {
			std::cout << std::to_string(x);
		}
		std::cout << std::endl;
	}

}

void PlayerInput::makeNetworked() {
	networkedPlayer = true;
}

bool PlayerInput::isNetworked() {
	return networkedPlayer;
}

void PlayerInput::ResetNetworkedInputs() {
	networkedJoystickInputs.clear();
	for (int i = 0; i < 20; i++) {
		networkedJoystickInputs.push_back(0);
		lastNetworkKeyPressed.push_back(0);
	}
}