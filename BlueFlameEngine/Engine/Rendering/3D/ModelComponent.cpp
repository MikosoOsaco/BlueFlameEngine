#include "ModelComponent.h"

using namespace ENGINE;

ModelComponent::ModelComponent(string const &path, bool gamma) : gammaCorrection(gamma)
{
	LoadModel(path);
}

void ModelComponent::RecursiveNodeProcess(aiNode* node)
{
	ai_nodes.push_back(node);

	for (unsigned int i = 0; i < node->mNumChildren; i++)
		RecursiveNodeProcess(node->mChildren[i]);
}

void ModelComponent::AnimNodeProcess()
{
	if (scene->mNumAnimations == 0)
	return;

	for (unsigned int i = 0; i < scene->mAnimations[0]->mNumChannels; i++)
	ai_nodes_anim.push_back(scene->mAnimations[0]->mChannels[i]);

}

void ModelComponent::LoadModel(string const &path)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	RecursiveNodeProcess(scene->mRootNode);
	AnimNodeProcess();
	globalInverseTransform = glm::inverse(Bone::AiToGLMMat4(scene->mRootNode->mTransformation));

	//RecursiveProcess(scene->mRootNode, scene);

	// process ASSIMP's root node recursively
	ProcessNode(scene->mRootNode, scene);

	
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		for (unsigned int j = 0; j < scene->mMeshes[i]->mNumBones; j++)
		{
			//Here we're just storing the bone information that we loaded
			//with ASSIMP into the formats our Bone class will recognize.
			std::string b_name = scene->mMeshes[i]->mBones[j]->mName.data;
			glm::mat4 b_mat = glm::transpose(Bone::AiToGLMMat4(scene->mMeshes[i]->mBones[j]->mOffsetMatrix));

			//Print bone name
			std::cout << "Bone " << j << " " << b_name << std::endl;

			//Here we create a Bone Object with the information we've
			//gathered so far, but wait, there's more!
			Bone bone(&meshes.at(i), i, b_name, b_mat);

			//These next parts are simple, we just fill up the bone's
			//remaining data using the functions we defined earlier.
			bone.node = FindAiNode(b_name);
			bone.animNode = FindAiNodeAnim(b_name);

			if (bone.animNode == nullptr)
				std::cout << "No Animations were found for " + b_name << std::endl;

			//Finally, we push the Bone into our vector. Yay.
			bones.push_back(bone);
		}
	}

	//Now we have to fill up the remaining ... remaining data within the
	//bone object, specifically: the pointers to the bone's parent bone.
	for (unsigned int i = 0; i < bones.size(); i++)
	{
		//Here we cycle through the existing bones and match them up with
		//their parents, the code here is pretty self explanatory.
		std::string b_name = bones.at(i).name;
		std::string parent_name = FindAiNode(b_name)->mParent->mName.data;

		Bone* p_bone = FindBone(parent_name);

		bones.at(i).parent_bone = p_bone;

		if (p_bone == nullptr)
			std::cout << "Parent Bone for " << b_name << " does not exist (is nullptr)" << std::endl;
	}
	//I tried combining the above loop with the one above, but this 
	//only resulted in crashes, and my Just-In-Time debugger isn't working,
	//so I'll just leave it as is.

	//Here we only need to give the first Mesh in meshes the skeleton data
	//because in order to initialize the GameObject that will encapsulate this
	//Mesh, we only need one skeleton. The GameObject will copy the skeleton
	//of the first Mesh in its meshes vector and use this as its own.
	//Did that not make sense?
	//Shit.
	//It will later on though, so don't worry.
	if (meshes.size() > 0) {
		meshes.at(0).sceneLoaderSkeleton = new Skeleton(bones, globalInverseTransform);
	}

	if (meshes.size() != 0)    //If there are, in fact, meshes.
		skeleton = meshes.at(0).sceneLoaderSkeleton;
	//We set skeleton to the first Mesh's sceneLoaderSkeleton
	
}

void ModelComponent::ProcessNode(aiNode *node, const aiScene *scene)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}

}

ModelMesh ModelComponent::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
	// data to fill
	vector<VertexM> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		VertexM vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
						  // positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else 
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		if (mesh->HasTangentsAndBitangents()) {
			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}

		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	int WEIGHTS_PER_VERTEX = 4;
	//DEFINING HOW MAY WEIGHTS PER VERTEX,
	//BUT ALSO HOW MANY INDICES PER VERTEX.

	//FIRST YOU HAVE TO CREATE TWO ARRAYS,
	//THESE ARE WHERE WE'LL STORE ALL OF THE VERTEX DATA.
	int boneArraysSize = mesh->mNumVertices*WEIGHTS_PER_VERTEX;
	//THIS IS DONE BECAUSE EACH VERTEX HAS SPACE FOR 4 WEIGHTS AND 4 INDICES.
	std::vector<int> boneIDs;
	boneIDs.resize(boneArraysSize);
	//HERE SPACE FOR 4 INDICES PER VERTEX IS BEING ALLOCATED
	std::vector<float> boneWeights;
	boneWeights.resize(boneArraysSize);
	//HERE SPACE FOR 4 WEIGHTS PER VERTEX IS BEING ALLOCATED

	for (unsigned int i = 0; i < mesh->mNumBones; i++)
	{
		//(above) NOTE THAT mesh IS NOT OF TYPE Mesh,
		//IT IS A POINTER TO THE CURRENT MESH, OF TYPE aiMesh

		aiBone* aiBone = mesh->mBones[i]; //CREATING A POINTER TO THE CURRENT BONE
										  //IT'S IMPORTANT TO NOTE THAT i IS JUST THE ID OF THE CURRENT BONE.

		for (unsigned int j = 0; j < aiBone->mNumWeights; j++)
		{
			aiVertexWeight weight = aiBone->mWeights[j];

			//THIS WILL TELL US WHERE, IN OUR ARRAY, TO START READING THE VERTEX'S WEIGHTS
			unsigned int vertexStart = weight.mVertexId * WEIGHTS_PER_VERTEX;

			//HERE WE'LL ACTUALLY FILL THE ARRAYS, WITH BOTH INDICES AND WEIGHTS.
			for (int k = 0; k < WEIGHTS_PER_VERTEX; k++)
			{
				if (boneWeights.at(vertexStart + k) == 0)
				{
					//(above) IF THE CURRENT BONE WEIGHT IS EQUAL TO 0,
					//THEN IT HASN'T BEEN FILLED YET WITH AN ACTUAL WEIGHT.
					boneWeights.at(vertexStart + k) = weight.mWeight;
					boneIDs.at(vertexStart + k) = i; //REMEMBER THAT i IS JUST THE ID OF THE CURRENT BONE.

													 //NOTE THAT data IS JUST AN ARRAY OF TYPE Vertex, WHERE I STORE ALL OF THE VERTEX INFO.
													 //EACH Vertex CLASS HAS SPACE FOR A POSITION, A UV, A NORMAL, AND 4 INDICES, AND 4 WEIGHTS.
													 //EACH Mesh IS THEN CREATED WITH THIS THIS ARRAY OF Vertex (THIS ARRAY BEING data).

					vertices.at(weight.mVertexId).id[k] = i;
					//SETTING THE ID
					//AT k, OF
					//THE VERTEX AT THIS WEIGHT'S ID,
					//TO THE CURRENT BONE ID.

					vertices.at(weight.mVertexId).weight[k] = weight.mWeight;
					//SETTING THE WEIGHT
					//AT k, OF
					//THE VERTEX AT THIS WEIGHT'S ID,
					//TO THIS WEIGHT'S WEIGHT.
					break;
				}
			}
		}
	}

	// return a mesh object created from the extracted mesh data
	return ModelMesh(vertices, indices, textures);
}

Bone* ModelComponent::FindBone(std::string name)
{
	for (unsigned int i = 0; i < bones.size(); i++)
	{
		if (bones.at(i).name == name)
			return &bones.at(i);
	}
	//This function simply scans our vector bones and checks if
	//any name matches the name we're looking for, if it doesn't
	//find any, we return nullptr.
	//Keep in mind, the bones vector is empty at the point of writing this,
	//but when this function is called it will already be filled up.
	return nullptr;
}

aiNode* ModelComponent::FindAiNode(std::string name)
{
	for (unsigned int i = 0; i < ai_nodes.size(); i++)
	{
		if (ai_nodes.at(i)->mName.data == name)
			return ai_nodes.at(i);
	}
	//This function's purpose is identical, except that instead of Bones,
	//it's looking for an aiNode* inside our ai_nodes vector.
	//This vector has already been filled by our recursiveNodeProcess() function.
	return nullptr;
}

aiNodeAnim* ModelComponent::FindAiNodeAnim(std::string name)
{
	for (unsigned int i = 0; i < ai_nodes_anim.size(); i++)
	{
		if (ai_nodes_anim.at(i)->mNodeName.data == name)
			return ai_nodes_anim.at(i);
	}
	//This function finds the animation with the name we pass in, we called it
	//right after calling our recursiveNodeProcess() function, but this function
	//will only really come into play during the next tutorial, where we cover
	//the actual animation portion of skeletal animation.
	return nullptr;
}

int ModelComponent::FindBoneIDByName(std::string name)
{
	for (unsigned int i = 0; i < bones.size(); i++)
	{
		if (bones.at(i).name == name)
			return i;
	}
	//This function finds the position of a certain bone within our bones vector.
	//This position is equal to the bone's ID, which is vital to determining the
	//rigging of our model within the vertex shader.
	return -1;    //In case we don't find a bone ID, we return -1.
				  //Just to avoid any confusion later on as to whether or not the
				  //ID was found. (It serves the same purpose as returning nullptr).
}

unsigned int ModelComponent::TextureFromFile(const char *path, const string &directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;
	//std::cout << filename << std::endl;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

vector<Texture> ModelComponent::LoadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

void ModelComponent::CanRender(bool b) {
	canRender = b;
}

void ModelComponent::Render(Shader* shader) {
	if (canRender) {
		glm::mat4 t;
		shader->setMat4("gBones", t);
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].Render(shader);
		}
	}
}

void ModelComponent::Update() {
	UpdateSkeleton();
}

void ModelComponent::UpdateSkeleton() {
	if (anim)    //If the object is rigged...
	{
		skeleton->Update();
	}
}