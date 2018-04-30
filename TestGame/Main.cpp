#include <BlueFlameEngine\Engine\BFEngine.h>
#include "Scenes\TestScene.h"
#include "Scenes\DemoScene.h"
#include "Scenes\MenuScene.h"

using namespace ENGINE;
using namespace GAME;

int main(int argc, char* args[]) {	

	// Set Window Settings
	BFEngine::GetInstance()->SetWindowName("Blue Flame Engine");
	BFEngine::GetInstance()->SetWindowDimensions(1280, 720);

	//Start Engine
	if (BFEngine::GetInstance()->Initialize()) {
		// Add scenes
		BFEngine::GetInstance()->GetSceneManager()->AddScene(new MenuScene);
		BFEngine::GetInstance()->GetSceneManager()->AddScene(new MenuSelectScene);
		BFEngine::GetInstance()->GetSceneManager()->StartScene();
		//Game Loop
		BFEngine::GetInstance()->Run();
	}

	return 0;
}