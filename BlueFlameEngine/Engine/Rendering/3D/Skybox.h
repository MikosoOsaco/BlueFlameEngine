#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include "../../Core/GameObject.h"
#include <glew/GL/glew.h>
#include <SDL/SDL_image.h>
#include <vector>
#include <iostream>

namespace ENGINE {

	class Skybox : public GameObject
	{
	public:
		Skybox();
		~Skybox();

		// Load the faces of the cubes by giving a vector of file paths
		void LoadTextures(std::vector<char*> faces);

		void Update(const float deltaTime);
		void Render();

	private:
		void LoadMesh();

		unsigned int VAO, VBO, textureID;
		std::vector<char*> faces;
	};

}
#endif

