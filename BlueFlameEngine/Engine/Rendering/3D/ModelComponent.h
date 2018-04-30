#pragma once
#ifndef MODELOMPONENET_H
#define MODELCOMPOENET_H

#include <glew/GL/glew.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ModelMesh.h"
#include "../../Graphics/Shader.h"
#include "../../Core/Component.h"
#include "../../Graphics/Bone.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

namespace ENGINE {

	class ModelComponent : public Component
	{
	public:

		ModelComponent(string const &path, bool gamma = false);

		// Model Data
		vector<Texture> textures_loaded; // stores all the textures loaded so far, optimization so textures aren't loaded more than once.
		vector<ModelMesh> meshes;
		string directory;
		bool gammaCorrection;

		// Animation data
		vector<aiNode*> ai_nodes;
		vector<aiNodeAnim*> ai_nodes_anim;		
		std::vector<Bone> bones;

		void Update();

		void Render(Shader* shader);
		void CanRender(bool b);

		void RecursiveNodeProcess(aiNode* node);
		void AnimNodeProcess();

		Bone* FindBone(std::string name);
		aiNode* FindAiNode(std::string name);
		aiNodeAnim* FindAiNodeAnim(std::string name);
		int FindBoneIDByName(std::string name);
		glm::mat4 globalInverseTransform;

		bool anim = false;
		Skeleton* skeleton;
		void UpdateSkeleton();

		// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
		void LoadModel(string const &path);

	private:

		const aiScene* scene;

		bool canRender = true;

		
		
		void ProcessNode(aiNode *node, const aiScene *scene); // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).

		ModelMesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
		
		unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

		vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
		// checks all material textures of a given type and loads the textures if they're not loaded yet.
		// the required info is returned as a Texture struct.
	};
}
#endif