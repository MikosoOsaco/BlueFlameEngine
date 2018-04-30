#ifndef CHARACTERSELECTSCENE_H
#define CHARACTERSELECTSCENE_H

#include <BlueFlameEngine\Engine\BFEngine.h>
#include <BlueFlameEngine\Engine\Core\Scene.h>
#include <BlueFlameEngine\Engine\Core\ResourceManager.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Skybox.h> 
#include <BlueFlameEngine\Engine\Graphics\Shader.h> 
#include <BlueFlameEngine\Engine\Timers\Cooldown.h>
#include <BlueFlameEngine\Engine\Rendering\2D\TextUI.h>
#include <BlueFlameEngine\Engine\Rendering\2D\ImageUI.h>
#include <BlueFlameEngine\Engine\Rendering\2D\ButtonUI.h>
#include <BlueFlameEngine\Engine\Rendering\2D\SliderUI.h>
#include <BlueFlameEngine\Engine\Audio\Announcer.h>
#include "DemoScene.h"
#include "TvTGameScene.h"
#include "..\Player\Crosshair.h"
#include "..\Game\PlayerPortrait.h"

using namespace ENGINE;

namespace GAME {

	class CharacterSelectScene : public Scene
	{
	public:
		CharacterSelectScene();
		~CharacterSelectScene();

		// Every scene requires these three methods to be implemented
		bool Initialize();
		void FixedUpdate(const float deltaTime);
		void Update(const float deltaTime);
		void HandleEvents(SDL_Event events);
		void HandleStates(const Uint8 *state);

		SceneManager* sceneManager;
	private:

		void ControllerPressStart(SDL_JoystickID jID);
		void SetUpPlayerPorts(PlayerPortrait* tempP);

		//network
		void HandleNetworkEvents(int i);

		// shaders
		Shader* skyboxShader;

		//Audio
		Announcer* announcer;
		Music* bgm;

		// shader handles / reference to shader in the renderers shader manager
		ResourceHandle<Shader> skyboxShaderHandle;

		// UI
		TextUI* titleText;
		Crosshair* crosshair1;
		Crosshair* crosshair2;
		Crosshair* crosshair3;
		Crosshair* crosshair4;
		std::vector<Crosshair*> crosshairList;
		std::vector<SDL_JoystickID> usedCrosshairList;


		float portBackRot = 0.0f;
		ImageUI* player1Back;
		ImageUI* player2Back;
		ImageUI* player3Back;
		ImageUI* player4Back;

		PlayerPortrait* player1Port;
		PlayerPortrait* player2Port;
		PlayerPortrait* player3Port;
		PlayerPortrait* player4Port;
		std::vector<PlayerPortrait*> portraitList;

		ImageUI* blueJoiner;
		ImageUI* redJoiner;

		ButtonUI* champKalOrr;
		ButtonUI* champAlexTrix;
		ButtonUI* champFlintDamascus;
		ButtonUI* champJackCole;
		ButtonUI* champOkiCaeli;

		ImageUI* champMystery;
		ImageUI* champMystery2;
		ImageUI* champMystery3;
		ImageUI* champMystery4;

		ImageUI* start;

		// Loading Screen
		ImageUI* loadingScreen;
		Cooldown loadingCD;
		bool loading = false;
		bool ready = false;

		float cameraTimer = 0;
		void MoveToGameScene();
	};
}
#endif

