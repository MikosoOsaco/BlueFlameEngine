#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "../Graphics/Shader.h" 
#include "../Core/Window.h" 
#include "../Core/Light.h" 
#include "../Core/GameObject.h"
#include "../Core/ResourceManager.h"
#include "../Camera/Camera.h"
#include "3D/Skybox.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace ENGINE {
	// Renders all game objects and the skybox
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		// Initialize
		void Initialize(Window* w);

		// Rendering Functions
		void PreRender(Window* window, Camera* camera, bool splitscreen);
		void Render(Window* window, Camera* camera, const double _interpolation, std::vector<GameObject*> objectList, 
									std::vector<Light*> dirLightList, 
									std::vector<Light*> pointLightList, 
									std::vector<Light*> spotLightList);
		void PostRender(Window* window, bool splitscreen);
		void RenderObjects(glm::mat4 viewMatrix, Camera* camera, const double _interpolation, std::vector<GameObject*> objectList,
																 std::vector<Light*> dirLightList,
																 std::vector<Light*> pointLightList,
																 std::vector<Light*> spotLightList);	
		void RenderObjects(Shader* shader, glm::mat4 viewMatrix, Camera* camera, std::vector<GameObject*> objectList,
			std::vector<Light*> dirLightList,
			std::vector<Light*> pointLightList,
			std::vector<Light*> spotLightList);

		// Skybox
		void RenderSkybox(Skybox* sceneSkybox, Camera* camera);

		// Load Screen
		void RenderLoadScreen();

		// Shader manager
		ResourceManager<Shader>* GetShaderManager();

		// set up framebuffers
		void SetUpFrameBuffers(Window* window, float resolutionScale);

		// Quad (square) to render to screen
		void SetUpQuad(float resolutionScale);
		void RenderQuad();		

		// Use frambuffers
		void PostProcess();
		void UseBloom();

		// framebuffer options
		void EnableBloom(bool set) { useBloom = set; };
		void EnableGreyscale(bool set) { greyscale = set; };
		void EnableInvertedColours(bool set) { inverted = set; };
		void EnableKernel(bool set) { kernel = set; };

		// Used for clean up
		void OnDestroy();

	private:
		// Projection
		glm::mat4 projection;

		// Shader manager
		ResourceManager<Shader>* shaderManager;

		// Framebuffer shaders
		Shader* blur;
		Shader* bloom;
		Shader* screen;

		// Framebuffer options
		bool useBloom = false;
		bool greyscale = false;
		bool inverted = false;
		bool kernel = false;
		bool screenBlur = false;

		// blur amount
		unsigned int blurAmount = 2;
		
		// framebuffer
		unsigned int framebuffer, colorBuffers[2], rboDepth, attachments[2];

		// blur frame buffer
		unsigned int pingpongFBO[2], pingpongColorbuffers[2];

		// quad (square) 
		unsigned int quadVAO;
		unsigned int quadVBO;

		/*
		// meshes
		unsigned int planeVAO;
	public:
		unsigned int depthMapFBO;
		unsigned int depthMap;
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		Shader* shadowShader;
		Shader* simpleDepthShader;
		Shader* debugDepthQuad;
		*/
	};
}

#endif