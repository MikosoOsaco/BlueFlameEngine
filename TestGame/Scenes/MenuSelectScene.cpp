#include "MenuSelectScene.h"

using namespace GAME;

MenuSelectScene::MenuSelectScene()
{
}

MenuSelectScene::~MenuSelectScene()
{
	sceneManager = nullptr;
}

bool MenuSelectScene::Initialize()
{
	// Make reference to the scene manager
	sceneManager = BFEngine::GetInstance()->GetSceneManager();
	
	// If you plan to use the scenemanagers scene vector you dont need to set these values all the time
	// Set camera options
	cameraList[0]->Yaw = 0.0f;
	cameraList[0]->Zoom = 45.0f;

	// Set screen options
	sceneManager->EnableSplitscreen(false);
	sceneManager->EnableFullscreen(false);
	sceneManager->ShowFPS(true);
	sceneManager->GetRenderer()->EnableBloom(true);
	
	// Load shaders
	defaultShader = new Shader("Shaders/model.vs", "Shaders/model.fs");
	skyboxShader = new Shader("Shaders/skybox.vs", "Shaders/skybox.fs");

	// Put shaders into shader manager
	defaultShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("default"), defaultShader);
	skyboxShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("skybox"), skyboxShader);

	// Make skybox, load its textures, set properties, and give to the renderer
	skybox = new Skybox();
	std::vector<char*> faces;
	faces.push_back("Resources/Textures/Skyboxes/ame_nebula/right.jpg");
	faces.push_back("Resources/Textures/Skyboxes/ame_nebula/left.jpg");
	faces.push_back("Resources/Textures/Skyboxes/ame_nebula/top.jpg");
	faces.push_back("Resources/Textures/Skyboxes/ame_nebula/bottom.jpg");
	faces.push_back("Resources/Textures/Skyboxes/ame_nebula/back.jpg");
	faces.push_back("Resources/Textures/Skyboxes/ame_nebula/front.jpg");
	skybox->LoadTextures(faces);
	skybox->SetShader(skyboxShaderHandle);

	// UI
	titleText = new TextUI();
	titleText->SetFont("Resources/Fonts/ka1.ttf");
	titleText->SetText("M.E.C.C.");
	titleText->SetColour(1.0, 1.0f, 1.0f);
	titleText->SetSize(0.8f);
	titleText->SetSpacing(9.0f);
	titleText->SetPosition(50, -100);

	buttonTest = new ButtonUI("Resources/Textures/DefaultButton.png");
	buttonTest->SetPosition(-(buttonTest->GetWidth() / 2) - 150, (buttonTest->GetHeight() / 2) + 600);
	buttonTest->SetHeight(100);
	buttonTest->SetPadding(-10, 0);
	buttonTest->GetText()->SetText("TEST");
	buttonTest->SetTextPadding(75, 35);
	buttonTest->GetImage()->SetAlpha(0.5f);

	buttonLighting = new ButtonUI("Resources/Textures/DefaultButton.png");
	buttonLighting->SetPosition(800, buttonTest->GetPosition().y);
	buttonLighting->SetHeight(100);
	buttonLighting->SetPadding(-10, 0);
	buttonLighting->GetText()->SetText("LIGHTING");
	buttonLighting->GetText()->SetSize(0.8f);
	buttonLighting->SetTextPadding(20, 25);
	buttonLighting->GetImage()->SetAlpha(0.5f);

	buttonDemo = new ButtonUI("Resources/Textures/DefaultButton.png");
	buttonDemo->SetPosition(1250, buttonTest->GetPosition().y);
	buttonDemo->SetHeight(100);
	buttonDemo->SetPadding(-10, 0);
	buttonDemo->GetText()->SetText("DEMO");
	buttonDemo->SetTextPadding(60, 35);
	buttonDemo->GetImage()->SetAlpha(0.5f);

	buttonExit = new ButtonUI("Resources/Textures/lewd.png");
	buttonExit->SetPosition(-(buttonTest->GetWidth() / 2) - 150, (buttonTest->GetHeight() / 2) + buttonTest->GetPosition().y + 100);
	buttonExit->SetHeight(100);
	buttonExit->GetText()->SetText("EXIT");
	buttonExit->GetText()->SetColour(0.0f, 0.0f, 0.0f);
	buttonExit->SetTextPadding(75, 35);
	buttonExit->GetImage()->SetAlpha(0.5f);

	sliderTEST = new SliderUI("Resources/Textures/Green.jpg", "Resources/Textures/blackFILL.png");
	sliderTEST->SetPosition(350, 1000);
	sliderTEST->SetHeight(100);
	sliderTEST->SetValue(0.9f);

	//kyouko = new Model("Resources/Models/Kyouko/figyrk0003.obj");
	//kyouko = new Model("Resources/Models/Robot_Base_Greybox/Robot_Var_002_Gurran.obj");
	kyouko = new Model("Resources/Models/Kal (Fire)/Models/Fire_0001.obj");
	kyouko->SetShader(defaultShaderHandle);
	kyouko->physicsComponent->SetPosition(glm::vec3(1.0f, -1.0f, -2.5f));
	kyouko->SetWorldScale(0.008f);
	kyouko->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	kyouko->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	kyouko->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	kyouko->physicsComponent->SetMass(0.0f);

	// Lights
	//
	// Make point light
	pointLight = new Light();
	pointLight->SetShader(defaultShaderHandle);
	pointLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	pointLight->renderComponent->SetColour(1.0f, 1.0f, 1.0f);
	pointLight->renderComponent->CanRender(false);
	pointLight->SetWorldPosition(2.0f, 0.0f, -2.5f);
	pointLight->SetWorldScale(0.5f);
	pointLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	pointLight->lightComponent->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));
	//

	// Make directional light
	dirLight = new Light(LightComponent::Light_Type::DIRECTIONAL);
	dirLight->lightComponent->SetDirection(glm::vec3(1.0f, -1.0f, 1.0f));
	dirLight->lightComponent->SetColour(glm::vec3(0.3f, 0.3f, 0.3f));
	//

	AddUIObject(titleText);
	AddUIObject(buttonTest);
	AddUIObject(buttonDemo);
	AddUIObject(buttonLighting);
	AddUIObject(buttonExit);
	AddUIObject(sliderTEST);
	AddObject(kyouko);
	AddLightObject(dirLight);
	AddLightObject(pointLight);

	return true;
}

void MenuSelectScene::Update(const float deltaTime)
{
	//cameraList[0]->SetRotationY(cameraList[0]->Yaw -= deltaTime * 5);	

	float lastPosition = buttonTest->GetPosition().x;
	buttonTest->SetPosition(lastPosition += deltaTime * 3000, buttonTest->GetPosition().y);
	if (buttonTest->GetPosition().x > (buttonTest->GetWidth() / 2) + 150) {
		buttonTest->SetPosition((buttonTest->GetWidth() / 2) + 150, buttonTest->GetPosition().y);
	}

	float lastPosition2 = buttonExit->GetPosition().x;
	buttonExit->SetPosition(lastPosition += deltaTime * 3000, buttonExit->GetPosition().y);
	if (buttonExit->GetPosition().x > (buttonExit->GetWidth() / 2) + 150) {
		buttonExit->SetPosition((buttonExit->GetWidth() / 2) + 150, buttonExit->GetPosition().y);
	}

	float lastPosition3 = titleText->GetPosition().y;
	titleText->SetPosition(50, lastPosition3 += deltaTime * 2000);
	if (titleText->GetPosition().y > 100) {
		titleText->SetPosition(50, 100);
	}

	//float lastPosition4 = sliderTEST->GetPosition().x;
	//sliderTEST->SetPosition(lastPosition4 += deltaTime * 3000, sliderTEST->GetPosition().y);
	//if (sliderTEST->GetPosition().x > (sliderTEST->GetWidth() / 2) + 150) {
		//sliderTEST->SetPosition((sliderTEST->GetWidth() / 2) + 150, sliderTEST->GetPosition().y);
	//}
	//sliderTEST->Update(deltaTime);
}

void MenuSelectScene::FixedUpdate(const float deltaTime) 
{
	kyouko->SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), kyouko->GetWorldRotationAngle() + 2.0f * deltaTime);
};

void MenuSelectScene::HandleEvents(SDL_Event events)
{
	if (events.jbutton.button == 2) //x button
	{
		sceneManager->SwitchScene(new CharacterSelectScene());
	}

	if (buttonTest->OnHover(events, sceneManager))
	{
		buttonTest->SetSelected(true);
		buttonTest->GetImage()->SetScale(1.2f);
		buttonTest->GetImage()->SetAlpha(1.0f);
		if (events.type == SDL_MOUSEBUTTONDOWN) {
			if (events.button.button == SDL_BUTTON_LEFT)
			{
				buttonTest->GetImage()->SetScale(1.1f);
			}
		}
		if (events.type == SDL_MOUSEBUTTONUP) {
			if (events.button.button == SDL_BUTTON_LEFT)
			{
				if (buttonTest->GetSelected()) {
					sceneManager->SwitchScene(new GameTestScene());
				}
			}
		}
	}
	else
	{
		buttonTest->SetSelected(false);
		buttonTest->GetImage()->SetScale(1);
		buttonTest->GetImage()->SetAlpha(0.5f);
	}

	if (buttonLighting->OnHover(events, sceneManager))
	{
		buttonLighting->SetSelected(true);
		buttonLighting->GetImage()->SetScale(1.2f);
		buttonLighting->GetImage()->SetAlpha(1.0f);
		if (events.type == SDL_MOUSEBUTTONDOWN) {
			if (events.button.button == SDL_BUTTON_LEFT)
			{
				buttonLighting->GetImage()->SetScale(1.1f);
			}
		}
		if (events.type == SDL_MOUSEBUTTONUP) {
			if (events.button.button == SDL_BUTTON_LEFT)
			{
				if (buttonLighting->GetSelected()) {
					sceneManager->SwitchScene(new TestScene());
				}
			}
		}
	}
	else
	{
		buttonLighting->SetSelected(false);
		buttonLighting->GetImage()->SetScale(1);
		buttonLighting->GetImage()->SetAlpha(0.5f);
	}

	if (buttonDemo->OnHover(events, sceneManager))
	{
		buttonDemo->SetSelected(true);
		buttonDemo->GetImage()->SetScale(1.2f);
		buttonDemo->GetImage()->SetAlpha(1.0f);
		if (events.type == SDL_MOUSEBUTTONDOWN) {
			if (events.button.button == SDL_BUTTON_LEFT)
			{
				buttonDemo->GetImage()->SetScale(1.1f);
			}
		}
		if (events.type == SDL_MOUSEBUTTONUP) {
			if (events.button.button == SDL_BUTTON_LEFT)
			{
				if (buttonDemo->GetSelected()) {
					sceneManager->SwitchScene(new DemoScene());
				}
			}
		}
	}
	else
	{
		buttonDemo->SetSelected(false);
		buttonDemo->GetImage()->SetScale(1);
		buttonDemo->GetImage()->SetAlpha(0.5f);
	}

	if (buttonExit->OnHover(events, sceneManager))
	{
		buttonExit->SetSelected(true);
		buttonExit->GetImage()->SetScale(1.2f);
		buttonExit->GetImage()->SetAlpha(1.0f);
		if (events.type == SDL_MOUSEBUTTONDOWN) {
			if (events.button.button == SDL_BUTTON_LEFT)
			{
				buttonExit->GetImage()->SetScale(1.1f);
			}
		}
		if (events.type == SDL_MOUSEBUTTONUP) {
			if (events.button.button == SDL_BUTTON_LEFT)
			{
				if (buttonExit->GetSelected()) {
					sceneManager->SetIsQuit(true);
				}
			}
		}
	}
	else
	{
		buttonExit->SetSelected(false);
		buttonExit->GetImage()->SetScale(1);
		buttonExit->GetImage()->SetAlpha(0.5f);
	}
}

void MenuSelectScene::HandleStates(const Uint8 *state)
{
	if (state[SDL_SCANCODE_Z]) {
		sceneManager->PreviousScene();
	}

	if (state[SDL_SCANCODE_I]) {
		sceneManager->SwitchScene(new TvTGameScene());
	}

	if (state[SDL_SCANCODE_P]) {
		sceneManager->SwitchScene(new CharacterSelectScene());
	}

	if (state[SDL_SCANCODE_O]) {
		sceneManager->SwitchScene(new ShowCaseScene());
	}
}