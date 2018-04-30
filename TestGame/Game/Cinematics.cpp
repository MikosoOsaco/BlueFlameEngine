#include "Cinematics.h"

using namespace GAME;

Cinematics::Cinematics() {

}

Cinematics::~Cinematics() {

}

void Cinematics::CameraMove(Camera* camera, ENGINE::Camera_Movement direction, float yaw, float pitch, float speed, float deltaTime)
{
	camera->SetRotationX(pitch);
	camera->SetRotationY(yaw);
	camera->ProcessKeyboard(direction, speed * deltaTime);
}

void Cinematics::PlayIntro(Scene* scene, SceneManager* sceneManager, TextUI* roundText, ImageUI* fadeImage, float deltaTime)
{
	if (cameraCD.secondsLeft() > 3) {
		CameraMove(scene->GetCameraList()[0], RIGHT, -145.0f, 11.0f, 0.5f, deltaTime);
	}
	else if (cameraCD.secondsLeft() > 1.5) {
		if (!cameraSwitch1) {
			scene->GetCameraList()[0]->Position = glm::vec3(-5.0f, 15.0f, 1.0f);
			cameraSwitch1 = true;
		}
		CameraMove(scene->GetCameraList()[0], LEFT, -33.0f, -56.0f, 0.5f, deltaTime);
	}
	else if (cameraCD.secondsLeft() > 0) {
		if (!cameraSwitch2) {
			scene->GetCameraList()[0]->Position = glm::vec3(6.0f, 8.0f, -12.0f);
			scene->GetCameraList()[0]->SetRotationY(-220.0f);
			scene->GetCameraList()[0]->SetRotationX(-29.0f);
			fadeAlpha = 1.0f;
			cameraSwitch2 = true;
		}
		scene->GetCameraList()[0]->SetRotationY(scene->GetCameraList()[0]->Yaw -= deltaTime * 10);
	}
	else if (cameraCD.checkOffCD()) {
		if (!cameraSwitch3) {
			scene->GetCameraList()[0]->Position = glm::vec3(0.0f, 7.5f, 9.5f);
			scene->GetCameraList()[0]->SetRotationY(-90.0f);
			scene->GetCameraList()[0]->SetRotationX(-25.0f);
			scene->GetCameraList()[1]->Position = glm::vec3(0.0f, 7.5f, -9.5f);
			scene->GetCameraList()[1]->SetRotationY(90.0f);
			scene->GetCameraList()[1]->SetRotationX(-25.0f);

			sceneManager->EnableSplitscreen(true);

			cameraSwitch3 = true;
		}

		scene->GetCameraList()[0]->SetRotationX(scene->GetCameraList()[0]->Pitch -= deltaTime * 10);
		scene->GetCameraList()[1]->SetRotationX(scene->GetCameraList()[0]->Pitch);

		if (scene->GetCameraList()[0]->Pitch <= -45.0f) {
			scene->GetCameraList()[0]->Pitch = -45.0f;
			scene->GetCameraList()[1]->Pitch = -45.0f;
			fadeImage->SetVisible(false);
			roundText->SetVisible(true);
			roundCD.startCD();
			fadeAlpha = 1.0f;
			playingIntro = false;
		}
	}

	fadeAlpha -= deltaTime;
	fadeImage->SetAlpha(fadeAlpha);
	if (fadeAlpha <= 0) {
		fadeAlpha = 0;
	}
}
/*
void Cinematics::SkipIntro()
{
	if (playingIntro) {
		playingIntro = false;

		cameraList[0]->Position = glm::vec3(0.0f, 7.5f, 9.5f);
		cameraList[0]->SetRotationY(-90.0f);
		cameraList[0]->SetRotationX(-45.0f);
		cameraList[1]->Position = glm::vec3(0.0f, 7.5f, -9.5f);
		cameraList[1]->SetRotationY(90.0f);
		cameraList[1]->SetRotationX(-45.0f);
		fadeImage->SetVisible(false);

		sceneManager->EnableSplitscreen(true);

		fadeAlpha = 1.0f;

		roundText->SetVisible(true);

		roundCD.startCD();
	}
}

void Cinematics::PlayRoundStart()
{
	if (!ready) {
		roundText->SetText("Ready?");
		roundText->SetSize(roundText->GetSize() + deltaTime * 5);
		if (roundText->GetSize() >= 1.0f) {
			roundText->SetSize(1.0f);
		}
		roundText->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundText->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
		if (roundCD.checkOffCD()) {
			roundText->SetSize(10.0f);
			roundText->SetText("Round Start");
			ready = true;
		}
	}
	if (ready) {
		roundText->SetSize(roundText->GetSize() - deltaTime * 40);
		roundText->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundText->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
		if (roundText->GetSize() < 1.0f) {
			roundText->SetSize(1.0f);
			roundText->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundText->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
			roundTextFade->SetVisible(true);
			roundCD.setNewDuration(1.0);
			roundCD.startCD();
			roundStart = true;
		}

		if (roundCD.secondsLeft() > 0.8) {
			roundTextFade->SetSize(roundTextFade->GetSize() + deltaTime);
			roundTextFade->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundTextFade->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
		}
		else if (roundCD.secondsLeft() > 0.6) {
			roundTextFade->SetSize(roundTextFade->GetSize() + deltaTime * 10);
			roundTextFade->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundTextFade->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
		}
		else if (roundCD.secondsLeft() > 0.4) {
			roundTextFade->SetSize(roundTextFade->GetSize() + deltaTime * 5);
			roundTextFade->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundTextFade->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
		}
		if (roundCD.secondsLeft() > 0) {
			fadeAlpha -= deltaTime;
			if (fadeAlpha <= 0) {
				fadeAlpha = 0;
			}
			roundTextFade->SetAlpha(fadeAlpha / 2);
			roundText->SetAlpha(fadeAlpha);
		}
		if (fadeAlpha == 0) {
			roundText->SetVisible(false);
			roundTextFade->SetVisible(false);
			startText = true;
		}
	}
}
*/