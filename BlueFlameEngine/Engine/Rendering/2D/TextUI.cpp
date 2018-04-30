#include "TextUI.h"

using namespace ENGINE;

TextUI::TextUI()
{
	text = "";
	position = glm::vec3(25.0f, 25.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(0.0f, 0.0f, 0.0f);
	font = "Resources/Fonts/arial.ttf";
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	size = 1.0f;
	alpha = 1.0f;
	spacing = 0.0f;

	shader = new Shader("Shaders/textshader.vs", "Shaders/textshader.fs");

	SetUp();
}

TextUI::~TextUI()
{
}

void TextUI::SetUp()
{
	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, font.c_str(), 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 96);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
			);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<GLuint>(face->glyph->advance.x)
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextUI::SetText(std::string setText)
{
	text = setText;
}

void TextUI::SetColour(glm::vec3 setColour)
{
	colour = setColour;
}

void TextUI::SetColour(float x, float y, float z)
{
	colour = glm::vec3(x, y, z);
}

void TextUI::SetFont(std::string setFont)
{
	font = setFont;
	Characters.clear();
	SetUp();
}

void TextUI::SetSize(float setSize)
{
	size = setSize;
}

void TextUI::SetAlpha(float setAlpha)
{
	alpha = setAlpha;
}

void TextUI::SetSpacing(float setSpacing)
{
	spacing = setSpacing;
}

float TextUI::GetLength() {
	float length = 0.0f;

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];
		length += ((ch.Advance >> 6) + spacing)* size;
	}
	return length;
}

float TextUI::GetSize()
{
	return size;
}

void TextUI::Update(const float deltaTime)
{

}

void TextUI::Draw(glm::mat4 projection)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	float x = position.x;
	float y = position.y;
	// Activate corresponding render state	
	shader->Use();
	shader->setMat4("projection", projection);
	glUniform3f(glGetUniformLocation(shader->GetShaderProgram(), "textColor"), colour.x, colour.y, colour.z);
	glUniform1f(glGetUniformLocation(shader->GetShaderProgram(), "alpha"), alpha);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * size;
		GLfloat ypos = y + (ch.Size.y - ch.Bearing.y) * size;

		GLfloat w = ch.Size.x * size;
		GLfloat h = ch.Size.y * size;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos - h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos - h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos - h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += ((ch.Advance >> 6) + spacing) * size; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}