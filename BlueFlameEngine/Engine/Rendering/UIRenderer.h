#pragma once
#ifndef UIRENDERER_H
#define UIRENDERER_H

#include "../Graphics/Shader.h" 
#include "../Core/Window.h" 
#include "2D/TextUI.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stb_image.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace ENGINE {

	// Renders all UI objects, seperate from 3D objects
	class UIRenderer
	{
	private:
		// Projection matrix
		glm::mat4 projection;

		// Dimensions
		float width;
		float height;

		// Variables used for debug text
		std::map<GLchar, Character> Characters;
		GLuint VAO, VBO;
		Shader* textShader;

	public:
		UIRenderer();
		~UIRenderer();

		// Initialize
		void Initialize(Window* window);

		// Draw UI elements
		void Draw(Window* window, std::vector<UIObject*> uiObjectList);	

		// Get Screen Dimensions (These DO NOT return the size of the window) only used for UI positions
		float GetWidth();
		float GetHeight();

		// Debug text 
		void SetUpDebugText();
		void DebugText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	};

}
#endif

