#pragma once
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "../Core/GameObject.h"
#include "../Core/ResourceManager.h"
#include "../Rendering/3D/Mesh.h"
#include "../Graphics/Shader.h"
#include "../Math/Randomizer.h"

namespace ENGINE {

	class ParticleSystem : public GameObject
	{
	public:
		// Constructor
		// Takes a shader manager to add its shader
		ParticleSystem(ResourceManager<Shader>* rm);
		ParticleSystem(ResourceManager<Shader>* rm, glm::vec3 colour);
		~ParticleSystem();

		// Set Colour
		void SetColour(float r, float g, float b) { colour = glm::vec3(r, g, b); };
		void SetColour(glm::vec3 c) { colour = c; };
		void SetColour(float c) { colour = glm::vec3(c, c, c); };

		// Set Life time
		void SetLifeTime(float l) { lifeTime = l; };

		// Set Looping
		void SetLoop(bool l) { loop = l; };

		// Set Size
		void SetSize(float s) { size = s; };

		// Set Alpha
		void SetAlpha(float a) { alpha = a; };

		// Set Glow
		void SetGlow(bool g) { glow = g; };

		// Set Image
		void SetImage(char* filePath) { mesh->LoadTexture(filePath, 1); };

		// Set if can render
		void CanRender(bool b) { canRender = b; }

		// Set if uses image
		void UseImage(bool b) { useImage = b; }

		// Get vertex list
		std::vector<Vertex> getVertexList() { return vertexList; }

		void Update(const float deltaTime);
		void Render(Shader* shader, const double _interpolation);

		void Play();

	private:
		// Vertices and mesh
		Vertex v;
		std::vector<Vertex> vertexList;
		Mesh* mesh;

		// Properties
		glm::vec3 colour;
		float lifeTime;
		bool loop;
		float size;
		float alpha;
		bool glow;
		bool canRender;
		bool useImage;

		// Time
		float time;

		// Shader
		Shader* particleShader;

		// Mesh set up
		void SetUpMesh();
		void SetUpSparkMesh();
	};
}
#endif

