#include "TvTGameScene.h"

using namespace GAME;

TvTGameScene::TvTGameScene()
{
}

TvTGameScene::~TvTGameScene()
{
	bgm->Stop();
}

bool TvTGameScene::Initialize()
{
	// Make reference to the scene manager
	sceneManager = BFEngine::GetInstance()->GetSceneManager();

	// Set screen options
	sceneManager->EnableSplitscreen(false);
	sceneManager->ShowFPS(false);
	sceneManager->CaptureMouse(true);

	// Set the options for the first camera
	cameraList[0]->Position = glm::vec3(7.0f, 0.7f, 7.0f);

	// Load shaders
	defaultShader = new Shader("Shaders/model.vs", "Shaders/model.fs");
	skyboxShader = new Shader("Shaders/skybox.vs", "Shaders/skybox.fs");
	lightShader = new Shader("Shaders/lamp.vs", "Shaders/lamp.fs");

	// Put shaders into shader manager
	defaultShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("default"), defaultShader);
	skyboxShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("skybox"), skyboxShader);
	lightShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("lamp"), lightShader);

	// Music
	bgm = new Music();
	if (!bgm->Load("Resources/Audio/20XX Theme Battle.mp3")) {
		std::cout << "BGM could not load" << std::endl;
	}
	bgm->SetVolume(0);

	// Make skybox, load its textures, set properties, and give to the renderer
	skybox = new Skybox();
	std::vector<char*> faces;
	faces.push_back("Resources/Textures/Skyboxes/BlueMist/right.png");
	faces.push_back("Resources/Textures/Skyboxes/BlueMist/left.png");
	faces.push_back("Resources/Textures/Skyboxes/BlueMist/top.png");
	faces.push_back("Resources/Textures/Skyboxes/BlueMist/bottom.png");
	faces.push_back("Resources/Textures/Skyboxes/BlueMist/back.png");
	faces.push_back("Resources/Textures/Skyboxes/BlueMist/front.png");
	skybox->LoadTextures(faces);
	skybox->SetShader(skyboxShaderHandle);

	replayText = new TextUI();
	replayText->SetFont("Resources/Fonts/ka1.ttf");
	replayText->SetText("PLAYING REPLAY");
	replayText->SetColour(1.0, 1.0f, 1.0f);
	replayText->SetSize(0.5f);
	replayText->SetSpacing(9.0f);
	replayText->SetPosition(50, 80);

	spectatorText = new TextUI();
	spectatorText->SetFont("Resources/Fonts/ka1.ttf");
	spectatorText->SetText("Spectator Mode");
	spectatorText->SetColour(1.0, 1.0f, 1.0f);
	spectatorText->SetSize(0.5f);
	spectatorText->SetSpacing(9.0f);
	spectatorText->SetPosition(50, 80);

	CPUText = new TextUI();
	CPUText->SetFont("Resources/Fonts/ka1.ttf");
	CPUText->SetText("COMPUTER MATCH");
	CPUText->SetColour(1.0, 1.0f, 1.0f);
	CPUText->SetSize(0.5f);
	CPUText->SetSpacing(9.0f);
	CPUText->SetPosition(40, 100);

	if (sceneManager->justCPUS) {
		CPUText->SetVisible(true);
		AddUIObject(CPUText);
	}

	// UI
	fadeImage = new ImageUI();
	fadeImage->SetImage("Resources/Textures/blackFILL.png");
	fadeImage->SetPosition(sceneManager->GetScreenWidth() / 2, sceneManager->GetScreenHeight() / 2);
	fadeImage->SetScale(4.0f, 11.0f);

	// UI
	roundText = new TextUI();
	roundText->SetFont("Resources/Fonts/ka1.ttf");
	roundText->SetText("Ready?");
	roundText->SetColour(1.0f, 0.5f, 0.0f);
	roundText->SetSize(0.0f);
	roundText->SetSpacing(9.0f);
	roundText->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundText->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
	roundText->SetVisible(false);

	// UI
	roundTextFade = new TextUI();
	roundTextFade->SetFont("Resources/Fonts/ka1.ttf");
	roundTextFade->SetText("Round Start");
	roundTextFade->SetColour(1.0f, 0.5f, 0.0f);
	roundTextFade->SetSize(1.0f);
	roundTextFade->SetSpacing(9.0f);
	roundTextFade->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundText->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
	roundTextFade->SetVisible(false);

	// Make Projectile Manager
	projectileManager = new ProjectileManager();

	// Make Players
	SetUpPlayers();

	// Make Arena
	SetUpArena();

	// Lights
	//
	// Make point light
	//
	pointLight = new Light();
	pointLight->SetShader(defaultShaderHandle);
	pointLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	pointLight->renderComponent->SetColour(1.0f, 1.0f, 1.0f);
	pointLight->renderComponent->CanRender(false);
	pointLight->canRender = false;
	pointLight->SetWorldPosition(-6.25f, -0.5f, 6.5f);
	pointLight->SetWorldScale(0.5f);
	pointLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	pointLight->lightComponent->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));
	//
	
	//
	pointLight2 = new Light();
	pointLight2->SetShader(lightShaderHandle);
	pointLight2->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	pointLight2->renderComponent->SetColour(1.0f, 1.0f, 1.0f);
	pointLight2->renderComponent->CanRender(false);
	pointLight2->canRender = false;
	pointLight2->SetWorldPosition(6.25f, -0.5f, 6.5f);
	pointLight2->SetWorldScale(0.5f);
	pointLight2->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	pointLight2->lightComponent->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));
	//

	//
	pointLight3 = new Light();
	pointLight3->SetShader(lightShaderHandle);
	pointLight3->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	pointLight3->renderComponent->SetColour(1.0f, 1.0f, 1.0f);
	pointLight3->renderComponent->CanRender(false);
	pointLight3->canRender = false;
	pointLight3->SetWorldPosition(6.25f, -0.5f, -6.5f);
	pointLight3->SetWorldScale(0.5f);
	pointLight3->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	pointLight3->lightComponent->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));
	//

	//
	pointLight4 = new Light();
	pointLight4->SetShader(lightShaderHandle);
	pointLight4->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	pointLight4->renderComponent->SetColour(1.0f, 1.0f, 1.0f);
	pointLight4->renderComponent->CanRender(false);
	pointLight4->canRender = false;
	pointLight4->SetWorldPosition(-6.25f, -0.5f, -6.5f);
	pointLight4->SetWorldScale(0.5f);
	pointLight4->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	pointLight4->lightComponent->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));
	//

	//
	blueLight = new Light();
	blueLight->SetShader(lightShaderHandle);
	blueLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	blueLight->renderComponent->SetColour(0.0f, 0.0f, 1.0f);
	blueLight->SetWorldPosition(0.0f, -2.95f, 6.0f);
	blueLight->SetWorldScale(11.0f, 5.0f, 0.1f);
	blueLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	blueLight->lightComponent->SetColour(glm::vec3(0.0f, 0.0f, 1.0f));
	//

	//
	redLight = new Light();
	redLight->SetShader(lightShaderHandle);
	redLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	redLight->renderComponent->SetColour(1.0f, 0.0f, 0.0f);
	redLight->SetWorldPosition(0.0f, -2.95f, -6.0f);
	redLight->SetWorldScale(11.0f, 5.0f, 0.1f);
	redLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	redLight->lightComponent->SetColour(glm::vec3(1.0f, 0.0f, 0.0f));
	//

	//
	middleLight = new Light();
	middleLight->SetShader(lightShaderHandle);
	middleLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	middleLight->renderComponent->SetColour(0.0f, 0.0f, 0.0f);
	middleLight->SetWorldPosition(0.0f, -2.95f, 0.0f);
	middleLight->SetWorldScale(11.0f, 5.0f, 0.1f);
	middleLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	middleLight->lightComponent->SetColour(glm::vec3(0.0f, 0.0f, 0.0f));
	//
	
	// Make directional light
	dirLight = new Light(LightComponent::Light_Type::DIRECTIONAL);
	dirLight->lightComponent->SetDirection(glm::vec3(1.0f, -1.0f, 0.0f));
	dirLight->lightComponent->SetColour(glm::vec3(0.5f, 0.5f, 0.5f));
	//

	// fireworks
	particle1 = new ParticleSystem(sceneManager->GetRenderer()->GetShaderManager());
	particle1->SetWorldPosition(-20.0f, 0.0f, 0.0f);
	// fireworks
	particle2 = new ParticleSystem(sceneManager->GetRenderer()->GetShaderManager());
	particle2->SetWorldPosition(20.0f, 0.0f, 0.0f);
	// fireworks
	particle3 = new ParticleSystem(sceneManager->GetRenderer()->GetShaderManager());
	particle3->SetWorldPosition(0.0f, 0.0f, 20.0f);
	// fireworks
	particle4 = new ParticleSystem(sceneManager->GetRenderer()->GetShaderManager());
	particle4->SetWorldPosition(0.0f, 0.0f, -20.0f);

	gameManager = new GameManager(playerList);
	gameManager->Initialize();

	// Add scene objects
	AddObject(projectileManager->GetProjectileRenderer());
	AddLightObject(dirLight);
	AddLightObject(pointLight);
	AddLightObject(pointLight2);
	AddLightObject(pointLight3);
	AddLightObject(pointLight4);
	AddLightObject(blueLight);
	AddLightObject(redLight);
	AddLightObject(middleLight);
	AddUIObject(fadeImage);
	AddUIObject(roundTextFade);
	AddUIObject(roundText);
	AddUIObject(spectatorText);
	AddUIObject(replayText);
	AddObject(particle1);
	AddObject(particle2);
	AddObject(particle3);
	AddObject(particle4);

	// update phyics list
	PhysicsEngine::GetInstance()->AddObjectList(objectList);

	cameraCD = Cooldown(6.0f);
	cameraCD.startCD();

	roundCD = Cooldown(0.5f);

	voiceCD = Cooldown(6.0);
	voiceCD.startCD();

	return true;
}
void TvTGameScene::Update(const float deltaTime)
{
	projectileManager->Update(deltaTime);

	if (playingIntro) {
		PlayIntro();
	}
	if (!playingIntro && !startText) {
		PlayRoundStart();
	}

		//check for networked inputs and pass them to the appropriate player
		if (Settings::getInstance()->networkedGame) {
			if (Settings::getInstance()->isServer) {
				if (!Settings::getInstance()->spectatorMode) {
					std::string dataChunk = player1->GetPlayerInput()->UpdateJoystickState() + player2->GetPlayerInput()->UpdateJoystickState();
					//std::cout << dataChunk << std::endl;
					BFEngine::GetInstance()->sendData(dataChunk);
				}
				else {
					std::string dataChunk = player1->GetPlayerInput()->UpdateJoystickState() + player2->GetPlayerInput()->UpdateJoystickState() + player3->GetPlayerInput()->UpdateJoystickState() + player4->GetPlayerInput()->UpdateJoystickState();
					//std::cout << dataChunk << std::endl;
					BFEngine::GetInstance()->sendData(dataChunk);
				}
			}
			else if (!Settings::getInstance()->spectatorMode) {
				std::string dataChunk = player3->GetPlayerInput()->UpdateJoystickState() + player4->GetPlayerInput()->UpdateJoystickState();
				//std::cout << dataChunk << std::endl;
				BFEngine::GetInstance()->sendData(dataChunk);
			}
			//we don't send data as the spectator client

			if (Settings::getInstance()->isServer && Settings::getInstance()->spectatorMode) {
				//skip receiving since they're not sending data back
			}
			else {
				//read input from the other game
				std::string externalInput = BFEngine::GetInstance()->receiveData();
				cout << "first set of data received: " << externalInput << endl;
				if (externalInput != "") {
					if (externalInput.length() >= 70) {
						//break it into components
						std::string input1 = externalInput.substr(0, 35);
						std::string input2 = externalInput.substr(35, 35);
						//check first string
						std::string playerNum = input1.substr(0, 1);
						if (playerNum == "0" || playerNum == "1" || playerNum == "2" || playerNum == "3") {
							//cout << "PlayerNum: " << playerNum << std::endl;
							playerList.at(std::stoi(playerNum))->GetPlayerInput()->ParseNetworkInputs(input1);
						}
						playerNum = input2.substr(0, 1);
						if (playerNum == "0" || playerNum == "1" || playerNum == "2" || playerNum == "3") {
							//cout << "PlayerNum: " << playerNum << std::endl;
							playerList.at(std::stoi(playerNum))->GetPlayerInput()->ParseNetworkInputs(input2);
						}
					}
					//if spectator mode is on we'll have more controllers to parse
					if (Settings::getInstance()->spectatorMode && externalInput.length() >= 140) {
						std::string input1 = externalInput.substr(70, 35);
						std::string input2 = externalInput.substr(105, 35);
						//check first string
						std::string playerNum = input1.substr(0, 1);
						if (playerNum == "0" || playerNum == "1" || playerNum == "2" || playerNum == "3") {
							//cout << "PlayerNum: " << playerNum << std::endl;
							playerList.at(std::stoi(playerNum))->GetPlayerInput()->ParseNetworkInputs(input1);
						}
						playerNum = input2.substr(0, 1);
						if (playerNum == "0" || playerNum == "1" || playerNum == "2" || playerNum == "3") {
							//cout << "PlayerNum: " << playerNum << std::endl;
							playerList.at(std::stoi(playerNum))->GetPlayerInput()->ParseNetworkInputs(input2);
						}
					}
				}
			}
		}

		//sets up spectator scene
		if (Settings::getInstance()->networkedGame && Settings::getInstance()->spectatorMode && !Settings::getInstance()->isServer && !playingIntro)
		{
			sceneManager->EnableSplitscreen(false);
			gameManager->divider->SetVisible(false);
			gameManager->end->SetVisible(false);
			bottomRing->canRender = false;
			spectatorText->SetVisible(true);
			cameraList[0]->Position = glm::vec3(7.0f, 5.5f, 0.0f);
			cameraList[0]->SetRotationY(-180.0f);
			cameraList[0]->SetRotationX(-50.0f);
		}
		else {
			bottomRing->canRender = true;
			spectatorText->SetVisible(false);
		}

		///temporary home of replay. we're gonna move this all to file soon enough
		if (Settings::getInstance()->replaySystemEnabled && Settings::getInstance()->playingReplay) {
			std::string externalInput = Replay::GetInstance()->PullFromReplay();
			if (externalInput != "") {
				if (externalInput.length() >= 70) {
					//break it into components
					std::string input1 = externalInput.substr(0, 35);
					std::string input2 = externalInput.substr(35, 35);
					//check first string
					std::string playerNum = input1.substr(0, 1);
					if (playerNum == "0" || playerNum == "1" || playerNum == "2" || playerNum == "3") {
						//cout << "PlayerNum: " << playerNum << std::endl;
						playerList.at(std::stoi(playerNum))->GetPlayerInput()->ParseNetworkInputs(input1);
					}
					playerNum = input2.substr(0, 1);
					if (playerNum == "0" || playerNum == "1" || playerNum == "2" || playerNum == "3") {
						//cout << "PlayerNum: " << playerNum << std::endl;
						playerList.at(std::stoi(playerNum))->GetPlayerInput()->ParseNetworkInputs(input2);
					}
				}
				if (externalInput.length() >= 140) {
					std::string input1 = externalInput.substr(70, 35);
					std::string input2 = externalInput.substr(105, 35);
					//check first string
					std::string playerNum = input1.substr(0, 1);
					if (playerNum == "0" || playerNum == "1" || playerNum == "2" || playerNum == "3") {
						//cout << "PlayerNum: " << playerNum << std::endl;
						playerList.at(std::stoi(playerNum))->GetPlayerInput()->ParseNetworkInputs(input1);
					}
					playerNum = input2.substr(0, 1);
					if (playerNum == "0" || playerNum == "1" || playerNum == "2" || playerNum == "3") {
						//cout << "PlayerNum: " << playerNum << std::endl;
						playerList.at(std::stoi(playerNum))->GetPlayerInput()->ParseNetworkInputs(input2);
					}
				}
			}
		}

		//controller button presses (no longer in handle events)
		for (int i = 0; i < playerList.size(); i++)
		{
				//playerList.at(i)->GetPlayerInput()->DebugState();
				int buttonPressed = 8; //back button
				if (playerList.at(i)->GetPlayerInput()->NewButtonPress(buttonPressed)) {
					if (playerList.at(i)->GetPlayerInput()->controllerState.at(buttonPressed) == 1) {
						if (gameManager->IsGameOver()) {
							if (Settings::getInstance()->replaySystemEnabled && !Settings::getInstance()->playingReplay) {
								std::string dataChunk = player1->GetPlayerInput()->UpdateJoystickState() + player2->GetPlayerInput()->UpdateJoystickState() + player3->GetPlayerInput()->UpdateJoystickState() + player4->GetPlayerInput()->UpdateJoystickState();
								Replay::GetInstance()->PushForReplay(dataChunk);
								Replay::GetInstance()->SaveReplay("DefaultReplay.txt");
							}
							Mix_HaltChannel(playerList[0]->dialogue.channel);
							Mix_HaltChannel(playerList[1]->dialogue.channel);
							Mix_HaltChannel(playerList[2]->dialogue.channel);
							Mix_HaltChannel(playerList[3]->dialogue.channel);
							InputManager::GetInstance()->ClearControllers();
							InputManager::GetInstance()->initalizeControllers();
							sceneManager->controllers.clear();
							sceneManager->saveData.clear();
							sceneManager->SwitchScene(new CharacterSelectScene());
						}
					}
				}
	
			buttonPressed = 14; //start button
			if (playerList.at(i)->GetPlayerInput()->NewButtonPress(buttonPressed)) {
				if (playerList.at(i)->GetPlayerInput()->controllerState.at(buttonPressed) == 1) {
					if (playingIntro) {
						SkipIntro();
					}
					if (gameManager->IsGameOver() && gameManager->canContinue) {
						Restart();
					}
				}
			}
		}

		if (Settings::getInstance()->replaySystemEnabled && !Settings::getInstance()->playingReplay) {
			std::string dataChunk = player1->GetPlayerInput()->ReturnJoystickState() + player2->GetPlayerInput()->ReturnJoystickState() + player3->GetPlayerInput()->ReturnJoystickState() + player4->GetPlayerInput()->ReturnJoystickState();
			Replay::GetInstance()->PushForReplay(dataChunk);
		}
	
}

void TvTGameScene::FixedUpdate(const float deltaTime)
{
	// Update object list for physics
	PhysicsEngine::GetInstance()->AddObjectList(objectList);

	// Update physics and projectile manager
	PhysicsEngine::GetInstance()->Update(deltaTime);
	projectileManager->FixedUpdate(deltaTime);
	gameManager->Update();

	// Update projectiles in projectile manager
	if (projectileManager->GetProjectileList().size() != NULL) {
		for (size_t i = 0; i < projectileManager->GetProjectileList().size(); ++i) {
			if (projectileManager->GetProjectileList().at(i)->deleted) {
				projectileManager->RemoveProjectile(projectileManager->GetProjectileList().at(i));
			}
		}
	}

	if (gameManager->IsGameOver() && sceneManager->justCPUS) {
		Restart();
	}
}
void TvTGameScene::HandleEvents(SDL_Event events)
{
	if (roundStart) {

		// Camera look
		if (events.type == SDL_MOUSEMOTION) {
			cameraList[0]->ProcessMouseMovement((float)events.motion.x, (float)events.motion.y);
		}

		// PLAYER
		if (events.type == SDL_KEYDOWN && events.key.repeat == 0)
		{
			if (events.key.keysym.sym == SDLK_LEFT) {
				std::vector<Projectile*> p = player1->LightAttack();
				if (p.size() > 0) {
					for each (Projectile* subP in p) {
						projectileManager->AddProjectile(subP);
					}
				}
			}

			if (events.key.keysym.sym == SDLK_UP) {
				player1->Jump();
			}

			if (events.key.keysym.sym == SDLK_DOWN) {
				std::vector<Projectile*> p = player1->MediumAttack();
				if (p.size() > 0) {
					for each (Projectile* subP in p) {
						projectileManager->AddProjectile(subP);
					}
				}
			}

			if (events.key.keysym.sym == SDLK_RIGHT) {
				std::vector<Projectile*> p = player1->HeavyAttack();
				if (p.size() > 0) {
					for each (Projectile* subP in p) {
						projectileManager->AddProjectile(subP);
					}
				}
			}

			if (events.key.keysym.sym == SDLK_RCTRL) {
				std::vector<Projectile*> p = player1->SpecialAttack();
				if (p.size() > 0) {
					for each (Projectile* subP in p) {
						projectileManager->AddProjectile(subP);
					}
				}
			}

			if (events.key.keysym.sym == SDLK_o) {
				player1->SwitchTarget();
			}

			if (events.key.keysym.sym == SDLK_i) {
				player1->EnableTarget();
			}
		}

		// Call player handle events
		player1->HandleEvents(events);
		player2->HandleEvents(events);
		player3->HandleEvents(events);
		player4->HandleEvents(events);

	}
}

void TvTGameScene::HandleStates(const Uint8 *state)
{
	if (roundStart) {
		// Camera move
		if (state[SDL_SCANCODE_T]) {
			cameraList[0]->ProcessKeyboard(FORWARD, deltaTime);
		}
		if (state[SDL_SCANCODE_G]) {
			cameraList[0]->ProcessKeyboard(BACKWARD, deltaTime);
		}
		if (state[SDL_SCANCODE_H]) {
			cameraList[0]->ProcessKeyboard(RIGHT, deltaTime);
		}
		if (state[SDL_SCANCODE_F]) {
			cameraList[0]->ProcessKeyboard(LEFT, deltaTime);
		}

		if (state[SDL_SCANCODE_Z]) {
			bgm->Stop();
			sceneManager->SwitchScene(new TvTGameScene());
		}

		// Player 1 movement
		if (state[SDL_SCANCODE_W]) {
			player1->Movement(Player::PLAYERMOVEMENT::FORWARD, fixedDeltaTime);
		}
		if (state[SDL_SCANCODE_S]) {
			player1->Movement(Player::PLAYERMOVEMENT::BACKWARD, fixedDeltaTime);
		}
		if (state[SDL_SCANCODE_D]) {
			player1->Movement(Player::PLAYERMOVEMENT::RIGHT, fixedDeltaTime);
		}
		if (state[SDL_SCANCODE_A]) {
			player1->Movement(Player::PLAYERMOVEMENT::LEFT, fixedDeltaTime);
		}

		// Reload Scene
		if (state[SDL_SCANCODE_Z]) {
			sceneManager->SwitchScene(new TvTGameScene());
		}

		// Reload match
		if (state[SDL_SCANCODE_R]) {
			if (gameManager->canContinue) {
				Restart();
			}
		}

		// Call player handle states
		player1->HandleStates(state);
		player2->HandleStates(state);
		player3->HandleStates(state);
		player4->HandleStates(state);
	}

	if (state[SDL_SCANCODE_SPACE]) {
		SkipIntro();
	}

	if (state[SDL_SCANCODE_L]) {
		spectator = true;
	}

	if (state[SDL_SCANCODE_K]) {
		spectator = false;
	}
}

void TvTGameScene::CameraMove(ENGINE::Camera_Movement direction, float yaw, float pitch, float speed, int camera)
{
	cameraList[camera]->SetRotationX(pitch);
	cameraList[camera]->SetRotationY(yaw);
	cameraList[camera]->ProcessKeyboard(direction, speed * deltaTime);
}

void TvTGameScene::PlayIntro()
{
	bgm->SetVolume(100);
	if (playAudio) {
		bgm->Play(-1);
		playAudio = false;
	}
	
	if (cameraCD.secondsLeft() > 4) {
		CameraMove(RIGHT, -145.0f, 11.0f, 0.5f, 0);
	}
	else if (cameraCD.secondsLeft() > 2.0) {
		if (!cameraSwitch1) {
			cameraList[0]->Position = glm::vec3(-5.0f, 15.0f, 1.0f);
			cameraSwitch1 = true;
		}
		CameraMove(LEFT, -33.0f, -56.0f, 0.5f, 0);
	}
	else if (cameraCD.secondsLeft() > 0) {
		if (!cameraSwitch2) {
			cameraList[0]->Position = glm::vec3(6.0f, 8.0f, -12.0f);
			cameraList[0]->SetRotationY(-220.0f);
			cameraList[0]->SetRotationX(-29.0f);
			fadeAlpha = 1.0f;
			cameraSwitch2 = true;
		}
		cameraList[0]->SetRotationY(cameraList[0]->Yaw -= deltaTime * 10);
	}
	else if (cameraCD.checkOffCD()) {
		if (!cameraSwitch3) {
			gameManager->StartMatch();
			cameraList[0]->Position = glm::vec3(0.0f, 7.5f, 9.5f);
			cameraList[0]->SetRotationY(-90.0f);
			cameraList[0]->SetRotationX(-25.0f);
			cameraList[1]->Position = glm::vec3(0.0f, 7.5f, -9.5f);
			cameraList[1]->SetRotationY(90.0f);
			cameraList[1]->SetRotationX(-25.0f);

			sceneManager->EnableSplitscreen(true);

			cameraSwitch3 = true;
		}

		cameraList[0]->SetRotationX(cameraList[0]->Pitch -= deltaTime * 10);
		cameraList[1]->SetRotationX(cameraList[0]->Pitch);

		if (cameraList[0]->Pitch <= -45.0f) {
			cameraList[0]->Pitch = -45.0f;
			cameraList[1]->Pitch = -45.0f;
			fadeImage->SetVisible(false);
			roundText->SetVisible(true);
			roundCD.startCD();
			fadeAlpha = 1.0f;
			playingIntro = false;
			for (int i = 0; i < playerList.size(); i++)
			{
				playerList.at(i)->PlayingIntro = false;
			}
		}
	}

	fadeAlpha -= deltaTime;
	fadeImage->SetAlpha(fadeAlpha);
	if (fadeAlpha <= 0) {
		fadeAlpha = 0;
	}

	PlayInteractions();
}

void TvTGameScene::SkipIntro()
{
	if (playingIntro) {
		playingIntro = false;

		cameraSwitch1 = true;
		cameraSwitch2 = true;
		cameraSwitch3 = true;

		gameManager->StartMatch();

		fadeImage->SetVisible(false);

		sceneManager->EnableSplitscreen(true);

		fadeAlpha = 1.0f;
		fadeImage->SetAlpha(1.0f);

		roundText->SetVisible(true);
		
		roundCD.startCD();

		cameraList[0]->Position = glm::vec3(0.0f, 7.5f, 9.5f);
		cameraList[0]->SetRotationY(-90.0f);
		cameraList[0]->SetRotationX(-45.0f);
		cameraList[1]->Position = glm::vec3(0.0f, 7.5f, -9.5f);
		cameraList[1]->SetRotationY(90.0f);
		cameraList[1]->SetRotationX(-45.0f);

		Mix_HaltChannel(playerList[0]->dialogue.channel);
		Mix_HaltChannel(playerList[1]->dialogue.channel);
		Mix_HaltChannel(playerList[2]->dialogue.channel);
		Mix_HaltChannel(playerList[3]->dialogue.channel);

		for (int i = 0; i < playerList.size(); i++)
		{
			playerList.at(i)->PlayingIntro = false;
		}
	}
}

void TvTGameScene::PlayRoundStart()
{
	if (!ready) {
		if (!announcerVoice) {
			if (!Mix_Playing(1)) {
				gameManager->announcer.playRandomFromOtherState(gameManager->announcer.MatchStart, false);
				announcerVoice = true;
			}
		}
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
			gameManager->StartTimer();
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

void TvTGameScene::SetUpPlayers()
{
	// Add players to the player list
	playerList.push_back(player1);
	playerList.push_back(player2);
	playerList.push_back(player3);
	playerList.push_back(player4);

	std::string playersForReplay;
	if (Settings::getInstance()->replaySystemEnabled && Settings::getInstance()->playingReplay == true) {
		if (Replay::GetInstance()->TryLoadReplay("defaultReplay.txt")) {
			
			//std::cout << "Replay loaded successfully" << std::endl;
		}
		else {
			//std::cout << "Replay failed to load" << std::endl;
		}
		playersForReplay = Replay::GetInstance()->PullFromReplay();
		replayText->SetVisible(true);
	}
	else {
		replayText->SetVisible(false);
	}

	// Loop through player list and create character based on save data from chararcter select
	for (unsigned int i = 0; i < playerList.size(); i++)
	{
		//normal player behavior
		if (sceneManager->saveData.size() != 0) {
			if (sceneManager->saveData[i] == 0) {
				playerList.at(i) = new LightningPlayer();
			}
			else if (sceneManager->saveData[i] == 1) {
				playerList.at(i) = new EarthPlayer();
			}
			else if (sceneManager->saveData[i] == 2) {
				playerList.at(i) = new IcePlayer();
			}
			else if (sceneManager->saveData[i] == 3) {
				playerList.at(i) = new FirePlayer();
			}
			else if (sceneManager->saveData[i] == 4) {
				playerList.at(i) = new WindPlayer();
			}
			else
			{
				playerList.at(i) = new WindPlayer();
			}
		}
		//replay player stuff
		if (Settings::getInstance()->replaySystemEnabled) {
			if (Settings::getInstance()->playingReplay == false) {
				playersForReplay += std::to_string(sceneManager->saveData[i]); //push in the player numbers for the replay
			}
			else { //pull the number out of the replay file
				std::string replaySaveData = playersForReplay.substr(i, 1);
				if (replaySaveData == "0") {
					playerList.at(i) = new LightningPlayer();
				}
				else if (replaySaveData == "1") {
					playerList.at(i) = new EarthPlayer();
				}
				else if (replaySaveData == "2") {
					playerList.at(i) = new IcePlayer();
				}
				else if (replaySaveData == "3") {
					playerList.at(i) = new FirePlayer();
				}
				else if (replaySaveData == "4") {
					playerList.at(i) = new WindPlayer();
				}
				else
				{
					playerList.at(i) = new WindPlayer();
				}
			}
		}
	}

	if (Settings::getInstance()->replaySystemEnabled && Settings::getInstance()->playingReplay == false) {
		Replay::GetInstance()->PushForReplay(playersForReplay);
		Replay::GetInstance()->PushForReplay(playersForReplay);
	}

	// Assign player reference
	player1 = playerList[0];
	player2 = playerList[1];
	player3 = playerList[2];
	player4 = playerList[3];

	// Set Shaders
	player1->SetShader(defaultShaderHandle);
	player2->SetShader(defaultShaderHandle);
	player3->SetShader(defaultShaderHandle);
	player4->SetShader(defaultShaderHandle);

	// Set Positions
	player1->SetWorldPosition(glm::vec3(-3.0f, 0.0f, 4.0f));
	player2->SetWorldPosition(glm::vec3(3.0f, 0.0f, 4.0f));
	player3->SetWorldPosition(glm::vec3(3.0f, 0.0f, -4.0f));
	player4->SetWorldPosition(glm::vec3(-3.0f, 0.0f, -4.0f));

	// Set player numbers
	player1->SetPlayerNumber(Player::PLAYERNUMBER::PLAYER1);
	player2->SetPlayerNumber(Player::PLAYERNUMBER::PLAYER2);
	player3->SetPlayerNumber(Player::PLAYERNUMBER::PLAYER3);
	player4->SetPlayerNumber(Player::PLAYERNUMBER::PLAYER4);

	// Set player teams
	player1->SetPlayerTeam(Player::PLAYERTEAM::TEAM1);
	player2->SetPlayerTeam(Player::PLAYERTEAM::TEAM1);
	player3->SetPlayerTeam(Player::PLAYERTEAM::TEAM2);
	player4->SetPlayerTeam(Player::PLAYERTEAM::TEAM2);

	// Assign controllers saved from character select if there are any (and we're not playing a replay)
	///cpu stuff is commented out for now
	if (sceneManager->controllers.size() != 0 && !Settings::getInstance()->playingReplay) {
		if (sceneManager->controllers[0] != NULL) {
			player1->GetPlayerInput()->SetJoystick(sceneManager->controllers[0]);
		}
		else {
			player1->SetAsCPU();
		}
		if (sceneManager->controllers[1] != NULL) {
			player2->GetPlayerInput()->SetJoystick(sceneManager->controllers[1]);
		}
		else if (!Settings::getInstance()->networkedGame) {
			player2->SetAsCPU();
		}
		if (sceneManager->controllers[2] != NULL) {
			player3->GetPlayerInput()->SetJoystick(sceneManager->controllers[2]);
		}
		else if (!Settings::getInstance()->networkedGame) {
			player3->SetAsCPU();
		}
		if (sceneManager->controllers[3] != NULL) {
			player4->GetPlayerInput()->SetJoystick(sceneManager->controllers[3]);
		}
		else if (!Settings::getInstance()->networkedGame) {
			player4->SetAsCPU();
		}
	}

	if (Settings::getInstance()->networkedGame && !Settings::getInstance()->isServer) {
		player1->GetPlayerInput()->makeNetworked();
	}

	if (Settings::getInstance()->networkedGame && !Settings::getInstance()->isServer) {
		player2->GetPlayerInput()->makeNetworked();
	}

	if (Settings::getInstance()->networkedGame && ((Settings::getInstance()->isServer && !Settings::getInstance()->spectatorMode) || (!Settings::getInstance()->isServer && Settings::getInstance()->spectatorMode))) {
		player3->GetPlayerInput()->makeNetworked();
	}

	if (Settings::getInstance()->networkedGame && ((Settings::getInstance()->isServer && !Settings::getInstance()->spectatorMode) || (!Settings::getInstance()->isServer && Settings::getInstance()->spectatorMode))) {
		player4->GetPlayerInput()->makeNetworked();
	}

	if (sceneManager->justCPUS) {
		player1->SetAsCPU();
		player2->SetAsCPU();
		player3->SetAsCPU();
		player4->SetAsCPU();
	}

	// Set player audio channels
	player1->dialogue.channel = 2;
	player2->dialogue.channel = 3;
	player3->dialogue.channel = 4;
	player4->dialogue.channel = 5;

	// Add projectile manager
	player1->AddProjecitleManager(projectileManager);
	player2->AddProjecitleManager(projectileManager);
	player3->AddProjecitleManager(projectileManager);
	player4->AddProjecitleManager(projectileManager);

	// Set the players to not  move when scene starts
	player1->SetCanMove(false);
	player2->SetCanMove(false);
	player3->SetCanMove(false);
	player4->SetCanMove(false);

	// Enemy Target set up
	player1->SetEnemyTeam(player3, player4);
	player2->SetEnemyTeam(player4, player3);
	player3->SetEnemyTeam(player1, player2);
	player4->SetEnemyTeam(player2, player1);

	// Add players to object list
	AddObject(player1);
	AddObject(player2);
	AddObject(player3);
	AddObject(player4);

	// Add players to the projectile manager
	projectileManager->AddPlayer(player1);
	projectileManager->AddPlayer(player2);
	projectileManager->AddPlayer(player3);
	projectileManager->AddPlayer(player4);
}

void TvTGameScene::SetUpArena()
{
	// Make models
	//
	platform = new Model("Resources/Models/Platform/platform.obj");
	platform->SetWorldRotation(glm::vec3(1, 0, 0), -1.57);
	platform->SetLocalRotation(glm::vec3(0, 0, 1), 1.57);
	platform->SetShader(defaultShaderHandle);
	platform->physicsComponent->SetPosition(glm::vec3(0.0f, -0.43f, 0.0f));
	platform->SetWorldScale(0.0090f, 0.01f, 0.006f);
	platform->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	platform->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	platform->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::PERFECT_SMOOTH);
	platform->physicsComponent->SetMass(0.0f);
	platform->collisionComponent->SetLayer(20);
	//

	//
	topRing = new Model("Resources/Models/ring/ring.obj");
	topRing->SetShader(lightShaderHandle);
	topRing->physicsComponent->SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
	topRing->SetWorldScale(0.05f, 0.025f, 0.05f);
	topRing->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	topRing->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	topRing->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	topRing->physicsComponent->SetMass(0.0f);
	topRing->collisionComponent->SetLayer(20);
	//

	//
	middleRing = new Model("Resources/Models/ring/ring.obj");
	middleRing->SetShader(lightShaderHandle);
	middleRing->physicsComponent->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
	middleRing->SetWorldScale(0.15f, 0.025f, 0.15f);
	middleRing->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	middleRing->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	middleRing->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	middleRing->physicsComponent->SetMass(0.0f);
	middleRing->collisionComponent->SetLayer(20);
	//

	//
	bottomRing = new Model("Resources/Models/ring/ring.obj");
	bottomRing->SetShader(lightShaderHandle);
	bottomRing->physicsComponent->SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));
	bottomRing->SetWorldScale(0.15f, 0.5f, 0.15f);
	bottomRing->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	bottomRing->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	bottomRing->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	bottomRing->physicsComponent->SetMass(0.0f);
	bottomRing->collisionComponent->SetLayer(20);
	//

	// floor collision
	floor = new Cube();
	floor->SetShader(defaultShaderHandle);
	floor->renderComponent->SetColour(0.1f, 0.1f, 0.1f);
	floor->physicsComponent->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
	floor->SetWorldScale(11.0f, 1.0f, 12.0f);
	floor->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	floor->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NORMAL_ELASTIC);
	floor->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::SMOOTH);
	floor->physicsComponent->SetMass(0.0f);
	floor->canRender = false;
	//

	// map middle divider
	divider = new Cube();
	divider->SetShader(defaultShaderHandle);
	divider->renderComponent->SetColour(0.1f, 0.1f, 0.1f);
	divider->physicsComponent->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	divider->SetWorldScale(100.0f, 100.0f, 0.5f);
	divider->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	divider->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::PERFECT_NON_ELASTIC);
	divider->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::PERFECT_SMOOTH);
	divider->physicsComponent->SetMass(0.0f);
	divider->canRender = false;
	//

	projectileManager->AddEnvironment(floor);

	AddObject(platform);
	//AddObject(topRing);
	//AddObject(middleRing);
	//AddObject(bottomRing);
	AddObject(floor);
	AddObject(divider);
}

void TvTGameScene::Restart()
{
	for (int i = 0; i < playerList.size(); i++)
	{
		playerList.at(i)->PlayingIntro = true;
	}

	fadeImage->SetVisible(true);

	gameManager->Reset();

	sceneManager->EnableSplitscreen(false);
	cameraList[0]->Position = glm::vec3(7.0f, 0.7f, 7.0f);
	cameraList[0]->Yaw = 0.0f;
	cameraList[0]->Pitch = 0.0f;
	cameraList[1]->Position = glm::vec3(7.0f, 0.7f, 7.0f);

	roundText->SetText("Ready?");
	roundText->SetSize(0.0f);
	roundText->SetSpacing(9.0f);
	roundText->SetAlpha(1.0f);
	roundText->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundText->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
	roundText->SetVisible(false);

	roundTextFade->SetSize(1.0f);
	roundTextFade->SetSpacing(9.0f);
	roundTextFade->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundText->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
	roundTextFade->SetVisible(false);

	cameraCD = Cooldown(6.0);
	cameraCD.startCD();

	voiceCD = Cooldown(6.0);
	voiceCD.startCD();

	roundCD = Cooldown(0.5);

	fadeAlpha = 1.0f;
	cameraSwitch1 = false;
	cameraSwitch2 = false;
	cameraSwitch3 = false;
	playingIntro = true;
	ready = false;
	startText = false;
	roundStart = false;
	playAudio = true;

	player1voice = false;
	player2voice = false;
	player3voice = false;
	player4voice = false;

	announcerVoice = false;
}

void TvTGameScene::PlayInteractions()
{
	if (!player1voice) {
		if (sceneManager->saveData.size() != 0) {
			if (sceneManager->saveData[1] == 0) {
				player1->dialogue.playRandomFromOtherState(player1->dialogue.Alex, true);
			}
			else if (sceneManager->saveData[1] == 1) {
				player1->dialogue.playRandomFromOtherState(player1->dialogue.Flint, true);
			}
			else if (sceneManager->saveData[1] == 2) {
				player1->dialogue.playRandomFromOtherState(player1->dialogue.Jack, true);
			}
			else if (sceneManager->saveData[1] == 3) {
				player1->dialogue.playRandomFromOtherState(player1->dialogue.Kal, true);
			}
			else if (sceneManager->saveData[1] == 4) {
				player1->dialogue.playRandomFromOtherState(player1->dialogue.Oki, true);
			}
		}
		player1voice = true;
	}
	if (!player2voice && voiceCD.secondsLeft() < 4.0) {
		if (sceneManager->saveData.size() != 0) {
			if (sceneManager->saveData[0] == 0) {
				player2->dialogue.playRandomFromOtherState(player2->dialogue.Alex, true);
			}
			else if (sceneManager->saveData[0] == 1) {
				player2->dialogue.playRandomFromOtherState(player2->dialogue.Flint, true);
			}
			else if (sceneManager->saveData[0] == 2) {
				player2->dialogue.playRandomFromOtherState(player2->dialogue.Jack, true);
			}
			else if (sceneManager->saveData[0] == 3) {
				player2->dialogue.playRandomFromOtherState(player2->dialogue.Kal, true);
			}
			else if (sceneManager->saveData[0] == 4) {
				player2->dialogue.playRandomFromOtherState(player2->dialogue.Oki, true);
			}
		}
		player2voice = true;
	}

	if (!player3voice && voiceCD.secondsLeft() < 2.0) {
		if (sceneManager->saveData.size() != 0) {
			if (sceneManager->saveData[3] == 0) {
				player3->dialogue.playRandomFromOtherState(player3->dialogue.Alex, true);
			}
			else if (sceneManager->saveData[3] == 1) {
				player3->dialogue.playRandomFromOtherState(player3->dialogue.Flint, true);
			}
			else if (sceneManager->saveData[3] == 2) {
				player3->dialogue.playRandomFromOtherState(player3->dialogue.Jack, true);
			}
			else if (sceneManager->saveData[3] == 3) {
				player3->dialogue.playRandomFromOtherState(player3->dialogue.Kal, true);
			}
			else if (sceneManager->saveData[3] == 4) {
				player3->dialogue.playRandomFromOtherState(player3->dialogue.Oki, true);
			}
		}
		player3voice = true;
	}
	if (!player4voice && voiceCD.secondsLeft() < 0) {
		if (sceneManager->saveData.size() != 0) {
			if (sceneManager->saveData[2] == 0) {
				player4->dialogue.playRandomFromOtherState(player4->dialogue.Alex, true);
			}
			else if (sceneManager->saveData[2] == 1) {
				player4->dialogue.playRandomFromOtherState(player4->dialogue.Flint, true);
			}
			else if (sceneManager->saveData[2] == 2) {
				player4->dialogue.playRandomFromOtherState(player4->dialogue.Jack, true);
			}
			else if (sceneManager->saveData[2] == 3) {
				player4->dialogue.playRandomFromOtherState(player4->dialogue.Kal, true);
			}
			else if (sceneManager->saveData[2] == 4) {
				player4->dialogue.playRandomFromOtherState(player4->dialogue.Oki, true);
			}
		}
		player4voice = true;
	}
}