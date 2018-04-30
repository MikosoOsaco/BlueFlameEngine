#include "PlayerInput.h"
#include <string>

using namespace GAME;

PlayerInput::PlayerInput() {
	ResetNetworkedInputs();
}

PlayerInput::~PlayerInput() {
	SDL_JoystickClose(joystick);
	SDL_HapticClose(haptic);
}

void PlayerInput::AddJoystick(SDL_Joystick* jstick) {

	joystick = jstick;
	for (int i = 0; i < 6; i++) {
		Tare[i] = SDL_JoystickGetAxis(joystick, i) * -1;
	}
}

void PlayerInput::PlayFeedback(float strength) {
	if (haptic == NULL) {
		haptic = SDL_HapticOpenFromJoystick(joystick);
		SDL_HapticRumbleInit(haptic);
	}
	else {
		SDL_HapticRumbleStop(haptic);
		SDL_HapticRumblePlay(haptic, 0.6, 500);
	}
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
		//std::cout << "no joystick!" << std::endl;
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

std::string PlayerInput::UpdateJoystickState() {
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

	for (int i = 0; i < 10; i++) {
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
		controllerState.clear();
		std::string outputs = inputs;
		std::string substringy = outputs.substr(0, 1);
		try
		{
			int xxx = std::stoi(substringy);
			controllerState.push_back(xxx);
		}
		catch (const std::exception&)
		{
			controllerState.push_back(0);
		}
		outputs = outputs.substr(2, 99);

		for (int i = 0; i < 4; i++) {
			std::string substringy = outputs.substr(0, 5);
			try
			{
				int xxx = std::stoi(substringy);
				controllerState.push_back(xxx);
			}
			catch (const std::exception&)
			{
				controllerState.push_back(0);
			}
			outputs = outputs.substr(5, 99);
		}
		for (int i = 0; i < 13; i++) {
			std::string substringy = outputs.substr(0, 1);
			try
			{
				int xxx = std::stoi(substringy);
				controllerState.push_back(xxx);
			}
			catch (const std::exception&)
			{
				controllerState.push_back(0);
			}

			outputs = outputs.substr(1, 99);
		}
		/*for each (int x in controllerState) {
		std::cout << std::to_string(x);
		}
		std::cout << std::endl;*/
	}

}

void PlayerInput::makeNetworked() {
	networkedPlayer = true;
}

bool PlayerInput::isNetworked() {
	return networkedPlayer;
}

void PlayerInput::ResetNetworkedInputs() {
	controllerState.clear();
	lastKeyPressed.clear();
	for (int i = 0; i < 20; i++) {
		controllerState.push_back(0);
		lastKeyPressed.push_back(0);
	}
}

bool PlayerInput::NewButtonPress(int i) {
	if (controllerState.at(i) == lastKeyPressed.at(i)) {
		return false;
	}
	else {
		lastKeyPressed.at(i) = controllerState.at(i);
		UpdateLastButtonState();
		return true;
	}
}

void PlayerInput::UpdateLastButtonState() {
	for (int i = 0; i < controllerState.size(); i++) {
		lastKeyPressed.at(i) = controllerState.at(i);
	}
}

void PlayerInput::DebugState() {
	for each (int i in controllerState) {
		printf(std::to_string(i).c_str());
	}
	printf("\n");
}

void GAME::PlayerInput::ResetMovementButtons()
{
	for (int i = 1; i < 4; i++) {
		controllerState.at(i) = 0;
		lastKeyPressed.at(i) = 0;
	}
}

void GAME::PlayerInput::ResetShootingButtons()
{
	controllerState.at(5) = 0;
	lastKeyPressed.at(5) = 0;
	controllerState.at(6) = 0;
	lastKeyPressed.at(6) = 0;
	controllerState.at(11) = 0;
	lastKeyPressed.at(11) = 0;
	controllerState.at(12) = 0;
	lastKeyPressed.at(12) = 0;

}

std::string PlayerInput::ReturnJoystickState() {
	std::string joystickChunk = std::to_string(playerNum) + "|"; //0 is player num

	for each (int i in controllerState) {
		joystickChunk += std::to_string(i);
	}

	return joystickChunk;
}