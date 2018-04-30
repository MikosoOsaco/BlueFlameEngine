#include "Renderer.h"

using namespace ENGINE;

Renderer::Renderer() : shaderManager(nullptr), blur(nullptr), bloom(nullptr), screen(nullptr) {}

Renderer::~Renderer()
{
	OnDestroy();
}

// Initialize
void Renderer::Initialize(Window* window) {
	// Shader manager
	shaderManager = new ResourceManager<Shader>;

	// framebuffer shaders
	blur = new Shader("Shaders/blur.vs", "Shaders/blur.fs");
	bloom = new Shader("Shaders/bloom.vs", "Shaders/bloom.fs");
	screen = new Shader("Shaders/screen.vs", "Shaders/screen.fs");

	// shader options
	blur->Use();
	blur->setInt("image", 0);
	bloom->Use();
	bloom->setInt("scene", 0);
	bloom->setInt("bloomBlur", 1);
	screen->Use();
	screen->setInt("screenTexture", 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*
	shadowShader = new Shader("Shaders/model.vs", "Shaders/model.fs");
	simpleDepthShader = new Shader("Shaders/shadowdepth.vs", "Shaders/shadowdepth.fs");
	debugDepthQuad = new Shader("Shaders/shadowquad.vs", "Shaders/shadowquad.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float planeVertices[] = {
	// positions            // normals         // texcoords
	25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
	-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
	-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

	25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
	-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
	25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
	};
	// plane VAO
	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);


	// configure depth map FBO
	// -----------------------


	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// shader configuration
	// --------------------
	shadowShader->Use();
	shadowShader->setInt("diffuseTexture", 0);
	shadowShader->setInt("shadowMap", 1);
	*/
}

// PreRender sets the projection matrix and clear colour for opengl
void Renderer::PreRender(Window* window, Camera* camera, bool splitscreen) {
	if (splitscreen) {
		projection = glm::perspective(glm::radians(camera->Zoom), ((float)window->GetWidth() / 2) / (float)window->GetHeight(), 0.1f, 100.0f);
	}
	else if (!splitscreen) {
		projection = glm::perspective(glm::radians(camera->Zoom), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 100.0f);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Render sets up the shaders then renders objects
void Renderer::Render(Window* window, Camera* camera, const double _interpolation, std::vector<GameObject*> objectList, std::vector<Light*> dirLightList,
	std::vector<Light*> pointLightList,
	std::vector<Light*> spotLightList)
{
	// update the view matrix based on the camera
	glm::mat4 view = camera->GetViewMatrix();

	// Render objects
	RenderObjects(view, camera, _interpolation, objectList, dirLightList, pointLightList, spotLightList);
}

// PostRender swaps the frame buffer
void Renderer::PostRender(Window* window, bool splitscreen)
{
	PostProcess();
}

// Renders objects from the object list
void Renderer::RenderObjects(glm::mat4 viewMatrix, Camera* camera, const double _interpolation, std::vector<GameObject*> objectList, std::vector<Light*> dirLightList,
	std::vector<Light*> pointLightList,
	std::vector<Light*> spotLightList)
{
	// check if the object list is empty
	if (objectList.size() != NULL) {

		// get the size of the object list and loop through
		unsigned int objectListSize = objectList.size();
		for (unsigned int i = 0; i < objectListSize; ++i) {

			// Make reference to current object
			GameObject* object = objectList.at(i);

			// Chack if can render
			if (object->canRender == true) {

				// Make sure shader exists
				if (shaderManager->get(object->GetShader()) != NULL) {

					// Get object shader
					Shader* shader = shaderManager->get(object->GetShader());
					shader->Use();

					// loop through the light object lists

					// directional lights
					if (dirLightList.size() != NULL) {
						unsigned int dirLightListSize = dirLightList.size();
						for (unsigned int j = 0; j < dirLightListSize; ++j) {
							// Reference to directional light
							Light* dirLight = dirLightList[j];
							shader->setVec3("dirLight[" + std::to_string(j) + "].direction", dirLight->lightComponent->direction);
							shader->setVec3("dirLight[" + std::to_string(j) + "].ambient", dirLight->lightComponent->ambient);
							shader->setVec3("dirLight[" + std::to_string(j) + "].diffuse", dirLight->lightComponent->diffuse);
							shader->setVec3("dirLight[" + std::to_string(j) + "].specular", dirLight->lightComponent->specular);
						}
					}

					// point lights
					if (pointLightList.size() != NULL) {
						unsigned int pointLightListSize = pointLightList.size();
						for (unsigned int j = 0; j < pointLightListSize; ++j) {
							// Reference to point light
							Light* pointLight = pointLightList[j];
							shader->setVec3("pointLight[" + std::to_string(j) + "].position", pointLight->GetWorldPosition());
							shader->setVec3("pointLight[" + std::to_string(j) + "].ambient", pointLight->lightComponent->ambient);
							shader->setVec3("pointLight[" + std::to_string(j) + "].diffuse", pointLight->lightComponent->diffuse);
							shader->setVec3("pointLight[" + std::to_string(j) + "].specular", pointLight->lightComponent->specular);
							shader->setFloat("pointLight[" + std::to_string(j) + "].constant", pointLight->lightComponent->constant);
							shader->setFloat("pointLight[" + std::to_string(j) + "].linear", pointLight->lightComponent->linear);
							shader->setFloat("pointLight[" + std::to_string(j) + "].quadratic", pointLight->lightComponent->quadratic);
						}
					}

					// Account for interpolation
					glm::mat4 interpolatedMatrix;
					interpolatedMatrix = object->GetWorldModelMatrix() * (float)_interpolation + object->GetPreviousWorldMatrix() * (1.0f - (float)_interpolation);

					// Don't interpolate on the first render 
					if (object->firstRender == true) {
						interpolatedMatrix = object->GetWorldModelMatrix();
					}

					// Set shader options
					shader->setMat4("model", interpolatedMatrix * object->GetLocalModelMatrix());
					shader->setMat4("projection", projection);
					shader->setMat4("view", viewMatrix);
					shader->setVec3("viewPos", camera->Position);

					// Render object
					object->Render(shader, _interpolation);

					object->firstRender = false;
				}
			}
		}
	}
}

// Renders objects from the object list
void Renderer::RenderObjects(Shader* shader, glm::mat4 viewMatrix, Camera* camera, std::vector<GameObject*> objectList,
	std::vector<Light*> dirLightList,
	std::vector<Light*> pointLightList,
	std::vector<Light*> spotLightList)
{
	// SHADOW TESTING

	// check if the object list is empty
	if (objectList.size() != NULL) {

		// get the size of the object list and loop through
		unsigned int objectListSize = objectList.size();
		for (unsigned int i = 0; i < objectListSize; ++i) {

			// Make reference to current object
			GameObject* object = objectList.at(i);

			// Make sure shader exists
			if (shaderManager->get(object->GetShader()) != NULL) {

				// Get object shader
				shader->Use();

				// loop through the light object lists

				// directional lights
				if (dirLightList.size() != NULL) {
					unsigned int dirLightListSize = dirLightList.size();
					for (unsigned int j = 0; j < dirLightListSize; ++j) {
						// Reference to directional light
						Light* dirLight = dirLightList[j];
						shader->setVec3("dirLight[" + std::to_string(j) + "].direction", dirLight->lightComponent->direction);
						shader->setVec3("dirLight[" + std::to_string(j) + "].ambient", dirLight->lightComponent->ambient);
						shader->setVec3("dirLight[" + std::to_string(j) + "].diffuse", dirLight->lightComponent->diffuse);
						shader->setVec3("dirLight[" + std::to_string(j) + "].specular", dirLight->lightComponent->specular);
					}
				}

				// point lights
				if (pointLightList.size() != NULL) {
					unsigned int pointLightListSize = pointLightList.size();
					for (unsigned int j = 0; j < pointLightListSize; ++j) {
						// Reference to point light
						Light* pointLight = pointLightList[j];
						shader->setVec3("pointLight[" + std::to_string(j) + "].position", pointLight->GetWorldPosition());
						shader->setVec3("pointLight[" + std::to_string(j) + "].ambient", pointLight->lightComponent->ambient);
						shader->setVec3("pointLight[" + std::to_string(j) + "].diffuse", pointLight->lightComponent->diffuse);
						shader->setVec3("pointLight[" + std::to_string(j) + "].specular", pointLight->lightComponent->specular);
						shader->setFloat("pointLight[" + std::to_string(j) + "].constant", pointLight->lightComponent->constant);
						shader->setFloat("pointLight[" + std::to_string(j) + "].linear", pointLight->lightComponent->linear);
						shader->setFloat("pointLight[" + std::to_string(j) + "].quadratic", pointLight->lightComponent->quadratic);
					}
				}

				// Set shader options
				shader->setMat4("model", object->GetWorldModelMatrix() * object->GetLocalModelMatrix());
				shader->setMat4("projection", projection);
				shader->setMat4("view", viewMatrix);
				shader->setVec3("viewPos", camera->Position);

				// Render object
				object->Render(shader, 0.0f);
			}
		}
	}

}

void Renderer::RenderSkybox(Skybox* sceneSkybox, Camera* camera) {
	// make sure skybox is not null
	if (sceneSkybox != NULL) {
		// remove translation from the view matrix
		glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));

		glDepthFunc(GL_LEQUAL);

		// get skybox shader
		Shader* shader = shaderManager->get(sceneSkybox->GetShader());
		shader->Use();
		shader->setInt("skybox", 0);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);

		// render skybox
		sceneSkybox->Render();

		glDepthFunc(GL_LESS);
	}
}

// Load Screen
void Renderer::RenderLoadScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Shader manager
ResourceManager<Shader>* Renderer::GetShaderManager() {
	return shaderManager;
}

// Clean up
void Renderer::OnDestroy() {
	delete shaderManager;
	shaderManager = nullptr;
	delete blur;
	blur = nullptr;
	delete bloom;
	bloom = nullptr;
	delete screen;
	screen = nullptr;
}

void Renderer::RenderQuad()
{
	// Renders a quad(square) to draw framebuffer onto
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Renderer::SetUpFrameBuffers(Window* window, float resolutionScale)
{
	// set up frame buffers

	// framebuffer
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// 2 color buffers; one for the normal scene and one for blurring
	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, window->GetWidth(), window->GetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}

	// create and attach depth buffer
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window->GetWidth(), window->GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// tell OpenGL which color attachments to use for rendering 
	attachments[0] = { GL_COLOR_ATTACHMENT0 };
	attachments[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// framebuffer for blurring (using a technique called ping-pong)
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, window->GetWidth() * (1 / resolutionScale), window->GetHeight() * (1 / resolutionScale), 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		// check if framebuffers are complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}
}

void Renderer::UseBloom()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	bool horizontal = true, first_iteration = true;
	blur->Use();
	for (unsigned int i = 0; i < blurAmount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		blur->setInt("horizontal", horizontal);
		// bind texture of other framebuffer (or scene if first iteration)
		glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);
		RenderQuad();
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	bloom->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
	// not used cause hdr is disabled (i think it makes everything look washed out)
	//bloom->setFloat("exposure", 1.0f);	
	RenderQuad();
}

void Renderer::PostProcess()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// disable depth test so quad is shown
	glDisable(GL_DEPTH_TEST);
	// clear depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	// screen options
	screen->Use();
	screen->setInt("inverted", inverted);
	screen->setInt("greyscale", greyscale);
	screen->setInt("kernel", kernel);

	// bind colour buffer onto quad
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);

	// Bloom
	if (useBloom && !inverted && !greyscale && !kernel) {
		UseBloom();
	}
	else {
		RenderQuad();
	}
}

void Renderer::SetUpQuad(float resolutionScale) {
	// quad (square) to render on screen
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, resolutionScale,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  resolutionScale, 0.0f,

		-1.0f,  1.0f,  0.0f, resolutionScale,
		1.0f, -1.0f,  resolutionScale, 0.0f,
		1.0f,  1.0f,  resolutionScale, resolutionScale
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}