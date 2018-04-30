#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include "PlayerInput.h"
#include <BlueFlameEngine\Engine\Rendering\2D\ImageUI.h>
#include <BlueFlameEngine\Engine\BFEngine.h>
#include "../Game/PlayerPortrait.h"

using namespace ENGINE;

namespace GAME {

	class Crosshair
	{
		//friend ImageUI;

	public:
		Crosshair();
		Crosshair(float x, float y);
		Crosshair(const char* filepath, float x, float y);
		~Crosshair();
		
		ImageUI* GetImage();

		void Update(const float deltaTime);
		void HandleEvents(SDL_Event events);
		void HandleStates(const Uint8 *state);

		void SetCanMove(bool canM) { canMove = canM; };
		void SetEnabled(bool enable) { enabled = enable; };
		void SetVisible(bool isVis) { if (isVis) { crosshair->SetAlpha(1.0f); } else { crosshair->SetAlpha(0.0f); } };
		void SetCharSelected(PlayerPortrait::CHARTYPE charSelect) { charSelected = charSelect; };

		PlayerInput* GetPlayerInput() { return playerInput; };
		PlayerPortrait::CHARTYPE GetCharSelected() { return charSelected; };
		bool CanMove() { return canMove; };
		bool GetEnabled() { return enabled; };

	private:
		// Player Input 
		PlayerInput* playerInput;
		PlayerPortrait::CHARTYPE charSelected = PlayerPortrait::CHARTYPE::NONE;

		ImageUI* crosshair;

		// Check to see if player can move
		bool canMove;

		bool enabled = false;
		
	};

}
#endif // !CROSSHAIR_H
