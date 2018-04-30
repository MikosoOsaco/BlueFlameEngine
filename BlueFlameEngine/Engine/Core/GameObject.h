#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../Graphics/Shader.h"
#include "ResourceManager.h"
#include "../Math/CollisionComponent.h"
#include "../Math/PhysicsComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ENGINE {

	class GameObject
	{
	protected:
		// Local variables
		glm::vec3 localPosition;
		glm::vec3 localRotation;
		glm::vec3 localScale;
		float localRotationAngle;

		// World variables
		glm::vec3 worldPosition;
		glm::vec3 worldRotation;
		glm::vec3 worldScale;
		float worldRotationAngle;

		// Model Matrix
		glm::mat4 localModelMatrix = glm::mat4();
		glm::mat4 worldModelMatrix = glm::mat4();
		glm::mat4 previousWorldModelMatrix = glm::mat4();

		// Object Shader
		ResourceHandle<Shader> shader;

	public:
		GameObject();
		virtual ~GameObject();

		// Local getters and setters
		// position
		virtual void SetLocalPosition(const float &x, const float &y, const float &z);
		virtual void SetLocalPosition(const glm::vec3 &p);
		virtual glm::vec3 GetLocalPosition() const;
		// rotation
		virtual void SetLocalRotation(const float &x, const float &y, const float &z, const float &a);
		virtual void SetLocalRotation(const glm::vec3 &r, const float &a);
		virtual glm::vec3 GetLocalRotation() const;
		// scale
		virtual void SetLocalScale(const float &x, const float &y, const float &z);
		virtual void SetLocalScale(const glm::vec3 &s);
		virtual void SetLocalScale(const float &s);
		virtual glm::vec3 GetLocalScale() const;
		// angle
		virtual float GetLocalRotationAngle() const;

		// World getters and setters
		// position
		virtual void SetWorldPosition(const float &x, const float &y, const float &z);
		virtual void SetWorldPosition(const glm::vec3 &p);
		virtual glm::vec3 GetWorldPosition() const;
		// rotation
		virtual void SetWorldRotation(const float &x, const float &y, const float &z, const float &a);
		virtual void SetWorldRotation(const glm::vec3 &r, const float &a);
		virtual glm::vec3 GetWorldRotation() const;
		// scale
		virtual void SetWorldScale(const float &x, const float &y, const float &z);
		virtual void SetWorldScale(const glm::vec3 &s);
		virtual void SetWorldScale(const float &s);
		virtual glm::vec3 GetWorldScale() const;
		// angle
		virtual float GetWorldRotationAngle() const;

		// Update Local Model Matrix
		virtual void UpdateLocalMatrix();
		virtual glm::mat4 GetLocalModelMatrix() const;

		// Update World Model Matrix
		virtual void UpdateWorldMatrix();
		virtual glm::mat4 GetWorldModelMatrix() const;
		virtual void UpdatePreviousModelMatrix();
		virtual glm::mat4 GetPreviousWorldMatrix() const;


		// Shader getter and setter
		ResourceHandle<Shader> GetShader();
		void SetShader(ResourceHandle<Shader> shader);

		// Physics
		CollisionComponent* collisionComponent;
		PhysicsComponent* physicsComponent;

		// Flag for deletion
		bool deleted = false;

		// To be rendered
		bool canRender = true;

		// Checks if first render of the object
		// Used for interpolation
		bool firstRender = true;

		// Loop functions
		virtual void Update(const float deltaTime);
		virtual void UpdateState();
		virtual void FixedUpdate(const float deltaTime);
		virtual void Render(Shader* shader, const double _interpolation);
	};
}
#endif
