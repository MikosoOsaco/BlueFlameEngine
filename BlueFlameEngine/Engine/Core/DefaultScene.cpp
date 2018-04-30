#include "DefaultScene.h"

using namespace ENGINE;

DefaultScene::DefaultScene()
{
}

DefaultScene::~DefaultScene()
{
}

bool DefaultScene::Initialize() 
{
	text = new TextUI;
	text->SetText("No Scenes Loaded");
	text->SetColour(1.0f, 1.0f, 1.0f);
	text->SetPosition(1920 / 2 - text->GetLength() / 2, 1080 / 2);

	AddUIObject(text);

	return true;
}
void DefaultScene::Update(const float deltaTime)
{
	
}

void DefaultScene::FixedUpdate(const float deltaTime)
{

}

void DefaultScene::HandleEvents(SDL_Event events)
{

}

void DefaultScene::HandleStates(const Uint8 *state) 
{

}