#pragma once
#ifndef UIOBJECT_H
#define UIOBJECT_H

#include "../../Graphics/Shader.h"
#include <glm/glm.hpp>

namespace ENGINE {

	class UIObject
	{
	protected:
		// object variables
		glm::vec3 position;
		glm::vec3 rotation;
		float rotationAngle;
		glm::vec3 scale;

		// sets whether this object is rendered
		bool visible = true;

	public:
		UIObject();
		virtual ~UIObject();

		// setters
		virtual void SetPosition(const float &x, const float &y);
		virtual void SetPosition(const glm::vec3 &p);
		virtual void SetRotation(const float &x, const float &y, const float &z);
		virtual void SetRotation(const glm::vec3 &r);
		virtual void SetRotationAngle(const float &a);
		virtual void SetScale(const float &x, const float &y);
		virtual void SetScale(const glm::vec3 &s);
		virtual void SetScale(const float &s);
		virtual void SetVisible(bool setVisible);

		// getters
		virtual glm::vec3 GetPosition() const;
		virtual glm::vec3 GetRotation() const;
		virtual float GetRotationAngle() const;
		virtual glm::vec3 GetScale() const;
		virtual bool GetVisible() const;

		virtual void Update(const float deltaTime) = 0;
		virtual void Draw(glm::mat4 projection) = 0;
	};

}
#endif

