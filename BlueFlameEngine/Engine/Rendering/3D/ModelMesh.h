#pragma once
#ifndef MODELMESH_H
#define MODELMESH_H

#include <glew/GL/glew.h> // holds all OpenGL type declarations

#include <assimp/Importer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Graphics/Shader.h"
#include "../../Graphics/Bone.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
namespace ENGINE {

	struct VertexM {
		// position
		glm::vec3 Position;
		// normal
		glm::vec3 Normal;
		// texCoords
		glm::vec2 TexCoords;
		// tangent
		glm::vec3 Tangent;
		// bitangent
		glm::vec3 Bitangent;
		// colour
		glm::vec3 Colour;

		float weight[4];
		unsigned int id[4];
	};

	struct Texture {
		unsigned int id;
		string type;
		aiString path;
	};

	class ModelMesh {
	public:
		/*  Mesh Data  */
		vector<VertexM> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
		unsigned int VAO;

		vector<float> vecWeights;
		vector<GLint> vecIDs;
		Skeleton* sceneLoaderSkeleton;

		/*  Functions  */
		// constructor
		ModelMesh(vector<VertexM> vertices, vector<unsigned int> indices, vector<Texture> textures);

		// render the mesh
		void Render(Shader* shader);

	private:
		/*  Render data  */
		unsigned int VBO, EBO;

		/*  Functions    */
		// initializes all the buffer objects/arrays
		void setupMesh();
		
	};
}
#endif