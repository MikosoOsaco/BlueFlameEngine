#pragma once
#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "Component.h"
#include "../Rendering/3D/Mesh.h"
#include "../Graphics/Shader.h"

namespace ENGINE {

	class RenderComponent : public Component
	{
	public:
		// Render type
		enum Render_Type 
		{
			NONE,
			CUBE,
			QUAD
		};

		// Constructor
		RenderComponent();
		~RenderComponent();

		// Set render type
		void SetRenderType(Render_Type renderType);
		// Set Colour
		void SetColour(float r, float g, float b);

		// Loop functions
		void Update();
		void Render(Shader* shader);

		// Get vertex list
		std::vector<Vertex> getVertexList() { return vertexList; }

		// Set if the object can render
		void CanRender(bool b) { canRender = b; }

	private:
		// Vertex 
		Vertex v;
		std::vector<Vertex> vertexList;
		// Colour
		glm::vec3 colour;
		// Render type
		Render_Type renderType;
		// Mesh
		Mesh* mesh;
		// Can Render
		bool canRender;
	};
}
#endif
