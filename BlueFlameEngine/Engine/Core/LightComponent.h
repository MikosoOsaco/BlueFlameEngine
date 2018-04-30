#pragma once
#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "Component.h"
#include <glm\glm.hpp>

namespace ENGINE {

	class LightComponent : public Component
	{
	public:
		// Light type
		enum Light_Type 
		{
			NONE,
			DIRECTIONAL,
			POINTLIGHT,
			SPOTLIGHT
		};

		// Constructors
		LightComponent();
		LightComponent(Light_Type lt);
		~LightComponent();

		// Light type get and set
		void SetLightType(Light_Type lt);
		LightComponent::Light_Type GetLightType();

		// Direction get and set
		void SetDirection(glm::vec3 direction);
		glm::vec3 GetDirection() const;

		// Set Colour
		void SetColour(glm::vec3 colour);

		// Light Properties
		glm::vec3 direction;

		float cutOff;
		float outerCutOff;

		float constant;
		float linear;
		float quadratic;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

	private:
		Light_Type lightType = NONE;
	};
}

#endif
