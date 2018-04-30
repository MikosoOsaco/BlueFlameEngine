#pragma once
#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <BlueFlameEngine\Engine\BFEngine.h>
#include <BlueFlameEngine\Engine\Core\Scene.h>
#include <BlueFlameEngine\Engine\Core\Light.h>
#include <BlueFlameEngine\Engine\Core\ResourceManager.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Skybox.h> 
#include <BlueFlameEngine\Engine\Graphics\Shader.h> 
#include <BlueFlameEngine\Engine\Rendering\2D\TextUI.h>
#include <BlueFlameEngine\Engine\Rendering\2D\ImageUI.h>
#include <BlueFlameEngine\Engine\Rendering\2D\ButtonUI.h>
#include "DemoScene.h"
#include "TestScene.h"
#include "MenuSelectScene.h"

using namespace ENGINE;

namespace GAME {

	class MenuScene : public Scene
	{
	public:
		MenuScene();
		~MenuScene();

		// Every scene requires these three methods to be implemented
		bool Initialize();
		void Update(const float deltaTime);
		void FixedUpdate(const float deltaTime);
		void HandleEvents(SDL_Event events);
		void HandleStates(const Uint8 *state);

		SceneManager* sceneManager;
	private:
		// shaders
		Shader* skyboxShader;

		// shader handles / reference to shader in the renderers shader manager
		ResourceHandle<Shader> skyboxShaderHandle;

		// UI
		TextUI* titleText;
		TextUI* startText;
		TextUI* creditText;

		float z = 0;
	};
}
#endif

