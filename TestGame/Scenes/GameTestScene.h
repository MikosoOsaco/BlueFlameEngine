#pragma once
#ifndef GAMETESTSCENE_H
#define GAMETESTSCENE_H

#include <BlueFlameEngine\Engine\BFEngine.h>
#include <BlueFlameEngine\Engine\Core\Scene.h>
#include <BlueFlameEngine\Engine\Core\Light.h>
#include <BlueFlameEngine\Engine\Core\ResourceManager.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Cube.h> 
#include <BlueFlameEngine\Engine\Rendering\3D\Model.h> 
#include <BlueFlameEngine\Engine\Rendering\3D\Skybox.h> 
#include <BlueFlameEngine\Engine\Graphics\Shader.h> 
#include <BlueFlameEngine\Engine\Graphics\ParticleSystem.h>
#include <BlueFlameEngine\Engine\Math\PhysicsEngine.h>
#include <BlueFlameEngine\Engine\Audio\Music.h>
#include <BlueFlameEngine\Engine\Audio\SoundEffect.h>
#include "..\Player\Player.h"
#include "../Player/WindPlayer.h"
#include "DemoScene.h"
#include "TestScene.h"

using namespace ENGINE;

namespace GAME {

	class GameTestScene : public Scene
	{
	public:
		GameTestScene();
		~GameTestScene();

		// Every scene requires these three methods to be implemented
		bool Initialize();
		void Update(const float deltaTime);
		void FixedUpdate(const float deltaTime);
		void HandleEvents(SDL_Event events);
		void HandleStates(const Uint8 *state);

		SceneManager* sceneManager;

	private:
		// Particle
		ParticleSystem* particle;

		// Lights
		Light* pointLight;
		Light* dirLight;

		// Player
		WindPlayer* player;
		Cube* floor;

		// shaders
		Shader* defaultShader;
		Shader* skyboxShader;
		Shader* lightShader;

		// shader handles / reference to shader in the renderers shader manager
		ResourceHandle<Shader> defaultShaderHandle;
		ResourceHandle<Shader> skyboxShaderHandle;
		ResourceHandle<Shader> lightShaderHandle;

		// other variables
		float moveSpeed = 2;
		bool fire = true;
		float timer = 0.3f;

		// Sound
		Music* bgm;
		SoundEffect* sEffect;
	};
}
#endif

