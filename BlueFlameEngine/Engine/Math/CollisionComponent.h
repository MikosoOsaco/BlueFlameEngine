#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include "..\Core\Component.h"
#include "..\Rendering\3D\Mesh.h"
#include "..\Rendering\3D\ModelMesh.h"
#include <iostream>
#include <vector>
#include "Volumes.h"
#include <glm\glm.hpp>

namespace ENGINE {

	class CollisionComponent : public Component {
	public:

		// Collision type defines the volume that will surround the object.

		// Box can be modified for more specific height, width, and length, 
		// but requires more calculations for collisions.
		
		// Sphere can only have a centre and a radius,
		// but makes calculations for collisions easier.
		enum Collision_Type {
			BOX,
			SPHERE
		};

		//Default Constructor sets the padding to to nothing.
		CollisionComponent();
		~CollisionComponent();

		// Basic functions needed by every component.
		// Update is called every frame and requires position, to be set every frame when the object moves.
		// Also infers max and min value changes with regards to position and scale.
		void Update(glm::vec3 pos);

		// Render is called every frame and does nothing for collisions.
		void Render();

		// Both functions create bounding volumes dependant on the collision type,
		// and creates max and min axis values based upon all given vertices of the object.
		// For objects with render components, and manually made shapes, it requires its list of vertices.
		// For objects with model components, with models and meshes, it requires its list of meshes.
		void CreateCollisionVolume(Collision_Type ct, std::vector<Vertex> vertexlist);
		void CreateCollisionVolume(Collision_Type ct, std::vector<ModelMesh> meshlist);

		// Getters for values used in collisions.
		Collision_Type GetCollisionType();
		int GetLayer();

		Box GetBoundingBox();
		glm::vec3 GetBoundingBoxCentre();
		glm::vec3 GetBoundingBoxRadii();
		Sphere GetBoundingSphere();
		glm::vec3 GetBoundingSphereCentre();
		float GetBoundingSphereRadius();
		glm::vec3 GetScale();

		// Setter for layer values
		void SetLayer(int l);

		// Sets the padding to be bigger or smaller than the list of vertices given.
		void SetBoxPadding(glm::vec3 _padding);
		void SetSpherePadding(float _padding);
		void SetScale(glm::vec3 _scale);

	private:

		// Values used in collisions.
		Collision_Type collisionType;
		Box *boundingBox;
		Sphere *boundingSphere;

		// Layer of object; objects on different layers will not collide
		int layer = 0;

		// Padding and scale of the bounding volumes.
		glm::vec3 boxPadding = glm::vec3(1.0f, 1.0f, 1.0f);
		float spherePadding = 1.0f;
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	};
}

#endif
