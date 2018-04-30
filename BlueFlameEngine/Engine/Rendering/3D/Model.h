#ifndef MODEL_H
#define MODEL_H

#include "../../Core/GameObject.h"
#include "../../Graphics/Shader.h"
#include "ModelComponent.h"
#include "../../Math/CollisionComponent.h"
#include "../../Math/PhysicsComponent.h"
#include <iostream>

namespace ENGINE {

	class Model : public GameObject
	{
	public:
		Model();
		Model(std::string path);
		Model(std::string path, bool check);
		~Model();

		void Update(const float deltaTime);
		void FixedUpdate(const float deltaTime);
		void Render(Shader* shader, const double _interpolation);

		ModelComponent* model;
	};
}

#endif

