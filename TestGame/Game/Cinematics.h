#pragma once
#ifndef CINEMATICS_H
#define CINEMATICS_H

#include <BlueFlameEngine\Engine\Timers\Cooldown.h>
#include <BlueFlameEngine\Engine\Camera\Camera.h>
#include <BlueFlameEngine\Engine\Core\Scene.h>
#include <BlueFlameEngine\Engine\Core\SceneManager.h>
#include <BlueFlameEngine\Engine\Rendering\2D\ImageUI.h>
#include <BlueFlameEngine\Engine\Rendering\2D\TextUI.h>

using namespace ENGINE;

namespace GAME {

	class Cinematics {

	public:
		Cinematics();
		~Cinematics();

		// Camera functions
		void CameraMove(Camera* camera, ENGINE::Camera_Movement direction, float yaw, float pitch, float speed, float deltaTime);

		// Intro 
		void PlayIntro(Scene* scene, SceneManager* sceneManager, TextUI* roundText, ImageUI* fadeImage, float deltaTime);
		void SkipIntro();
		void PlayRoundStart();

	private:
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

	};

}

#endif