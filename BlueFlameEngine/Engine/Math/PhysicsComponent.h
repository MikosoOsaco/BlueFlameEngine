#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "..\Core\Component.h"

#include <glm\glm.hpp>

namespace ENGINE {

	class PhysicsComponent : public Component {
	public:

		// Physics type defines how physics will be applied to the object.
		// Dynamic bodies are affected by forces, and have mass.
		// Static bodies are not affected by forces, and have infinite, or zero mass.
		enum Physics_Type {
			DYNAMIC,
			STATIC
		};

		// Elastic type defines how velocity will be dispersed when there is a collision.
		// The more elastic an object is, the more velocity will be transferred to the other object.
		// The less elastic an object is, the less velocity will be transferred to the other object.
		// The lowest elasticity of the two colliding objects will be used.
		enum Elastic_Type {
			PERFECT_ELASTIC,
			VERY_ELASTIC,
			ELASTIC,
			NORMAL_ELASTIC,
			NON_ELASTIC,
			VERY_NON_ELASTIC,
			PERFECT_NON_ELASTIC
		};

		// Material type defines how much friction will be applied when touching other objects.
		// The smoother an objects surface is, the less amount of friction will be applied.
		// The rougher an objects surface is, the more amount of friction will be applied.
		// This is split up into two types of friction; static friction, and kinematic friction.
		// Static friction is applied when the object is at rest, 
		// and kinematic friction is applied when the object is in motion.
		// The overall static or kinematic friction of the two objects is calculated with pythagorean theorem,
		// where overall friction = the square root of (first objects friction ^ 2 + second objects friction ^ 2).
		enum Material_Type {
			PERFECT_SMOOTH,
			VERY_SMOOTH,
			SMOOTH,
			NORMAL_MATERIAL,
			ROUGH,
			VERY_ROUGH,
			PERFECT_ROUGH
		};

		// Basic constructor of Physics Component applies gravity, sets the physics type to dynamic, and sets the elastic and material type to normal
		PhysicsComponent();
		~PhysicsComponent();

		// Basic functions needed by every component
		// Update is called every frame and requires deltaTime to calculate position changes
		void Update(float deltaTime);

		// Render is called every frame and does nothing for physics
		void Render();

		// Getters and setters for types used in collision calculation
		void SetPhysicsType(Physics_Type pt);
		void SetElasticity(Elastic_Type et);
		void SetMaterialType(Material_Type mt);

		PhysicsComponent::Physics_Type GetPhysicsType();

		// Getters and setters for values used in movement, and collision calculation
		void SetPosition(glm::vec3 pos);
		void SetVelocity(glm::vec3 vel);
		void SetAcceleration(glm::vec3 accel);
		void AddForce(glm::vec3 _force);

		void SetMass(float _mass);
		void SetRestitution(float _e);
		void SetStaticFriction(float _u);
		void SetKineticFriction(float _u);

		glm::vec3 GetPosition();
		glm::vec3 GetVelocity();
		glm::vec3 GetAcceleration();

		float GetMass();
		float GetInverseMass();
		float GetRestitution();
		float GetStaticFriction();
		float GetKineticFriction();

		void SetDestructible(bool b);

	//private:

		// Values used in movement, and collision calculation
		glm::vec3 previousPosition;
		glm::vec3 position;
		glm::vec3 previousVelocity;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		glm::vec3 force;

		float angularVelocity;
		float torque;
		float orientation;

		float inertia;
		float invInertia;
		float mass;
		float invMass;

		float restitution;
		float staticFriction;
		float kineticFriction;

		bool hasGravity = true;

		bool destructible = false;

		Physics_Type physicsType;
	};
}

#endif