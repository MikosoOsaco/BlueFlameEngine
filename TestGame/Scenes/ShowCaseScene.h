#pragma once
#ifndef SHOWCASESCENE_H
#define SHOWCASESCENE_H

#include <BlueFlameEngine\Engine\BFEngine.h>
#include <BlueFlameEngine\Engine\Core\Scene.h>
#include <BlueFlameEngine\Engine\Core\Light.h>
#include <BlueFlameEngine\Engine\Core\ResourceManager.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Cube.h> 
#include <BlueFlameEngine\Engine\Rendering\3D\Model.h> 
#include <BlueFlameEngine\Engine\Rendering\3D\Skybox.h> 
#include <BlueFlameEngine\Engine\Rendering\2D\ImageUI.h>
#include <BlueFlameEngine\Engine\Graphics\Shader.h> 
#include "../Player/Player.h"
#include "../Player/WindPlayer.h"
#include "../Player/FirePlayer.h"
#include "../Player/IcePlayer.h"
#include "../Player/EarthPlayer.h"
#include "../Player/LightningPlayer.h"

using namespace ENGINE;

namespace GAME {

	class ShowCaseScene : public Scene
	{
	public:
		ShowCaseScene();
		~ShowCaseScene();

		// Every scene requires these three methods to be implemented
		bool Initialize();
		void Update(const float deltaTime);
		void FixedUpdate(const float deltaTime);
		void HandleEvents(SDL_Event events);
		void HandleStates(const Uint8 *state);

		SceneManager* sceneManager;

	private:
		// Lights
		Light* pointLight;
		Light* dirLight;

		// Player
		Player* player;

		// Stage
		Cube* floor;
		Model* platform;
		Model* bottomRing;

		// shaders
		Shader* defaultShader;
		Shader* skyboxShader;
		Shader* lightShader;

		// shader handles / reference to shader in the renderers shader manager
		ResourceHandle<Shader> defaultShaderHandle;
		ResourceHandle<Shader> skyboxShaderHandle;
		ResourceHandle<Shader> lightShaderHandle;

		ImageUI* fadeImage;

		// Intro Variables
		Cooldown cameraCD;
		Cooldown roundCD;
		float fadeAlpha = 1.0f;
		bool cameraSwitch1 = false;
		bool cameraSwitch2 = false;
		bool cameraSwitch3 = false;
		bool playingIntro = true;
		bool ready = false;
		bool startText = false;
		bool roundStart = false;
		bool playAudio = true;

		// Camera functions
		void CameraMove(ENGINE::Camera_Movement direction, float yaw, float pitch, float speed, int camera);

	};
}
#endif

