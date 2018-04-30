#ifndef MESH_H
#define MESH_H

#include <glew\GL\glew.h>
#include <vector>
#include <glm\glm.hpp>
#include <SDL\SDL_image.h>
#include "../../Graphics/Shader.h"

namespace ENGINE {

	//Create a structure that defines what a vertex will have within our program
	//A Struct is a class-like object where all member variables and functions are public
	//Today we will just be starting with position data for the vertext
	//but in the future we will be adding normals and texture coordinates
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color; //This will be a range from 0-1 instead of 0-255
		glm::vec2 textureCoordinates;
	};

	class Mesh {
	public:
		Mesh(std::vector<Vertex>* vertexList_);
		~Mesh();

		//This will be used to draw the mesh
		void Render(Shader* shader);
		void RenderPoints(Shader* shader);

		void LoadTexture(char* path, int id);
		void LoadTextures(std::vector<char*> faces);

	private:
		void GenerateBuffers();

		

		//VAO: Vertex Array Object
		//The VAO will hold all of our data for the shape's vertices that we define
		//VBO: Vertex Buffer Object
		//The VBO will be the space where our shape is stored on the GPU
		//GLuints will be the addresses as to where this data is stored on the GPU
		GLuint VAO, VBO;
		std::vector<Vertex> vertexList;
		unsigned int textureID;
	};
}

#endif MESH_H