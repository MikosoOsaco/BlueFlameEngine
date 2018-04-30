#ifndef CUBE_H
#define CUBE_H

#include "../../Core/GameObject.h"
#include "../../Core/RenderComponent.h"
#include "../../Math/CollisionComponent.h"
#include "../../Math/PhysicsComponent.h"
#include "../../Math/PhysicsEngine.h"
#include "../../Graphics/Shader.h"
#include "Projectile.h"

namespace ENGINE {

	class Cube : public GameObject {
	public:
		Cube();
		~Cube();

		void Jump(glm::vec3 vel);
		void AddVelocity(glm::vec3 vel);

		void Fire();

		void Update(const float deltaTime);
		void FixedUpdate(const float deltaTime);
		void Render(Shader* shader, const double _interpolation);

		RenderComponent* renderComponent;
	};

}
#endif
