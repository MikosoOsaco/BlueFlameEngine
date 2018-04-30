#include "CollisionComponent.h"

using namespace ENGINE;

#include <iostream>

CollisionComponent::CollisionComponent() {
	boxPadding = glm::vec3(1.0f, 1.0f, 1.0f);
	spherePadding = 1.0f;
}

CollisionComponent::~CollisionComponent() {
	boundingBox = nullptr;
	delete boundingBox;
}

void CollisionComponent::Update(glm::vec3 pos) {
	if (collisionType == Collision_Type::SPHERE) {
		boundingSphere->SetCentre(pos);
	}
	else if (collisionType == Collision_Type::BOX) {
		boundingBox->SetCentre(pos);
	}
}

void CollisionComponent::Render() {

}

void CollisionComponent::CreateCollisionVolume(Collision_Type ct, std::vector<ModelMesh> meshlist) {
	// Sphere min and max values being set
	if (ct == SPHERE) {
		boundingBox = nullptr;
		boundingSphere = new Sphere();
		collisionType = Collision_Type::SPHERE;
		
		glm::vec3 min, max;

		min.x = meshlist[0].vertices[0].Position.x;
		max.x = meshlist[0].vertices[0].Position.x;
		
		min.y = meshlist[0].vertices[0].Position.y;
		max.y = meshlist[0].vertices[0].Position.y;
		
		min.z = meshlist[0].vertices[0].Position.z;
		max.z = meshlist[0].vertices[0].Position.z;

		for (unsigned int i = 0; i < meshlist.size(); i++) {
			for (unsigned int j = 0; j < meshlist[i].vertices.size(); j++) {
				if (min.x > meshlist[i].vertices[j].Position.x)
					min.x = meshlist[i].vertices[j].Position.x;
					
				if (max.x < meshlist[i].vertices[j].Position.x)
					max.x = meshlist[i].vertices[j].Position.x;
					
				if (min.y > meshlist[i].vertices[j].Position.y)
					min.y = meshlist[i].vertices[j].Position.y;
					
				if (max.y < meshlist[i].vertices[j].Position.y)
					max.y = meshlist[i].vertices[j].Position.y;
					
				if (min.z > meshlist[i].vertices[j].Position.z)
					min.z = meshlist[i].vertices[j].Position.z;
					
				if (max.z < meshlist[i].vertices[j].Position.z)
					max.z = meshlist[i].vertices[j].Position.z;
			}
		}
		// Sphere centre and radius being set
		boundingSphere->SetCentreFromValues(min, max);
		boundingSphere->SetRadiusFromValues(min, max);
	}

	// Box min and max axis values being set
	else if (ct == BOX) {
		boundingSphere = nullptr;
		boundingBox = new Box();
		collisionType = Collision_Type::BOX;

		glm::vec3 min, max;

		min.x = meshlist[0].vertices[0].Position.x;
		max.x = meshlist[0].vertices[0].Position.x;
				
		min.y = meshlist[0].vertices[0].Position.y;
		max.y = meshlist[0].vertices[0].Position.y;
				
		min.z = meshlist[0].vertices[0].Position.z;
		max.z = meshlist[0].vertices[0].Position.z;

		for (unsigned int i = 0; i < meshlist.size(); i++) {
			for (unsigned int j = 0; j < meshlist[i].vertices.size(); j++) {
				if (min.x > meshlist[i].vertices[j].Position.x)
					min.x = meshlist[i].vertices[j].Position.x;
							
				if (max.x < meshlist[i].vertices[j].Position.x)
					max.x = meshlist[i].vertices[j].Position.x;
							
				if (min.y > meshlist[i].vertices[j].Position.y)
					min.y = meshlist[i].vertices[j].Position.y;
							
				if (max.y < meshlist[i].vertices[j].Position.y)
					max.y = meshlist[i].vertices[j].Position.y;
							
				if (min.z > meshlist[i].vertices[j].Position.z)
					min.z = meshlist[i].vertices[j].Position.z;
							
				if (max.z < meshlist[i].vertices[j].Position.z)
					max.z = meshlist[i].vertices[j].Position.z;
			}
		}
		// Box centre and dimensions being set
		boundingBox->SetCentreFromValues(min, max);
		boundingBox->SetExtentsFromValues(min, max);
	}
}

void CollisionComponent::CreateCollisionVolume(Collision_Type ct, std::vector<Vertex> vertexlist) {
	// Sphere min and max values being set
	if (ct == SPHERE) {
		boundingBox = nullptr;
		boundingSphere = new Sphere();
		collisionType = Collision_Type::SPHERE;

		glm::vec3 min, max;

		min.x = vertexlist[0].position.x;
		max.x = vertexlist[0].position.x;
		
		min.y = vertexlist[0].position.y;
		max.y = vertexlist[0].position.y;
		
		min.z = vertexlist[0].position.z;
		max.z = vertexlist[0].position.z;

		for (unsigned int i = 0; i < vertexlist.size(); i++) {
			if (min.x > vertexlist[i].position.x)
				min.x = vertexlist[i].position.x;
				
			if (max.x < vertexlist[i].position.x)
				max.x = vertexlist[i].position.x;
				
			if (min.y > vertexlist[i].position.y)
				min.y = vertexlist[i].position.y;
				
			if (max.y < vertexlist[i].position.y)
				max.y = vertexlist[i].position.y;
				
			if (min.z > vertexlist[i].position.z)
				min.z = vertexlist[i].position.z;
				
			if (max.z < vertexlist[i].position.z)
				max.z = vertexlist[i].position.z;
		}
		// Sphere centre and radius being set
		boundingSphere->SetCentreFromValues(min, max);
		boundingSphere->SetRadiusFromValues(min, max);
	}
	// Box min and max axis values being set
	else if (ct == BOX) {
		boundingSphere = nullptr;
		boundingBox = new Box();
		collisionType = Collision_Type::BOX;

		glm::vec3 min, max;

		min.x = vertexlist[0].position.x;
		max.x = vertexlist[0].position.x;
				
		min.y = vertexlist[0].position.y;
		max.y = vertexlist[0].position.y;
				
		min.z = vertexlist[0].position.z;
		max.z = vertexlist[0].position.z;

		for (unsigned int i = 0; i < vertexlist.size(); i++) {
			if (min.x > vertexlist[i].position.x)
				min.x = vertexlist[i].position.x;
						
			if (max.x < vertexlist[i].position.x)
				max.x = vertexlist[i].position.x;
						
			if (min.y > vertexlist[i].position.y)
				min.y = vertexlist[i].position.y;
						
			if (max.y < vertexlist[i].position.y)
				max.y = vertexlist[i].position.y;
						
			if (min.z > vertexlist[i].position.z)
				min.z = vertexlist[i].position.z;
						
			if (max.z < vertexlist[i].position.z)
				max.z = vertexlist[i].position.z;
		}
		// Box centre and dimensions being set
		boundingBox->SetCentreFromValues(min, max);
		boundingBox->SetExtentsFromValues(min, max);
	}
}

CollisionComponent::Collision_Type CollisionComponent::GetCollisionType() {
	return collisionType;
}

int CollisionComponent::GetLayer() {
	return layer;
}

Box CollisionComponent::GetBoundingBox() {
	return *boundingBox;
}

glm::vec3 CollisionComponent::GetBoundingBoxCentre() {
	return boundingBox->c;
}

glm::vec3 CollisionComponent::GetBoundingBoxRadii() {
	return boundingBox->r * scale;
}

Sphere CollisionComponent::GetBoundingSphere() {
	return *boundingSphere;
}

glm::vec3 CollisionComponent::GetBoundingSphereCentre() {
	return boundingSphere->c;
}

float CollisionComponent::GetBoundingSphereRadius() {
	return boundingSphere->r * scale.x;
}

glm::vec3 CollisionComponent::GetScale() {
	return scale;
}

void CollisionComponent::SetLayer(int l) {
	layer = l;
}

void CollisionComponent::SetBoxPadding(glm::vec3 _padding) {
	boxPadding = _padding;
	boundingBox->SetExtentsFromScale(scale * _padding);
}

void CollisionComponent::SetSpherePadding(float _padding) {
	spherePadding = _padding;
	boundingSphere->SetRadiusFromScale(scale * _padding);
}

void CollisionComponent::SetScale(glm::vec3 _scale) {
	scale = _scale;
}