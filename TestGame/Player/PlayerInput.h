#pragma once
#ifndef PLAYERINPUT_H
#define PLAYERINPUT_H

#include <glm\glm.hpp>
#include <BlueFlameEngine\Engine\InputHandling\InputManager.h>

///HOW TO USE THE NEW JOYSTICK STATE
///
///THE CONTROLLER INPUTS ARE NOW STORED IN controllerState in vector form
///
/// Index 0 is the player the controller refers to (0 to 3) you can mostly ignore this
/// Index 1/2 are left joystick x/y and Index 3/4 are right joystick x/y. These are stored as proper ints rather than bools
/// Index 5 and 6 are the Left and Right Trigger, respectively. 1 for pressed, 0 for not
/// Index 7 onwards are controller buttons. 1 for pressed, 0 for not. We do not look at the xbox key.
/// 7 is A Button
/// 8 is B Button
/// 9 is X Button
/// 10 is Y Button
/// 11 is Left Bumper
/// 12 is Right Bumper
/// 13 is Back Button
/// 14 is Start Button
/// 15 is Left Joystick Push
/// 16 is Right Joystick Push
/// Finally, we save something that isn't a button
/// 17 is Up on the DPad (1 for pressed, 0 for not)
///
///always use NewButtonPress(int i) to check if you're receiving a new input at index i


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

		std::string UpdateJoystickState();
		std::vector<int> controllerState;
		std::vector<int> lastKeyPressed;
		void ParseNetworkInputs(std::string inputs);
		void makeNetworked();
		bool isNetworked();
		inline void setPlayerNum(int num) { playerNum = num; };
		bool NewButtonPress(int i);
		void DebugState();

		void ResetMovementButtons();
		void ResetShootingButtons();

		void PlayFeedback(float strength);

		std::string ReturnJoystickState();

	private:

		SDL_Joystick* joystick = nullptr;
		Sint16 Tare[6];
		glm::vec2 AnyJoystick(int indexA, int indexB);
		int playerNum = 0;
		bool networkedPlayer = false;
		void ResetNetworkedInputs();

		void UpdateLastButtonState();

		///Haptic testing
		SDL_Haptic *haptic;
		


	};
}
#endif
