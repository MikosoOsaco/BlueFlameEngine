#pragma once
#ifndef TEXTUI_H
#define TEXTUI_H

#include <glew/GL/glew.h>
#include "UIObject.h"
#include <iostream>
#include <string>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace ENGINE {

	struct Character {
		GLuint TextureID;   // ID handle of the glyph texture
		glm::ivec2 Size;    // Size of glyph
		glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
		GLuint Advance;     // Horizontal offset to advance to next glyph
	};

	class TextUI : public UIObject
	{
	private: 
		std::string text;
		std::string font;
		glm::vec3 colour;
		float size;
		float alpha;
		float spacing;

		std::map<GLchar, Character> Characters;
		GLuint VAO, VBO;

		Shader* shader;

		void SetUp();

	public:
		TextUI();
		~TextUI();

		void SetText(std::string setText);
		void SetColour(glm::vec3 setColour);
		void SetColour(float x, float y, float z);
		void SetFont(std::string setFont);
		void SetSize(float setSize);
		void SetAlpha(float setAlpha);
		void SetSpacing(float setSpacing);

		float GetLength();
		float GetSize();

		void Update(const float deltaTime) override;
		void Draw(glm::mat4 projection) override;
	};

}
#endif

