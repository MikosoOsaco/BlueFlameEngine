#include "Crosshair.h"

using namespace GAME;

Crosshair::Crosshair()
{
	crosshair = new ImageUI();
	crosshair->SetImage("Resources/Textures/CharacterSelectScreen/crosshair.png");
	crosshair->SetPosition(940, 600);
	//crosshair->SetHeight(250);
	//crosshair->SetWidth(250);

	// Make player input
	playerInput = new PlayerInput();
	canMove = false;
}

Crosshair::Crosshair(float x, float y)
{
	crosshair = new ImageUI();
	crosshair->SetImage("Resources/Textures/CharacterSelectScreen/crosshair.png");
	crosshair->SetPosition(x, y);

	// Make player input
	playerInput = new PlayerInput();
	canMove = false;
}

Crosshair::Crosshair(const char* filepath, float x, float y)
{
	crosshair = new ImageUI();
	crosshair->SetImage(filepath);
	crosshair->SetPosition(x, y);

	// Make player input
	playerInput = new PlayerInput();
	canMove = false;
}

Crosshair::~Crosshair()
{

}

ImageUI* Crosshair::GetImage()
{
	return crosshair;
}

void Crosshair::Update(const float deltaTime)
{
	if (!Settings::getInstance()->networkedGame && !Settings::getInstance()->spectatorMode) {
		playerInput->ParseNetworkInputs(playerInput->UpdateJoystickState());
	}
	
	//playerInput->DebugState();

	// If player can move update movement 
	if (canMove) {
		if (playerInput->CheckForController() || playerInput->isNetworked()) {//if they have a controller
			glm::vec2 mods = glm::vec2(playerInput->controllerState.at(1), playerInput->controllerState.at(2));

			if (mods.x > 0.01f) {
				crosshair->SetPosition(crosshair->GetPosition().x + deltaTime * 800, crosshair->GetPosition().y);
			}
			if (mods.x < -0.01f) {
				crosshair->SetPosition(crosshair->GetPosition().x - deltaTime * 800, crosshair->GetPosition().y);
			}
			if (mods.y > 0.01f) {
				crosshair->SetPosition(crosshair->GetPosition().x, crosshair->GetPosition().y + deltaTime * 800);
			}
			if (mods.y < -0.01f) {
				crosshair->SetPosition(crosshair->GetPosition().x, crosshair->GetPosition().y - deltaTime * 800);
			}
		}
	}

	if (crosshair->GetPosition().x < 0) {
		crosshair->SetPosition(0, crosshair->GetPosition().y);
	}

	if (crosshair->GetPosition().y < 0) {
		crosshair->SetPosition(crosshair->GetPosition().x, 0);
	}

	if (crosshair->GetPosition().x > BFEngine::GetInstance()->GetSceneManager()->GetScreenWidth()) {
		crosshair->SetPosition(BFEngine::GetInstance()->GetSceneManager()->GetScreenWidth(), crosshair->GetPosition().y);
	}

	if (crosshair->GetPosition().y > BFEngine::GetInstance()->GetSceneManager()->GetScreenHeight()) {
		crosshair->SetPosition(crosshair->GetPosition().x, BFEngine::GetInstance()->GetSceneManager()->GetScreenHeight());
	}
}

void Crosshair::HandleEvents(SDL_Event events)
{

}

void Crosshair::HandleStates(const Uint8 *state)
{

}
