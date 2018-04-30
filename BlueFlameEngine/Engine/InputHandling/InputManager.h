#pragma once
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL\SDL.h>
#include <memory>
#include <vector>
#include <iostream>

namespace ENGINE {
	class InputManager {
	public:
		//Instance getter to allow them to get the current version of this class
		static InputManager* GetInstance(); //we're using a singleton class so we only have one input handler
		void initalizeControllers();
		InputManager();
		~InputManager();
		

		SDL_Joystick* getController(SDL_JoystickID id);
		SDL_Joystick* getControllerByIndex(int index);
		SDL_Joystick* getAnyController();
		bool newController(SDL_JoystickID id);
		int connectedControllers = 0;
		void ClearControllers() { spareControllers.clear(); usedControllers.clear(); };

	private:
		
		static std::unique_ptr<InputManager> Input;
		std::vector<SDL_Joystick*> spareControllers;
		std::vector<SDL_JoystickID> usedControllers;
	};
}

#endif
