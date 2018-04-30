#include "Mesh.h"

using namespace ENGINE;

Mesh::Mesh(std::vector<Vertex>* vertList_)
	: VBO(0), VAO(0), vertexList(std::vector<Vertex>()) {

	vertexList = *vertList_;

	GenerateBuffers();
	LoadTexture("Resources/Textures/default_texture.png", 0);
}

Mesh::~Mesh() {
	//Delete the VAO & VBO from the GPU and clear their data
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Mesh::GenerateBuffers() {

	//Generate the VAO & VBO
	//Pass in the VAO & VBO variable as a pointer to be filled by the GPU
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//Bind the VAO and VBO to the GPU to activate them for use
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Fill the VBO that you activated above with your vertex data
	//(type of Buffer, size of array, pointer to the first item in array, type of draw call)
	glBufferData(GL_ARRAY_BUFFER, vertexList.size() * sizeof(Vertex), &vertexList[0], GL_STATIC_DRAW);

	//Enable a vertex attribute array
	//This will be where we define that data we are passing to our VBO
	glEnableVertexAttribArray(0);

	//Assign thd data to the specific attribute pointer you just made
	//(attribute pointer, number of variables, type of variable, 
	//is it normalized?, size of step to next vert, pointer to the spot in the Vertex this data is stored)
	//POSITION
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	//The offset specifies where in our structure these specific variables are located
	//NORMAL
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	
	
	//TEX COORDS
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textureCoordinates));
	

	//COLOR
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));


	//Clear the vertex array and the buffer so no one else can access it or push to it
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::Render(Shader* shader) {
	shader->Use();
	shader->setInt("texture_diffuse1", 0);
	shader->setInt("texture_specular1", 0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Bind the VAO that you want to use for drawing
	glBindVertexArray(VAO);

	//Draw the array stored in the bound VAO
	//(type of render, start of array, end of array)
	glDrawArrays(GL_TRIANGLES, 0, vertexList.size());

	//Clear the vertex array for future use
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::RenderPoints(Shader* shader) {
	shader->Use();
	shader->setInt("texture_diffuse1", 0);
	shader->setInt("texture_specular1", 0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Bind the VAO that you want to use for drawing
	glBindVertexArray(VAO);

	//Draw the array stored in the bound VAO
	//(type of render, start of array, end of array)
	glDrawArrays(GL_POINTS, 0, vertexList.size());

	//Clear the vertex array for future use
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::LoadTexture(char* path, int id) {
	/// Bind the first one
	glBindTexture(GL_TEXTURE_2D, textureID);

	SDL_Surface *textureSurface = IMG_Load(path);
	if (textureSurface == nullptr) {
		//return false;
	}
	/// Are we using alpha? Not in jpeg but let's be careful
	int mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;

	/// Wrapping and filtering options
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	/// Load the texture data from the SDL_Surface to the GPU memmory
	glTexImage2D(GL_TEXTURE_2D, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	/// Release the memory
	SDL_FreeSurface(textureSurface); /// let go of the memory
}

void Mesh::LoadTextures(std::vector<char*> faces) {

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (unsigned int i = 0; i < faces.size(); i++) {
		SDL_Surface *textureSurface = IMG_Load(faces[i]);
		if (textureSurface == nullptr) {
			std::cout << IMG_GetError() << std::endl;
			//return false;			
		}
		/// Are we using alpha? Not in jpeg but let's be careful
		int mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;

		/// Wrapping and filtering options
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		/// Load the texture data from the SDL_Surface to the GPU memmory
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
		/// Release the memory
		SDL_FreeSurface(textureSurface); /// let go of the memory
	}
}