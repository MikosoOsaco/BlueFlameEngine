#include "SceneManager.h"

using namespace ENGINE;

SceneManager::SceneManager() : currentScene(nullptr), window(nullptr), renderer(nullptr), uiRenderer(nullptr)
{
}

SceneManager::~SceneManager()
{
	delete currentScene;
	currentScene = nullptr;
	delete window;
	window = nullptr;
	delete renderer;
	renderer = nullptr;
	delete uiRenderer;
	uiRenderer = nullptr;
}

// Starts the window, renderer, and sets a default scene with nothing in it (incase no scene is initially set)
void SceneManager::Initialize(Window* w) {
	window = w;

	renderer = new Renderer();
	renderer->Initialize(window);
	renderer->SetUpFrameBuffers(window, resolutionScale);
	renderer->SetUpQuad(resolutionScale);
	cout << "Renderer Initialized" << endl;

	uiRenderer = new UIRenderer();
	uiRenderer->Initialize(window);
	cout << "UI Renderer Initialized" << endl;

	cout << "Default scene added" << endl;
	SwitchScene(new DefaultScene());

	// Load settings for text file
	resolutionScale = (float)Settings::getInstance()->resolutionScale;
	renderer->EnableBloom(Settings::getInstance()->useBloom);	
	ShowFPS(Settings::getInstance()->showFPS);
	EnableFullscreen(Settings::getInstance()->fullscreen);

	loading = false;

	justCPUS = false;
}

// Calls the current scene's update 
void SceneManager::Update(const float deltaTime) {
	this->deltaTime = deltaTime;
	if (!loading) {
		currentScene->PreUpdate(deltaTime);
		currentScene->Update(deltaTime);
		currentScene->LateUpdate(deltaTime);
	}
	loading = false;
}

// Calls the current scene and updates its state
void SceneManager::UpdateState() {
	currentScene->UpdateState();
}

// Calls the current scene's fixedUpdate
void SceneManager::FixedUpdate(const float deltaTime) {
	this->fixedDeltaTime = deltaTime;
	currentScene->PreFixedUpdate(deltaTime);
	currentScene->FixedUpdate(deltaTime);
	currentScene->LateFixedUpdate(deltaTime);
}

// The renderer is split into PreRender, Render, and PostRender
void SceneManager::PreRender() {
	renderer->PreRender(window, currentScene->GetCameraList()[0], splitscreen);
}

void SceneManager::Render(const double _interpolation)
{
	// SPLITSCREEN
	if (splitscreen) {
		// LEFT SCREEN
		glViewport(0, 0, (window->GetWidth() / 2) * resolutionScale, window->GetHeight() * resolutionScale);
		renderer->Render(window, currentScene->GetCameraList()[0], _interpolation, currentScene->GetObjectList(), currentScene->GetDirLightList(), 
																						  currentScene->GetPointLightList(), 
																						  currentScene->GetSpotLightList());
		renderer->RenderSkybox(currentScene->GetSkybox(), currentScene->GetCameraList()[0]);

		// RIGHT SCREEN
		glViewport((window->GetWidth() / 2) * resolutionScale, 0, (window->GetWidth() / 2)* resolutionScale, window->GetHeight() * resolutionScale);
		renderer->Render(window, currentScene->GetCameraList()[1], _interpolation, currentScene->GetObjectList(), currentScene->GetDirLightList(),
																						  currentScene->GetPointLightList(), 
																						  currentScene->GetSpotLightList());
		renderer->RenderSkybox(currentScene->GetSkybox(), currentScene->GetCameraList()[1]);
	}

	// NO SPLITSCREEN
	else if (!splitscreen) {
		glViewport(0, 0, window->GetWidth() * resolutionScale, window->GetHeight() * resolutionScale);
		renderer->Render(window, currentScene->GetCameraList()[0], _interpolation, currentScene->GetObjectList(), currentScene->GetDirLightList(),
																						  currentScene->GetPointLightList(), 
																						  currentScene->GetSpotLightList());
		renderer->RenderSkybox(currentScene->GetSkybox(), currentScene->GetCameraList()[0]);

		/*

		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(glm::vec3(1.0f, 4.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		renderer->simpleDepthShader->Use();
		renderer->simpleDepthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, renderer->SHADOW_WIDTH, renderer->SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, renderer->depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		renderer->RenderObjects(renderer->simpleDepthShader, lightView, currentScene->GetCameraList()[0], currentScene->GetObjectList(), currentScene->GetDirLightList(),
			currentScene->GetPointLightList(),
			currentScene->GetSpotLightList());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// reset viewport
		glViewport(0, 0, window->GetWidth() * resolutionScale, window->GetHeight() * resolutionScale);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		renderer->shadowShader->Use();
		glm::mat4 projection = glm::perspective(glm::radians(currentScene->GetCameraList()[0]->Zoom), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 100.0f);
		glm::mat4 view = currentScene->GetCameraList()[0]->GetViewMatrix();
		renderer->shadowShader->setMat4("projection", projection);
		renderer->shadowShader->setMat4("view", view);
		// set light uniforms
		renderer->shadowShader->setVec3("viewPos", currentScene->GetCameraList()[0]->Position);
		renderer->shadowShader->setVec3("lightPos", glm::vec3(1.0f, 4.0f, 1.0f));
		renderer->shadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, renderer->depthMap);
		renderer->RenderObjects(renderer->shadowShader, view, currentScene->GetCameraList()[0], currentScene->GetObjectList(), currentScene->GetDirLightList(),
			currentScene->GetPointLightList(),
			currentScene->GetSpotLightList());

		*/
	}
}

void SceneManager::PostRender() {
	// Reset viewport
	glViewport(0, 0, window->GetWidth(), window->GetHeight());
	// Render frame buffers
	renderer->PostRender(window, splitscreen);
	// Draw UI
	Draw();
	// Debug text is drawn after so it is not affected by the post process effects
	DrawDebugFPS();
	DrawDebugText();
	// Swap window
	SDL_GL_SwapWindow(window->GetWindow());
}

// Used to render and 2d or UI elements
void SceneManager::Draw() {
	uiRenderer->Draw(window, currentScene->GetUIObjectList());
}

// Handles all input and also calls the current scene's HandleEvent
void SceneManager::HandleEvents() 
{
	while (SDL_PollEvent(&events))
	{
		// If X window button is pressed then quit
		if (events.type == SDL_QUIT) {
			quit = true;
		}

		// Resize window
		if (events.window.event == SDL_WINDOWEVENT_SIZE_CHANGED && events.type != SDL_JOYBUTTONUP && events.type != SDL_JOYBUTTONDOWN) {
			OnResize(events.window.data1, events.window.data2);
			renderer->SetUpFrameBuffers(window, resolutionScale);
		}

		// If you leave the window then mouse is no longer captured
		if (events.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
			if (captureMouse) {
				SDL_CaptureMouse(SDL_FALSE);
				SDL_SetRelativeMouseMode(SDL_FALSE);
			}
		}

		// Capture mouse again when user clicks in the window
		if (events.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
			if (captureMouse) {
				SDL_CaptureMouse(SDL_TRUE);
				SDL_SetRelativeMouseMode(SDL_TRUE);
			}
		}

		currentScene->HandleEvents(events);
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);

	currentScene->HandleStates(state);

	// SCREEN SETTINGS

	if (state[SDL_SCANCODE_V]) {
		window->SetFullScreen(false);
	}

	if (state[SDL_SCANCODE_B]) {
		window->SetFullScreen(true);
	}

	if (state[SDL_SCANCODE_N]) {
		EnableSplitscreen(false);
	}

	if (state[SDL_SCANCODE_M]) {
		EnableSplitscreen(true);
	}	

	if (state[SDL_SCANCODE_1]) {
		renderer->EnableGreyscale(false);
		renderer->EnableInvertedColours(false);
		renderer->EnableKernel(false);
	}

	if (state[SDL_SCANCODE_2]) {
		renderer->EnableGreyscale(true);
	}

	if (state[SDL_SCANCODE_3]) {
		renderer->EnableInvertedColours(true);
	}

	if (state[SDL_SCANCODE_4]) {
		renderer->EnableKernel(true);
	}

	if (state[SDL_SCANCODE_5]) {
		renderer->EnableBloom(true);
	}

	if (state[SDL_SCANCODE_6]) {
		renderer->EnableBloom(false);
	}

	if (state[SDL_SCANCODE_ESCAPE]) {
		quit = true;
	}
}

// Flag indicating the user wants to quit
bool SceneManager::IsQuit() {
	return quit;
}

// Set the quit bool
void SceneManager::SetIsQuit(bool setQuit) {
	quit = setQuit;
}

// Used to switch scenes
void SceneManager::SwitchScene(Scene* scene) {
	loading = true;
	// The new scene gets initialized
	// TODO make this actually delete scene from memory
	currentScene = nullptr;
	delete currentScene;
	currentScene = scene;
	if (currentScene->Initialize()) {
		ClearSceneList();		
		cout << "Scene loaded" << endl;
	}
	else 
	{
		cout << "Scene failed to initialize" << endl;
	}	
}

void SceneManager::AddScene(Scene* scene) {
	if (scene->Initialize()) {
		sceneList.push_back(scene);
		cout << "Scene added" << endl;
	}
	else {
		cout << "Scene failed to be added" << endl;
	}
}

void SceneManager::NextScene() {
	sceneIter++;
	if (sceneList[sceneIter] != NULL) {
		currentScene = sceneList[sceneIter];
		cout << "Next scene displayed" << endl;
	}
}

void SceneManager::PreviousScene() {
	sceneIter--;
	if (sceneList[sceneIter] != NULL) {
		currentScene = sceneList[sceneIter];
		cout << "Previous scene displayed" << endl;
	}
}

void SceneManager::ClearSceneList() {
	sceneIter = 0;
	if (sceneList.size() != 0) {
		for (size_t i = 0; i < sceneList.size(); ++i) {
			// TODO make this actually delete the scenes from memory
			sceneList[i] = nullptr;
			delete sceneList[i];
		}
		sceneList.clear();
	}
}

void SceneManager::StartScene() {
	if (sceneList.size() != 0) {
		currentScene = sceneList[0];
		cout << "Scene loaded" << endl;
	}
	else {
		SwitchScene(new DefaultScene());
	}
}

void SceneManager::OnResize(int w, int h) {
	window->SetWindowSize(w, h);
}

void SceneManager::EnableSplitscreen(bool setSplitscreen) {
	splitscreen = setSplitscreen;
}

void SceneManager::EnableFullscreen(bool setFullscreen) {
	window->SetFullScreen(setFullscreen);
}

void SceneManager::ShowFPS(bool setShowFPS) {
	showFPS = setShowFPS;
}

void SceneManager::CaptureMouse(bool setCaptureMouse) {

	captureMouse = setCaptureMouse;

	if (setCaptureMouse) {
		SDL_CaptureMouse(SDL_TRUE);
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else if (!setCaptureMouse) {
		SDL_CaptureMouse(SDL_FALSE);
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

Renderer* SceneManager::GetRenderer() {
	return renderer;
}

int SceneManager::GetWindowWidth() {
	return window->GetWidth();
}

int SceneManager::GetWindowHeight() {
	return window->GetHeight();
}

float SceneManager::GetScreenWidth() {
	return uiRenderer->GetWidth();
}

float SceneManager::GetScreenHeight() {
	return uiRenderer->GetHeight();
}

void SceneManager::DebugText(std::string string) {
	debugTextList.push_back(string);
}

void SceneManager::DebugText(int value) {
	debugTextList.push_back(std::to_string(value));
}

void SceneManager::DebugText(double value) {
	debugTextList.push_back(std::to_string(value));
}

void SceneManager::DebugText(float value) {
	debugTextList.push_back(std::to_string(value));
}

void SceneManager::DebugText(glm::vec3 value) {
	std::string string;
	string = "X: " + std::to_string(value.x) + " Y: " + std::to_string(value.y) + " Z: " + std::to_string(value.z);
	debugTextList.push_back(string);
}

void SceneManager::DebugText(std::string string, GLfloat x, GLfloat y) {
	uiRenderer->DebugText(string , x, y, 0.4f, glm::vec3(0.0, 1.0f, 0.0f));
}

void SceneManager::DrawDebugText() {
	if (debugTextList.size() != 0) {
		for (size_t i = 0; i < debugTextList.size(); ++i) {
			DebugText(debugTextList[i], 10.0f, debugTextOffset);
			debugTextOffset += 25.0f;
		}
		debugTextList.clear();
		debugTextOffset = 25.0f;
	}
}

void SceneManager::DrawDebugFPS() {
	if (showFPS) {
        loops++;
        fpsTimer -= deltaTime;
        if (fpsTimer <= 0) {
            fpsTimer = fpsTimerSet;
        }

        if (fpsTimeStep >= 1.0f) {
            fpsTimeStep = 0.0f;
            fps = loops;
            loops = 0;
        }
        fpsTimeStep += deltaTime;
        DebugText("FPS: " + std::to_string((int)fps), GetScreenWidth() - 125.0f, 35.0f);
    }
}

Scene* SceneManager::GetCurrentScene() {
	return currentScene;
}

float SceneManager::GetDeltaTime()
{
	return deltaTime;
}