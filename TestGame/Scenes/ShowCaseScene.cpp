#include "ShowCaseScene.h"

using namespace GAME;

ShowCaseScene::ShowCaseScene()
{
}

ShowCaseScene::~ShowCaseScene()
{
}

bool ShowCaseScene::Initialize()
{
	// Make reference to the scene manager
	sceneManager = BFEngine::GetInstance()->GetSceneManager();

	// Scene Options
	sceneManager->CaptureMouse(true);
	sceneManager->ShowFPS(false);

	// Set the position of the first camera
	cameraList[0]->Position = glm::vec3(1.7f, 0.5f, 2.0f);
	cameraList[0]->Zoom = 45.0f;
	// Set the position for the second camera
	cameraList[1]->Position = glm::vec3(0.0f, 2.0f, 4.0f);

	// Load shaders
	defaultShader = new Shader("Shaders/model.vs", "Shaders/model.fs");
	skyboxShader = new Shader("Shaders/skybox.vs", "Shaders/skybox.fs");
	lightShader = new Shader("Shaders/lamp.vs", "Shaders/lamp.fs");

	// Put shaders into shader manager
	defaultShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("default"), defaultShader);
	skyboxShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("skybox"), skyboxShader);
	lightShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("light"), lightShader);

	// Lights
	pointLight = new Light();
	pointLight->SetShader(lightShaderHandle);
	pointLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	pointLight->renderComponent->SetColour(1.0f, 1.0f, 1.0f);
	pointLight->SetWorldPosition(-1.0f, 1.0f, -0.5f);
	pointLight->SetWorldScale(0.5f);
	pointLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	pointLight->lightComponent->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));
	pointLight->canRender = false;

	// Make directional light
	dirLight = new Light(LightComponent::Light_Type::DIRECTIONAL);
	dirLight->lightComponent->SetDirection(glm::vec3(0.5f, 0.0f, 0.4f));
	dirLight->lightComponent->SetColour(glm::vec3(0.3f, 0.3f, 0.3f));

	// UI
	fadeImage = new ImageUI();
	fadeImage->SetImage("Resources/Textures/whiteFILL.png");
	fadeImage->SetPosition(sceneManager->GetScreenWidth() / 2, sceneManager->GetScreenHeight() / 2);
	fadeImage->SetScale(4.0f, 11.0f);

	// Player
	player = new LightningPlayer();
	player->SetShader(defaultShaderHandle);
	player->EnableTarget();

	// Floor
	floor = new Cube();
	floor->SetShader(defaultShaderHandle);
	floor->renderComponent->SetColour(0.1f, 0.1f, 0.1f);
	floor->physicsComponent->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
	floor->SetWorldScale(11.0f, 1.0f, 11.0f);
	floor->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	floor->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	floor->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	floor->physicsComponent->SetMass(0.0f);
	floor->canRender = false;

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
	bottomRing = new Model("Resources/Models/ring/ring.obj");
	bottomRing->SetShader(lightShaderHandle);
	bottomRing->physicsComponent->SetPosition(glm::vec3(0.0f, -4.0f, 0.0f));
	bottomRing->SetWorldScale(0.15f, 0.5f, 0.15f);
	bottomRing->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	bottomRing->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	bottomRing->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	bottomRing->physicsComponent->SetMass(0.0f);
	bottomRing->collisionComponent->SetLayer(20);
	//

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

	// add objects and lights to their lists
	AddObject(player);
	AddObject(floor);
	AddObject(platform);
	//AddObject(bottomRing);
	AddLightObject(pointLight);
	AddLightObject(dirLight);
	AddUIObject(fadeImage);

	cameraCD = Cooldown(4.0f);
	cameraCD.startCD();

	return true;
}

void ShowCaseScene::Update(const float deltaTime)
{
	//sceneManager->DebugText(cameraList[0]->Position);
	//sceneManager->DebugText(cameraList[0]->Yaw);
	//sceneManager->DebugText(cameraList[0]->Pitch);
	//sceneManager->DebugText(cameraList[0]->Zoom);

	if (playingIntro) {
		if (cameraCD.secondsLeft() > 2.5) {
			CameraMove(RIGHT, -143.0f, 10.0f, 0.5f, 0);
		}
		else if (cameraCD.secondsLeft() > 1.0) {
			if (!cameraSwitch1) {
				cameraList[0]->Position = glm::vec3(0.0f, 4.0f, -2.0f);
				fadeAlpha = 1.0f;
				cameraSwitch1 = true;
			}
			CameraMove(LEFT, 0.0f, -90.0f, 0.5f, 0);
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
				cameraList[0]->Position = glm::vec3(4.0f, 0.7f, -4.5f);
				fadeAlpha = 1.0f;

				cameraSwitch3 = true;
			}

			CameraMove(FORWARD, -216.0f, -2.5f, 1.5f, 0);
			cameraList[0]->Zoom += deltaTime * 30;

			if (cameraList[0]->Zoom >= 75.0f) {
				cameraList[0]->Zoom = 75.0f;
				fadeAlpha = 0.0f;
				playingIntro = false;
			}
		}

		fadeAlpha -= deltaTime * 2;
		fadeImage->SetAlpha(fadeAlpha);
		if (fadeAlpha <= 0) {
			fadeAlpha = 0;
		}
	}
}

void ShowCaseScene::FixedUpdate(const float deltaTime) 
{
	PhysicsEngine::GetInstance()->AddObjectList(objectList);
	PhysicsEngine::GetInstance()->Update(deltaTime);
};

void ShowCaseScene::HandleEvents(SDL_Event events)
{
	// Camera look
	if (events.type == SDL_MOUSEMOTION) {
		cameraList[0]->ProcessMouseMovement((float)events.motion.x, (float)events.motion.y);
	}

	if (events.type == SDL_MOUSEWHEEL) {
		cameraList[0]->ProcessMouseScroll((float)events.wheel.y);
	}
}

void ShowCaseScene::HandleStates(const Uint8 *state)
{
	// VIEW/CAMERA
	if (state[SDL_SCANCODE_W]) {
		cameraList[0]->ProcessKeyboard(FORWARD, deltaTime * 2);
	}
	if (state[SDL_SCANCODE_S]) {
		cameraList[0]->ProcessKeyboard(BACKWARD, deltaTime * 2);
	}
	if (state[SDL_SCANCODE_D]) {
		cameraList[0]->ProcessKeyboard(RIGHT, deltaTime * 2);
	}
	if (state[SDL_SCANCODE_A]) {
		cameraList[0]->ProcessKeyboard(LEFT, deltaTime * 2);
	}

	// Reload Scene
	if (state[SDL_SCANCODE_Z]) {
		sceneManager->EnableSplitscreen(false);
		sceneManager->SwitchScene(new ShowCaseScene());
	}

	if (state[SDL_SCANCODE_7]) {
		sceneManager->CaptureMouse(false);
	}

	if (state[SDL_SCANCODE_8]) {
		sceneManager->CaptureMouse(true);
	}

	// PRESS B FOR FULLSCREEN
	// PRESS V FOR WINDOWED
}

void ShowCaseScene::CameraMove(ENGINE::Camera_Movement direction, float yaw, float pitch, float speed, int camera)
{
	cameraList[camera]->SetRotationX(pitch);
	cameraList[camera]->SetRotationY(yaw);
	cameraList[camera]->ProcessKeyboard(direction, speed * deltaTime);
}