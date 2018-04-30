#pragma once
#ifndef DEMOSCENE_H
#define DEMOSCENE_H

#include <BlueFlameEngine\Engine\BFEngine.h>
#include <BlueFlameEngine\Engine\Core\Scene.h>
#include <BlueFlameEngine\Engine\Core\Light.h>
#include <BlueFlameEngine\Engine\Core\ResourceManager.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Cube.h> 
#include <BlueFlameEngine\Engine\Rendering\3D\Projectile.h> 
#include <BlueFlameEngine\Engine\Rendering\3D\Skybox.h> 
#include <BlueFlameEngine\Engine\Graphics\Shader.h> 
#include <BlueFlameEngine\Engine\Math\PhysicsEngine.h>
#include <BlueFlameEngine\Engine\Rendering\2D\TextUI.h>
#include "../Game/ProjectileManager.h"
#include "../Player/Player.h"
#include "../Player/WindPlayer.h"
#include "../Player/IcePlayer.h"
#include "../Player/FirePlayer.h"
#include "../Player/EarthPlayer.h"
#include "../Player/LightningPlayer.h"

using namespace ENGINE;

namespace GAME {

	class DemoScene : public Scene
	{
	public:
		DemoScene();
		~DemoScene();

		// Every scene requires these five methods to be implemented
		bool Initialize();
		void Update(const float deltaTime);
		void FixedUpdate(const float deltaTime);
		void HandleEvents(SDL_Event events);
		void HandleStates(const Uint8 *state);

	private:
		// Lights
		Light* pointLight;
		Light* dirLight;

		// Objects
		Player* player1;  //P1
		Player* player2;  //P2
		Player* player3;  //P3
		Player* player4;  //P4

		Cube* floor;
		Cube* wall;
		Cube* wall1;
		Cube* middleWall;

		// shaders
		Shader* defaultShader;
		Shader* skyboxShader;

		// shader handles / reference to shader in the renderers shader manager
		ResourceHandle<Shader> defaultShaderHandle;
		ResourceHandle<Shader> skyboxShaderHandle;

		// scene manager
		SceneManager* sceneManager;

		// Projectile Manager
		ProjectileManager* projectileManager;

		// Player list
		vector<Player*> playerList;

		// other variables
		float moveSpeed = 2;
		float deltaTime;
	};
}
#endif

