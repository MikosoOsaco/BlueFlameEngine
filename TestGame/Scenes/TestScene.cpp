#include "TestScene.h"

using namespace GAME;

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
	delete model;
	model = nullptr;
	delete model2;
	model2 = nullptr;
	delete model3;
	model3 = nullptr;
	delete model4;
	model4 = nullptr;
	delete floor;
	floor = nullptr;
}

bool TestScene::Initialize()
{
	// Make reference to the scene manager
	sceneManager = BFEngine::GetInstance()->GetSceneManager();

	// Scene Options
	sceneManager->CaptureMouse(true);

	// Set the position of the first camera
	cameraList[0]->Position = glm::vec3(0.0f, 1.0f, 3.0f);
	// Set the position for the second camera
	cameraList[1]->Position = glm::vec3(0.0f, 2.0f, 4.0f);

	// Load shaders
	defaultShader = new Shader("Shaders/default.vs", "Shaders/default.fs");
	modelShader = new Shader("Shaders/model.vs", "Shaders/model.fs");
	skyboxShader = new Shader("Shaders/skybox.vs", "Shaders/skybox.fs");
	lightShader = new Shader("Shaders/lamp.vs", "Shaders/lamp.fs");

	// Put shaders into shader manager
	defaultShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("cube2"), defaultShader);
	modelShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("model"), modelShader);
	skyboxShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("skybox"), skyboxShader);
	lightShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("light"), lightShader);

	// Lights
	//
	// Initialize
	blueLight = new Light(LightComponent::Light_Type::POINTLIGHT);
	// Set which shader this object is using
	blueLight->SetShader(modelShaderHandle);
	// Set type and colour of object's mesh
	blueLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	blueLight->renderComponent->SetColour(0.0f, 0.0f, 1.0f);
	// Set position and scale of object
	blueLight->SetWorldPosition(-3.0f, 2.0f, 0.0f);
	blueLight->SetWorldScale(1.0f);
	// Set what type of light and what colour this object emits
	blueLight->lightComponent->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));
	//

	redLight = new Light();
	redLight->SetShader(lightShaderHandle);
	redLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	redLight->renderComponent->SetColour(1.0f, 0.0f, 0.0f);
	redLight->SetWorldPosition(-3.0f, 0.0f, 0.0f);
	redLight->SetWorldScale(0.5f);
	redLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	redLight->lightComponent->SetColour(glm::vec3(1.0f, 0.0f, 0.0f));

	greenLight = new Light();
	greenLight->SetShader(lightShaderHandle);
	greenLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	greenLight->renderComponent->SetColour(0.0f, 1.0f, 0.0f);
	greenLight->SetWorldPosition(3.0f, 0.0f, 0.0f);
	greenLight->SetWorldScale(0.5f);
	greenLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	greenLight->lightComponent->SetColour(glm::vec3(0.0f, 1.0f, 0.0f));

	yellowLight = new Light();
	yellowLight->SetShader(lightShaderHandle);
	yellowLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	yellowLight->renderComponent->SetColour(1.0f, 1.0f, 0.0f);
	yellowLight->SetWorldPosition(3.0f, 2.0f, 0.0f);
	yellowLight->SetWorldScale(0.5f);
	yellowLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	yellowLight->lightComponent->SetColour(glm::vec3(1.0f, 1.0f, 0.0f));

	// Make directional light
	dirLight = new Light(LightComponent::Light_Type::DIRECTIONAL);
	//dirLight->lightComponent->SetDirection(glm::vec3(-0.0f, 0.0f, 0.8f));
	dirLight->lightComponent->SetDirection(glm::vec3(-0.5f, 0.0f, -0.8f));

	// Floor
	floor = new Cube();
	floor->SetShader(defaultShaderHandle);
	floor->renderComponent->SetColour(0.0f, 0.0f, 0.0f);
	floor->physicsComponent->SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));
	floor->SetWorldScale(12.0f, 1.0f, 12.0f);
	floor->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	floor->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	floor->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::PERFECT_SMOOTH);
	floor->physicsComponent->SetMass(0.0f);

	model = new Model("Resources/Models/miku/miku.obj");
	model->SetShader(modelShaderHandle);
	model->physicsComponent->SetPosition(glm::vec3(1.0f, 1.0f, 0.0f));
	model->SetWorldScale(0.02f);
	//model->SetWorldScale(0.5f);
	//model->SetWorldRotation(glm::vec3(1.0f, 0.0f, 0.0f), 80.0f);
	//model->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
	//model->physicsComponent->SetMass(50.0f);

	model2 = new Model("Resources/Models/nanosuit/nanosuit.obj");
	model2->SetShader(modelShaderHandle);
	model2->physicsComponent->SetPosition(glm::vec3(-1.5f, -0.9f, 0.0f));
	model2->SetWorldScale(0.25f);
	//model->SetWorldScale(0.5f);
	//model->SetWorldRotation(glm::vec3(1.0f, 0.0f, 0.0f), 80.0f);
	//model->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
	//model->physicsComponent->SetMass(50.0f);

	model3 = new Model("Resources/Models/monster/Monster_1.dae");
	model3->SetShader(modelShaderHandle);
	model3->physicsComponent->SetPosition(glm::vec3(0.0f, -1.0f, -2.0f));
	//model3->SetWorldScale(0.02f);
	model3->SetWorldScale(0.5f);
	model3->SetWorldRotation(glm::vec3(1.0f, 0.0f, 0.0f), 80.0f);
	//model->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
	//model->physicsComponent->SetMass(50.0f);

	model4 = new Model("Resources/Models/boblampclean.md5mesh");
	model4->SetShader(modelShaderHandle);
	model4->physicsComponent->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
	model4->SetWorldScale(0.02f);
	model4->SetWorldRotation(glm::vec3(1.0f, 0.0f, 0.0f), 80.0f);
	//model->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
	//model->physicsComponent->SetMass(50.0f);

	// Make skybox, load its textures, set properties, and give to the renderer
	skybox = new Skybox();
	std::vector<char*> faces;
	faces.push_back("Resources/Textures/Skyboxes/grimmnight/right.png");
	faces.push_back("Resources/Textures/Skyboxes/grimmnight/left.png");
	faces.push_back("Resources/Textures/Skyboxes/grimmnight/top.png");
	faces.push_back("Resources/Textures/Skyboxes/grimmnight/bottom.png");
	faces.push_back("Resources/Textures/Skyboxes/grimmnight/back.png");
	faces.push_back("Resources/Textures/Skyboxes/grimmnight/front.png");
	skybox->LoadTextures(faces);
	skybox->SetShader(skyboxShaderHandle);

	image = new ImageUI();
	image->SetImage("Resources/Textures/DefaultButton.png");
	image->SetPosition(300.0f, 300.0f);
	image->SetColour(glm::vec3(0, 0, 0));

	// add objects and lights to their lists
	AddUIObject(image);
	AddObject(model);
	AddObject(model2);
	AddObject(model3);
	AddObject(model4);
	AddObject(floor);
	AddLightObject(dirLight);
	AddLightObject(blueLight);
	AddLightObject(redLight);
	AddLightObject(greenLight);
	AddLightObject(yellowLight);

	return true;
}

void TestScene::Update(const float deltaTime)
{
	//Code to make the model spin
	//model->SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), model->GetWorldRotationAngle() + moveSpeed * deltaTime);
}

void TestScene::FixedUpdate(const float deltaTime)
{

};

void TestScene::HandleEvents(SDL_Event events)
{
	//joystick
	switch (events.type)
	{
	case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */
		if (events.jaxis.axis == 2) //left trigger
		{
			/* Up-Down movement code goes here */
			std::cout << "Left Trigger" << std::endl;
		}
		if (events.jaxis.axis == 5) //right trigger
		{
			/* Up-Down movement code goes here */
			std::cout << "Right Trigger" << std::endl;
		}


		break;

	case SDL_JOYBUTTONDOWN:  /* Handle Joystick Button Presses */
		if (events.jbutton.button == 0) //A button
		{
			/* code goes here */
			std::cout << "A button was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 1) //B button
		{
			/* code goes here */
			std::cout << "B button was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 2) //X button
		{
			/* code goes here */
			std::cout << "X button was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 3) //Y button
		{
			/* code goes here */
			std::cout << "Y button was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 4) //left bumper
		{
			/* code goes here */
			std::cout << "Left bumper was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 5) //right bumper
		{
			/* code goes here */
			std::cout << "right bumper was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 6) //back button
		{
			/* code goes here */
			std::cout << "back button was probably pressed." << std::endl;
			BFEngine::GetInstance()->TerminateGame();
		}
		if (events.jbutton.button == 7) //start button
		{
			/* code goes here */
			std::cout << "start was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 8) //left joystick
		{
			/* code goes here */
			std::cout << "left joystick was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 9) //right joystick
		{
			/* code goes here */
			std::cout << "right joystick was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 10) //xbox key
		{
			/* code goes here */
			std::cout << "xbox key was probably pressed." << std::endl;
			//xbox key closes the game
			BFEngine::GetInstance()->TerminateGame();
		}
		if (events.jbutton.button == 11) //left trigger
		{
			/* code goes here */
			std::cout << "left trigger was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 12) //right trigger
		{
			/* code goes here */
			std::cout << "right trigger was probably pressed." << std::endl;
		}
		break;

	case SDL_JOYBALLMOTION:  /* Handle Joyball Motion */
		if (events.jball.ball == 0)
		{
			/* ball handling */
		}
		break;
	}

	// Camera look
	if (events.type == SDL_MOUSEMOTION) {
		cameraList[0]->ProcessMouseMovement((float)events.motion.x, (float)events.motion.y);
	}

	if (events.type == SDL_MOUSEWHEEL) {
		cameraList[0]->ProcessMouseScroll((float)events.wheel.y);
	}
}

void TestScene::HandleStates(const Uint8 *state) 
{
	// CUBE 1
	// Movement
	if (state[SDL_SCANCODE_SPACE]) {
	}
	if (state[SDL_SCANCODE_T]) {
		//cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(0.0f, 0.0f, -moveSpeed * deltaTime));
		model->SetWorldPosition(model->GetWorldPosition() + glm::vec3(0.0f, 0.0f, -moveSpeed * deltaTime));
	}
	if (state[SDL_SCANCODE_G]) {
		//cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(0.0f, 0.0f, moveSpeed * deltaTime));
		model->SetWorldPosition(model->GetWorldPosition() + glm::vec3(0.0f, 0.0f, moveSpeed * deltaTime));
	}
	if (state[SDL_SCANCODE_H]) {
		//cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(moveSpeed * deltaTime, 0.0f, 0.0f));
		model->SetWorldPosition(model->GetWorldPosition() + glm::vec3(moveSpeed * deltaTime, 0.0f, 0.0f));
	}
	if (state[SDL_SCANCODE_F]) {
		//cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(-moveSpeed * deltaTime, 0.0f, 0.0f));
		model->SetWorldPosition(model->GetWorldPosition() + glm::vec3(-moveSpeed * deltaTime, 0.0f, 0.0f));
	}
	// Rotation
	if (state[SDL_SCANCODE_E]) {

	}
	if (state[SDL_SCANCODE_Q]) {

	}

	// Light
	// Movement
	if (state[SDL_SCANCODE_I]) {
		blueLight->SetWorldPosition(blueLight->GetWorldPosition() + glm::vec3(0.0f, moveSpeed * deltaTime, 0.0f));
	}
	if (state[SDL_SCANCODE_K]) {
		blueLight->SetWorldPosition(blueLight->GetWorldPosition() + glm::vec3(0.0f, -moveSpeed * deltaTime, 0.0f));
	}
	if (state[SDL_SCANCODE_L]) {
		blueLight->SetWorldPosition(blueLight->GetWorldPosition() + glm::vec3(moveSpeed * deltaTime, 0.0f, 0.0f));
	}
	if (state[SDL_SCANCODE_J]) {
		blueLight->SetWorldPosition(blueLight->GetWorldPosition() + glm::vec3(-moveSpeed * deltaTime, 0.0f, 0.0f));
	}
	// Rotation
	if (state[SDL_SCANCODE_O]) {
		blueLight->SetWorldRotation(glm::vec3(0.0f, 0.0f, 1.0f), blueLight->GetWorldRotationAngle() - moveSpeed * deltaTime);
	}
	if (state[SDL_SCANCODE_U]) {
		blueLight->SetWorldRotation(glm::vec3(0.0f, 0.0f, 1.0f), blueLight->GetWorldRotationAngle() + moveSpeed * deltaTime);
	}

	// VIEW/CAMERA
	if (state[SDL_SCANCODE_W]) {
		cameraList[0]->ProcessKeyboard(FORWARD, 0.01f);
	}
	if (state[SDL_SCANCODE_S]) {
		cameraList[0]->ProcessKeyboard(BACKWARD, 0.01f);
	}
	if (state[SDL_SCANCODE_D]) {
		cameraList[0]->ProcessKeyboard(RIGHT, 0.01f);
	}
	if (state[SDL_SCANCODE_A]) {
		cameraList[0]->ProcessKeyboard(LEFT, 0.01f);
	}

	// Reload Scene
	if (state[SDL_SCANCODE_Z]) {
		sceneManager->EnableSplitscreen(false);
		sceneManager->SwitchScene(new TestScene());
	}

	// demo scene
	if (state[SDL_SCANCODE_X]) {
		sceneManager->SwitchScene(new DemoScene());
	}

	// Reload Scene
	if (state[SDL_SCANCODE_C]) {
		sceneManager->EnableSplitscreen(false);
		sceneManager->SwitchScene(new GameTestScene());
	}
}