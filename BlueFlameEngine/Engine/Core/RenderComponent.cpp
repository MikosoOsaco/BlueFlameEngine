#include "RenderComponent.h"

using namespace ENGINE;

RenderComponent::RenderComponent() : mesh(nullptr), canRender(true), renderType(NONE), colour (glm::vec3(0.5f, 0.5f, 0.5f))
{

}
RenderComponent::~RenderComponent()
{

}
void RenderComponent::SetRenderType(Render_Type rt) 
{
	if (rt == NONE) {
		renderType = NONE;
		vertexList.clear();
	}
	else if (rt == CUBE) {
		renderType = CUBE;
		vertexList.clear();
		v.color = colour;		
		// SIDE 1
		// BACKSIDE FROM CAMERA
		v.normal = glm::vec3(0.0f, 0.0f, -1.0f);
		v.position = glm::vec3(-0.5f, -0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(0, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(1, 0);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(1, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(1, 0);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, -0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(0, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(0, 0);
		vertexList.push_back(v);
		// SIDE 2
		// FRONT FACING CAMERA
		v.normal = glm::vec3(0.0f, 0.0f, 1.0f);
		v.position = glm::vec3(-0.5f, -0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(0, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(1, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(1, 0);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(1, 0);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(0, 0);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, -0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(0, 1);
		vertexList.push_back(v);
		// SIDE 3
		// LEFT SIDE
		v.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
		v.position = glm::vec3(-0.5f, 0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(1, 0);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(1, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, -0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(0, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, -0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(0, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, -0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(0, 0);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(1, 0);
		vertexList.push_back(v);
		// SIDE 4
		// RIGHT SIDE
		v.normal = glm::vec3(1.0f, 0.0f, 0.0f);
		v.position = glm::vec3(0.5f, 0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(1, 0);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(0, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(1, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(0, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(1, 0);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(0, 0);
		vertexList.push_back(v);
		// SIDE 5
		// BOTTOM
		v.normal = glm::vec3(0.0f, -1.0f, 0.0f);
		v.position = glm::vec3(-0.5f, -0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(0, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(1, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(1, 0);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(1, 0);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, -0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(0, 0);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, -0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(0, 1);
		vertexList.push_back(v);
		// SIDE 6
		// TOP
		v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
		v.position = glm::vec3(-0.5f, 0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(0, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(1, 0);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(1, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(1, 0);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, -0.5f);
		v.textureCoordinates = glm::vec2(0, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, 0.5f);
		v.textureCoordinates = glm::vec2(0, 0);
		vertexList.push_back(v);
		
		mesh = new Mesh(&vertexList);
	}
	else if (rt == QUAD) {
		renderType = QUAD;
		vertexList.clear();
		v.color = colour;

		v.normal = glm::vec3(0.0f, 0.0f, 1.0f);
		v.position = glm::vec3(-0.5f, -0.5f, 0.0f);
		v.textureCoordinates = glm::vec2(0, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, 0.0f);
		v.textureCoordinates = glm::vec2(1, 1);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, 0.0f);
		v.textureCoordinates = glm::vec2(1, 0);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, 0.0f);
		v.textureCoordinates = glm::vec2(1, 0);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, 0.0f);
		v.textureCoordinates = glm::vec2(0, 0);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, -0.5f, 0.0f);
		v.textureCoordinates = glm::vec2(0, 1);
		vertexList.push_back(v);

		mesh = new Mesh(&vertexList);
	}
	else {

	}
}
void RenderComponent::SetColour(float r, float g, float b)
{
	colour = glm::vec3(r, g, b);
	SetRenderType(renderType);
}
void RenderComponent::Update() 
{

}
void RenderComponent::Render(Shader* shader) {
	if (canRender) {
		if (mesh != nullptr) {
			mesh->Render(shader);
		}
	}
}