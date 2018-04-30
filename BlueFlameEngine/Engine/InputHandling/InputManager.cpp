#include "InputManager.h"

using namespace ENGINE;

std::unique_ptr<InputManager> InputManager::Input(nullptr);


InputManager::InputManager() {
	spareControllers.clear();
}

InputManager::~InputManager() {
	spareControllers.clear();
}

InputManager* InputManager::GetInstance() {
	if (Input.get() == nullptr) {
		Input.reset(new InputManager);
	}
	return Input.get();
}

void InputManager::initalizeControllers() {
	spareControllers.clear();
	connectedControllers = SDL_NumJoysticks();
	if (connectedControllers == 0) {
		std::cout << "No controllers detected" << std::endl;
	}
	else { //pushes the controllers into the vector
		std::cout << "Controllers detected: " << connectedControllers << std::endl;
		for (int i = 0; i < connectedControllers; i++) {
			SDL_Joystick* j = SDL_JoystickOpen(i);
			std::cout << SDL_JoystickNumHats(j) << std::endl;
			spareControllers.push_back(j);
			Sint16 test = SDL_JoystickGetAxis(j, 0);
			//std::cout << SDL_GetError() << std::endl;
		}
	}
}

SDL_Joystick* InputManager::getController(SDL_JoystickID id) {
	//check that the controller isn't already in use
	if (!newController(id)) {
		return nullptr;
	}

	//push back the reference to the controller
	for each (SDL_Joystick* pInput in spareControllers) {
		if (SDL_JoystickInstanceID(pInput) == id) {
			usedControllers.push_back(id);
			return pInput;
		}
	}
	return nullptr;
}

SDL_Joystick* InputManager::getControllerByIndex(int index) {
	//check that the controller isn't already in use
	if (index > usedControllers.size() - 1) {
		return nullptr;
	}
	return getController(usedControllers.at(index));
}

SDL_Joystick* InputManager::getAnyController() {
	//check that the controller isn't already in use
	if (spareControllers.size() > usedControllers.size()) {
		SDL_Joystick* temp = spareControllers.at(usedControllers.size());
		usedControllers.push_back(SDL_JoystickInstanceID(temp));
		return temp;
	}
	return nullptr;
}

bool InputManager::newController(SDL_JoystickID id) {
	//check that the controller isn't already in use
	for each (SDL_JoystickID used in usedControllers) {
		if (used == id) {
			return false;
		}
	}
	return true;
}