#include "PhysicsComponent.h"

#include <iostream>

using namespace ENGINE;

PhysicsComponent::PhysicsComponent() {
	//acceleration = glm::vec3(0.0f, -5.0f, 0.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	SetPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
	SetElasticity(PhysicsComponent::Elastic_Type::NORMAL_ELASTIC);
	SetMaterialType(PhysicsComponent::Material_Type::NORMAL_MATERIAL);
}

PhysicsComponent::~PhysicsComponent() {

}

void PhysicsComponent::Update(float deltaTime) {
	if (physicsType == DYNAMIC) {

		if (deltaTime >= 1.0f / 144.0f) {
			deltaTime = 1.0f / 144.0f;
		}

		if (hasGravity) {
			force += glm::vec3(0.0f, -4000.0f, 0.0f);
		}

		velocity += (invMass * force) * deltaTime;
		position += velocity * deltaTime;
		force = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void PhysicsComponent::Render() {

}

void PhysicsComponent::SetPhysicsType(Physics_Type pt) {
	physicsType = pt;

	if (pt == PhysicsComponent::Physics_Type::STATIC) {
		SetMass(0.0f);
	}
}

void PhysicsComponent::SetElasticity(Elastic_Type et) {
	if (et == PhysicsComponent::Elastic_Type::PERFECT_ELASTIC) {
		restitution = 1.0f;
	}
	else if (et == PhysicsComponent::Elastic_Type::VERY_ELASTIC) {
		restitution = 0.83f;
	}
	else if (et == PhysicsComponent::Elastic_Type::ELASTIC) {
		restitution = 0.67f;
	}
	else if (et == PhysicsComponent::Elastic_Type::NORMAL_ELASTIC) {
		restitution = 0.5f;
	}
	else if (et == PhysicsComponent::Elastic_Type::NON_ELASTIC) {
		restitution = 0.33f;
	}
	else if (et == PhysicsComponent::Elastic_Type::VERY_NON_ELASTIC) {
		restitution = 0.17f;
	}
	else if (et == PhysicsComponent::Elastic_Type::PERFECT_NON_ELASTIC) {
		restitution = 0.0f;
	}
}

void PhysicsComponent::SetMaterialType(Material_Type mt) {
	if (mt == PhysicsComponent::Material_Type::PERFECT_SMOOTH) {
		SetStaticFriction(0.0f);
		SetKineticFriction(0.0f);
	}
	else if (mt == PhysicsComponent::Material_Type::VERY_SMOOTH) {
		SetStaticFriction(0.075f);
		SetKineticFriction(0.002f);
	}
	else if (mt == PhysicsComponent::Material_Type::SMOOTH) {
		SetStaticFriction(0.2f);
		SetKineticFriction(0.13f);
	}
	else if (mt == PhysicsComponent::Material_Type::NORMAL_MATERIAL) {
		SetStaticFriction(0.33f);
		SetKineticFriction(0.27f);
	}
	else if (mt == PhysicsComponent::Material_Type::ROUGH) {
		SetStaticFriction(0.45f);
		SetKineticFriction(0.39f);
	}
	else if (mt == PhysicsComponent::Material_Type::VERY_ROUGH) {
		SetStaticFriction(0.58f);
		SetKineticFriction(0.50f);
	}
	else if (mt == PhysicsComponent::Material_Type::PERFECT_ROUGH) {
		SetStaticFriction(0.71f);
		SetKineticFriction(0.71f);
	}
}

PhysicsComponent::Physics_Type PhysicsComponent::GetPhysicsType() {
	return physicsType;
}

void PhysicsComponent::SetPosition(glm::vec3 pos) {
	position = pos;
}

void PhysicsComponent::SetVelocity(glm::vec3 vel) {
	velocity = vel;
}

void PhysicsComponent::SetAcceleration(glm::vec3 accel) {
	acceleration = accel;
}

void PhysicsComponent::AddForce(glm::vec3 _force) {
	force +=_force * 1000.0f;
}

void PhysicsComponent::SetMass(float _mass) {
	mass = _mass;

	if (_mass == 0)
		invMass = 0;
	else
		invMass = 1 / _mass;
}

void PhysicsComponent::SetRestitution(float _e) {
	restitution = _e;
}

void PhysicsComponent::SetStaticFriction(float _u) {
	staticFriction = _u;
}

void PhysicsComponent::SetKineticFriction(float _u) {
	kineticFriction = _u;
}

glm::vec3 PhysicsComponent::GetPosition() {
	return position;
}

glm::vec3 PhysicsComponent::GetVelocity() {
	return velocity;
}

glm::vec3 PhysicsComponent::GetAcceleration() {
	return acceleration;
}

float PhysicsComponent::GetMass() {
	return mass;
}

float PhysicsComponent::GetInverseMass() {
	return invMass;
}

float PhysicsComponent::GetRestitution() {
	return restitution;
}

float PhysicsComponent::GetStaticFriction() {
	return staticFriction;
}

float PhysicsComponent::GetKineticFriction() {
	return kineticFriction;
}

void PhysicsComponent::SetDestructible(bool b) {
	destructible = b;
}