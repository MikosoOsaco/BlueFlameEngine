#include "ImageUI.h"

using namespace ENGINE;

ImageUI::ImageUI() : width(1), height(1), colour(0.0f, 0.0f, 0.0f), alpha(1.0f)
{
	shader = new Shader("Shaders/image.vs", "Shaders/image.fs");
	SetUp();
}

ImageUI::~ImageUI()
{
}

void ImageUI::SetWidth(const int width) {
	this->width = width;
}

void ImageUI::SetHeight(const int height) {
	this->height = height;
}

void ImageUI::SetImage(const std::string filePath) {
	this->filePath = filePath;
	textureID = LoadTexture(filePath.c_str());
	SetUp();
}

void ImageUI::SetColour(const glm::vec3 colour) {
	this->colour = colour;
}

void ImageUI::SetAlpha(const float alpha) {
	this->alpha = alpha;
}

void ImageUI::SetName(const std::string name) {
	this->name = name;
}

void ImageUI::SetPixelData() {
	std::cout << "lul why would you need this" << std::endl;
}

int ImageUI::GetWidth() const {
	return width;
}

int ImageUI::GetHeight() const {
	return height;
}

std::string ImageUI::GetImagePath() const {
	return filePath;
}

glm::vec3 ImageUI::GetColour() const {
	return colour;
}

float ImageUI::GetAlpha() const {
	return alpha;
}

std::string ImageUI::GetName() const {
	return name;
}

std::string ImageUI::GetPixelData() const {
	return "why";
}

void ImageUI::Update(const float deltatime) {

}

void ImageUI::Draw(glm::mat4 projection) {
	glClear(GL_DEPTH_BUFFER_BIT);

	shader->Use();

	shader->setVec3("colour", colour);
	shader->setFloat("alpha", alpha);
	shader->setMat4("projection", projection);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glm::mat4 model = glm::mat4();
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(width, height, 0.0f) * scale);
	model = glm::rotate(model, rotationAngle, rotation);
	shader->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int ImageUI::LoadTexture(char const * path)
{
	SDL_Surface *textureSurface = IMG_Load(path);
	if (textureSurface == nullptr) {
		return false;
	}
	/// Are we using alpha? Not in jpeg but let's be careful
	int mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	/// Wrapping and filtering options
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	/// Load the texture data from the SDL_Surface to the GPU memmory
	glTexImage2D(GL_TEXTURE_2D, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	/// Release the memory

	width = textureSurface->w;
	height = textureSurface->h;

	SDL_FreeSurface(textureSurface); /// let go of the memory

	return textureID;
}

void ImageUI::SetUp() {
	shader->Use();

	float vertices[] = {
		// positions         
		-0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}