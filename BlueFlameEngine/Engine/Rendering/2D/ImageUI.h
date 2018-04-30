#pragma once
#ifndef IMAGEUI_H
#define IMAGEUI_H

#include <iostream>
#include <string>
#include <glm\glm.hpp>
#include "UIObject.h"
#include <stb_image.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL\SDL_image.h>

namespace ENGINE {

	class ImageUI : public UIObject
	{
	public:
		ImageUI();
		~ImageUI();

		void SetWidth(const int width);
		void SetHeight(const int height);
		void SetImage(const std::string filePath);
		void SetColour(const glm::vec3 colour);
		void SetAlpha(const float alpha);
		void SetName(const std::string name);
		void SetPixelData();

		int GetWidth() const;
		int GetHeight() const;
		std::string GetImagePath() const;
		glm::vec3 GetColour() const;
		float GetAlpha() const;
		std::string GetName() const;
		std::string GetPixelData()const;

		void Update(const float deltatime);
		void Draw(glm::mat4 projection);

		glm::mat4 projection;

	private:
		int width;
		int height;
		std::string filePath;
		glm::vec3 colour;
		float alpha;
		std::string name;

		Shader* shader;

		unsigned int LoadTexture(char const * path);
		void SetUp();

		unsigned int textureID, VAO, VBO;
	};

}

#endif

