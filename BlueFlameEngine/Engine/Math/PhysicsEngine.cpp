#include "PhysicsEngine.h"
#include <iostream>

using namespace ENGINE;

std::unique_ptr<PhysicsEngine> PhysicsEngine::PhysicsEngineInstance(nullptr);


PhysicsEngine::PhysicsEngine() {

}

PhysicsEngine::~PhysicsEngine() {

}

void PhysicsEngine::Update(float deltaTime) {
	int size = contactList.size();

	if (size != NULL) {
		for (int i = 0; i < size; ++i) {
			// If Objects Collide, correct penatration and apply impulse
			if (isColliding(contactList.at(i))) {
				PenatrationCorrection(contactList.at(i));
				ApplyImpulse(contactList.at(i));
			}
		}
	}
}

PhysicsEngine* PhysicsEngine::GetInstance() {
	if (PhysicsEngineInstance.get() == nullptr) {
		PhysicsEngineInstance.reset(new PhysicsEngine());
	}
	return PhysicsEngineInstance.get();
}

void PhysicsEngine::AddObjectList(std::vector<GameObject*> physicsObjectList) {
	GeneratePairs(physicsObjectList);
}

void PhysicsEngine::GeneratePairs(std::vector<GameObject*> physicsObjectList) {
	contactList.clear();
	int size = physicsObjectList.size();
	for (int i = 0; i < size; ++i) {
		for (int j = i + 1; j < size; ++j) {

			// Check if object has an active physics component
			if (physicsObjectList.at(i)->physicsComponent == nullptr || physicsObjectList.at(j)->physicsComponent == nullptr) {
				continue;
			}

			// Check if object has an active physics component
			if (physicsObjectList.at(i)->collisionComponent == nullptr || physicsObjectList.at(j)->collisionComponent == nullptr) {
				continue;
			}

			// Checks that both objects are on the same layer
			if (physicsObjectList.at(i)->collisionComponent->GetLayer() != physicsObjectList.at(j)->collisionComponent->GetLayer()) {
				continue;
			}

			// Checks that both objects are not static
			if (physicsObjectList.at(i)->physicsComponent->GetPhysicsType() == PhysicsComponent::Physics_Type::STATIC &&
				physicsObjectList.at(j)->physicsComponent->GetPhysicsType() == PhysicsComponent::Physics_Type::STATIC) {
				continue;
			}

			// Once pair passes all tests, push into the list
			contactList.push_back(ContactData::CreateContactData(physicsObjectList.at(i), physicsObjectList.at(j)));
		}
	}
}

bool PhysicsEngine::isColliding(ContactData *c) {

	// If statements differentiate which type of colliders want to collide, and does the appropriate collision check

	if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {

		//// Calculates squared distance between centres
		//glm::vec3 d = c->A->collisionComponent->GetBoundingSphereCentre() + c->B->collisionComponent->GetBoundingSphereCentre();
		//float d2 = glm::dot(d, d);

		//// If the squared distance is less than the squared sum of radii, spheres intersect
		//float radiusSum = c->A->collisionComponent->GetBoundingSphereRadius() + c->B->collisionComponent->GetBoundingSphereRadius();
		//if (d2 <= radiusSum * radiusSum) {
		//	return true;
		//}
		//else {
		//	return false;
		//}

		glm::vec3 d;
		float r;
		int result = 0;
		float s;
		glm::vec3 v1, v2;
		float vrn;

		r = c->A->collisionComponent->GetBoundingSphereRadius() + c->B->collisionComponent->GetBoundingSphereRadius();
		d = c->A->collisionComponent->GetBoundingSphereCentre() - c->B->collisionComponent->GetBoundingSphereCentre();
		s = d.length() - r;

		glm::normalize(d);
		c->normal = d;

		v1 = c->A->physicsComponent->velocity;
		v2 = c->B->physicsComponent->velocity;


	}
	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {

		// Find point p on AABB closest to the centre of the sphere
		glm::vec3 p;
		ClosestPointOnAABB(c->A->collisionComponent->GetBoundingSphereCentre(), c->B->collisionComponent->GetBoundingBoxCentre(), c->B->collisionComponent->GetBoundingBoxRadii(), p);

		// If the squared distance from sphere centre to point p is less than the squared sphere radius, box and sphere intersect
		glm::vec3 v = p - c->A->collisionComponent->GetBoundingSphereCentre();
		if (glm::dot(v, v) <= c->A->collisionComponent->GetBoundingSphereRadius() * c->A->collisionComponent->GetBoundingSphereRadius()) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {

		// Find point p on AABB closest to the centre of the sphere
		glm::vec3 p;
		ClosestPointOnAABB(c->B->collisionComponent->GetBoundingSphereCentre(), c->A->collisionComponent->GetBoundingBoxCentre(), c->A->collisionComponent->GetBoundingBoxRadii(), p);

		// If the squared distance from sphere centre to point p is less than the squared sphere radius, box and sphere intersect
		glm::vec3 v = p - c->B->collisionComponent->GetBoundingSphereCentre();
		if (glm::dot(v, v) <= c->B->collisionComponent->GetBoundingSphereRadius() * c->B->collisionComponent->GetBoundingSphereRadius()) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {

		// If no intersection along an axis, boxes are not colliding
		if (glm::abs(c->A->collisionComponent->GetBoundingBoxCentre().x - c->B->collisionComponent->GetBoundingBoxCentre().x) > (c->A->collisionComponent->GetBoundingBoxRadii().x + c->B->collisionComponent->GetBoundingBoxRadii().x)) return false;
		if (glm::abs(c->A->collisionComponent->GetBoundingBoxCentre().y - c->B->collisionComponent->GetBoundingBoxCentre().y) > (c->A->collisionComponent->GetBoundingBoxRadii().y + c->B->collisionComponent->GetBoundingBoxRadii().y)) return false;
		if (glm::abs(c->A->collisionComponent->GetBoundingBoxCentre().z - c->B->collisionComponent->GetBoundingBoxCentre().z) > (c->A->collisionComponent->GetBoundingBoxRadii().z + c->B->collisionComponent->GetBoundingBoxRadii().z)) return false;

		/*if (c->A->physicsComponent->destructible == true) {
		c->A->deleted = true;
		}

		if (c->B->physicsComponent->destructible == true) {
		c->B->deleted = true;
		}*/

		// If intersecting along all axes, boxes are colliding 
		return true;

	}
	else {
		return false;
	}
}

bool PhysicsEngine::isColliding(CollisionComponent* a, CollisionComponent* b) {

	// If statements differentiate which type of colliders want to collide, and does the appropriate collision check

	if (a->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE &&
		b->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {

		// Calculates squared distance between centres
		glm::vec3 d = a->GetBoundingSphereCentre() + b->GetBoundingSphereCentre();
		float d2 = glm::dot(d, d);

		// If the squared distance is less than the squared sum of radii, spheres intersect
		float radiusSum = a->GetBoundingSphereRadius() + b->GetBoundingSphereRadius();
		if (d2 <= radiusSum * radiusSum) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (a->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE &&
		b->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {

		// Find point p on AABB closest to the centre of the sphere
		glm::vec3 p;
		ClosestPointOnAABB(a->GetBoundingSphereCentre(), b->GetBoundingBoxCentre(), b->GetBoundingBoxRadii(), p);

		// If the squared distance from sphere centre to point p is less than the squared sphere radius, box and sphere intersect
		glm::vec3 v = p - a->GetBoundingSphereCentre();
		if (glm::dot(v, v) <= a->GetBoundingSphereRadius() * a->GetBoundingSphereRadius()) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (a->GetCollisionType() == CollisionComponent::Collision_Type::BOX &&
		b->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {

		// Find point p on AABB closest to the centre of the sphere
		glm::vec3 p;
		ClosestPointOnAABB(b->GetBoundingSphereCentre(), a->GetBoundingBoxCentre(), a->GetBoundingBoxRadii(), p);

		// If the squared distance from sphere centre to point p is less than the squared sphere radius, box and sphere intersect
		glm::vec3 v = p - b->GetBoundingSphereCentre();
		if (glm::dot(v, v) <= b->GetBoundingSphereRadius() * b->GetBoundingSphereRadius()) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (a->GetCollisionType() == CollisionComponent::Collision_Type::BOX &&
		b->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {

		// If no intersection along an axis, boxes are not colliding
		if (glm::abs(a->GetBoundingBoxCentre().x - b->GetBoundingBoxCentre().x) > (a->GetBoundingBoxRadii().x + b->GetBoundingBoxRadii().x)) return false;
		if (glm::abs(a->GetBoundingBoxCentre().y - b->GetBoundingBoxCentre().y) > (a->GetBoundingBoxRadii().y + b->GetBoundingBoxRadii().y)) return false;
		if (glm::abs(a->GetBoundingBoxCentre().z - b->GetBoundingBoxCentre().z) > (a->GetBoundingBoxRadii().z + b->GetBoundingBoxRadii().z)) return false;

		// If intersecting along all axes, boxes are colliding 
		return true;

	}
	else {
		return false;
	}
}


void PhysicsEngine::PenatrationCorrection(ContactData *c) {

	if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {

		// Creating normal to the vector between centre points of both objects
		glm::vec3 normal = c->B->physicsComponent->GetPosition() - c->A->physicsComponent->GetPosition();

		// Calculates the sum of each spheres radius
		float r = c->A->collisionComponent->GetBoundingSphereRadius() + c->B->collisionComponent->GetBoundingSphereRadius();

		// Calculates the distance (magnitude) of the normal vector
		float distance = glm::length(normal);

		// Checks if spheres are directly on top of each other
		// Pushes back sphere in a consistent manner
		if (distance == 0.0f) {
			c->penatration = c->A->collisionComponent->GetBoundingSphereRadius();
			c->normal = glm::vec3(0.0f, 1.0f, 0.0f);
		}
		// If spheres are not directly on top of each other
		// Penatration is equal to the sum of the radii - the distance between them
		// Normal is then normalized with the already calculated magnitude
		else {
			c->penatration = r - distance;
			c->normal = normal / distance;
		}
	}

	// ======== SPHERE VS BOX COLLISION ==========
	// ============ WORK IN PROGRESS ============= 
	/*
	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE &&
	c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {
	glm::vec3 n = c->B->physicsComponent->GetPosition() - c->A->physicsComponent->GetPosition();

	glm::vec3 closest = n;

	float xExtent = (c->B->collisionComponent->GetBoundingBox().max.x - c->B->collisionComponent->GetBoundingBox().min.x) / 2.0f;
	float yExtent = (c->B->collisionComponent->GetBoundingBox().max.y - c->B->collisionComponent->GetBoundingBox().min.y) / 2.0f;
	float zExtent = (c->B->collisionComponent->GetBoundingBox().max.z - c->B->collisionComponent->GetBoundingBox().min.z) / 2.0f;

	closest.x = glm::clamp(-xExtent, xExtent, closest.x);
	closest.y = glm::clamp(-yExtent, yExtent, closest.y);
	closest.z = glm::clamp(-zExtent, zExtent, closest.z);

	bool inside = false;

	if (n == closest) {
	inside = true;

	if (glm::abs(n.x) < glm::abs(n.y) && glm::abs(n.x) < glm::abs(n.z)) {
	if (closest.x > 0) {
	closest.x = -xExtent;
	}
	else {
	closest.x = xExtent;
	}
	}
	else if (glm::abs(n.y) < glm::abs(n.x) && glm::abs(n.y) < glm::abs(n.z)) {
	if (closest.y > 0) {
	closest.y = -yExtent;
	}
	else {
	closest.y = yExtent;
	}
	}
	else if (glm::abs(n.z) < glm::abs(n.x) && glm::abs(n.z) < glm::abs(n.y)) {
	if (closest.z > 0) {
	closest.z = -zExtent;
	}
	else {
	closest.z = zExtent;
	}
	}

	glm::vec3 normal = n - closest;
	float d = glm::length(normal);
	float r = c->A->collisionComponent->GetBoundingSphereRadius()adius;

	d = glm::sqrt(d);

	if (inside) {
	c->normal = -n;
	c->penatration = r - d;
	}
	else {
	c->normal = n;
	c->penatration = r - d;
	}
	}
	}
	// ======== SPHERE VS BOX COLLISION ==========
	// ============ WORK IN PROGRESS =============
	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX &&
	c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {
	glm::vec3 n = c->B->physicsComponent->GetPosition() - c->A->physicsComponent->GetPosition();

	glm::vec3 closest = n;

	float xExtent = (c->A->collisionComponent->GetBoundingBox().max.x - c->A->collisionComponent->GetBoundingBox().min.x) / 2.0f;
	float yExtent = (c->A->collisionComponent->GetBoundingBox().max.y - c->A->collisionComponent->GetBoundingBox().min.y) / 2.0f;
	float zExtent = (c->A->collisionComponent->GetBoundingBox().max.z - c->A->collisionComponent->GetBoundingBox().min.z) / 2.0f;

	closest.x = glm::clamp(-xExtent, xExtent, closest.x);
	closest.y = glm::clamp(-yExtent, yExtent, closest.y);
	closest.z = glm::clamp(-zExtent, zExtent, closest.z);

	bool inside = false;

	if (n == closest) {
	inside = true;

	if (glm::abs(n.x) < glm::abs(n.y) && glm::abs(n.x) < glm::abs(n.z)) {
	if (closest.x > 0) {
	closest.x = xExtent;
	}
	else {
	closest.x = -xExtent;
	}
	}
	else if (glm::abs(n.y) < glm::abs(n.x) && glm::abs(n.y) < glm::abs(n.z)) {
	if (closest.y > 0) {
	closest.y = yExtent;
	}
	else {
	closest.y = -yExtent;
	}
	}
	else if (glm::abs(n.z) < glm::abs(n.x) && glm::abs(n.z) < glm::abs(n.y)) {
	if (closest.z > 0) {
	closest.z = zExtent;
	}
	else {
	closest.z = -zExtent;
	}
	}

	glm::vec3 normal = n - closest;
	float d = glm::length(normal);
	float r = c->B->collisionComponent->GetBoundingSphereRadius();

	d = glm::sqrt(d);

	if (inside) {
	c->normal = -n;
	c->penatration = r - d;
	}
	else {
	c->normal = n;
	c->penatration = r - d;
	}
	}
	}*/

	else if (c->A->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX &&
		c->B->collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {
		// Setting the normal to a vector between centre points of both objects
		c->normal = c->B->physicsComponent->GetPosition() - c->A->physicsComponent->GetPosition();

		// The overlap in the x axis of the objects
		float x_overlap = c->A->collisionComponent->GetBoundingBoxRadii().x + c->B->collisionComponent->GetBoundingBoxRadii().x - glm::abs(c->normal.x);

		// If the overlap exists, continue
		if (x_overlap > 0) {

			// The overlap in the y axis of the objects
			float y_overlap = c->A->collisionComponent->GetBoundingBoxRadii().y + c->B->collisionComponent->GetBoundingBoxRadii().y - glm::abs(c->normal.y);

			// If the overlap exists, continue
			if (y_overlap > 0) {

				// The overlap in the z axis of the objects
				float z_overlap = c->A->collisionComponent->GetBoundingBoxRadii().z + c->B->collisionComponent->GetBoundingBoxRadii().z - glm::abs(c->normal.z);

				// If the overlap exists, continue
				if (z_overlap > 0) {

					// Checking if x axis is the smallest penatration
					if (x_overlap < y_overlap && x_overlap < z_overlap) {

						// Point normal towards B, since distance points from A->B
						if (c->normal.x < 0) {
							c->normal = glm::vec3(-1.0f, 0.0f, 0.0f);
						}

						// Point normal towards A, since distance points from A->B
						else {
							c->normal = glm::vec3(1.0f, 0.0f, 0.0f);
						}

						// Since x axis is the smallest, it will be our penatration distance
						c->penatration = x_overlap;

					}

					// Checking if y axis is the smallest penatration
					else if (y_overlap < x_overlap && y_overlap < z_overlap) {

						// Point normal towards B, since distance points from A->B
						if (c->normal.y < 0) {
							c->normal = glm::vec3(0.0f, -1.0f, 0.0f);
						}

						// Point normal towards A, since distance points from A->B
						else {
							c->normal = glm::vec3(0.0f, 1.0f, 0.0f);
						}

						// Since y axis is the smallest, it will be our penatration distance
						c->penatration = y_overlap;

					}

					// Checking if z axis is the smallest penatration
					else if (z_overlap < x_overlap && z_overlap < y_overlap) {

						// Point normal towards B, since distance points from A->B
						if (c->normal.z < 0) {
							c->normal = glm::vec3(0.0f, 0.0f, -1.0f);
						}

						// Point normal towards A, since distance points from A->B
						else {
							c->normal = glm::vec3(0.0f, 0.0f, 1.0f);
						}

						// Since z axis is the smallest, it will be our penatration distance
						c->penatration = z_overlap;

					}
				}
			}
		}
	}

	// Storing inverse masses
	float invMassA = c->A->physicsComponent->GetInverseMass();
	float invMassB = c->B->physicsComponent->GetInverseMass();

	// Calculates positional correction based on penatration of both objects and their inverse masses, along the normal
	const float slop = 0.02f; // Penatration allowed
	const float percent = 0.8f; // Penatration percentage to be corrected
	glm::vec3 correction = (glm::max(c->penatration - slop, 0.0f)) / (invMassA + invMassB) * c->normal * percent;

	// Pushes back objects based on correction vector and inverse mass
	//c->A->physicsComponent->SetPosition(c->A->physicsComponent->GetPosition() - invMassA * correction);
	//c->B->physicsComponent->SetPosition(c->B->physicsComponent->GetPosition() + invMassB * correction);
	c->A->SetWorldPosition(c->A->physicsComponent->GetPosition() - invMassA * correction);
	c->B->SetWorldPosition(c->B->physicsComponent->GetPosition() + invMassB * correction);
}

void PhysicsEngine::ApplyImpulse(ContactData *c) {

	// Initializing our relative velocity and impulses
	// All three are used when resolving collisions
	glm::vec3 rv;
	glm::vec3 impulse;
	glm::vec3 frictionImpulse;

	// Calculates relative velocity between both objects
	rv = c->B->physicsComponent->GetVelocity() - c->A->physicsComponent->GetVelocity();

	// Projects the relative velocity along the collision normal
	float velAlongNormal = glm::dot(rv, c->normal);

	// Does not resolve if velocities are seperating
	if (velAlongNormal > 0) {
		return;
	}

	// Calculates restitution based on the lowest value of restitution of our colliding objects
	float e = glm::min(c->A->physicsComponent->GetRestitution(), c->B->physicsComponent->GetRestitution());

	// Storing inverse masses
	float invMassA = c->A->physicsComponent->GetInverseMass();
	float invMassB = c->B->physicsComponent->GetInverseMass();

	// Calculates impulse scalar
	float j = -(1 + e) * velAlongNormal;
	j /= invMassA + invMassB;

	// If both objects haven't penatrated slightly,
	// the normal hasn't been calculated, and make the impulse zero
	impulse = j * c->normal;

	// FIX FOR CURRENT PHYISCS ENGINE BUILD
	// LIMITS IMPULSE TO PROVIDE LESS REALISTIC PHYSICS
	// MAKES FAST FLYING OBJECTS ON COLLISIONS IMPOSSIBLE
	/*
	if (impulse.x >= 150.0f || impulse.x <= -150.0f) {
	impulse.x = 0.0f;
	}

	if (impulse.y >= 150.0f || impulse.y <= -150.0f) {
	impulse.y = 0.0f;
	}

	if (impulse.z >= 150.0f || impulse.z <= -150.0f) {
	impulse.z = 0.0f;
	}
	*/

	// Changes the velocity of  objects based on  impulse and inverse mass
	c->A->physicsComponent->SetVelocity(c->A->physicsComponent->GetVelocity() - (invMassA * impulse));
	c->B->physicsComponent->SetVelocity(c->B->physicsComponent->GetVelocity() + (invMassB * impulse));

	// GAME SPECIFIC FIX THAT RESETS PLAYERS Y VELOCITY
	// DUE TO REDUCTION OF IMPULSE ON CONTACT WITH GROUND
	if (c->A->physicsComponent->position.y < 0.1f) {
		c->A->physicsComponent->velocity.y = 0.0f;
	}

	if (c->B->physicsComponent->position.y < 0.1f) {
		c->B->physicsComponent->velocity.y = 0.0f;
	}

	// Calculates the tangent vector
	glm::vec3 tangent = rv - (glm::dot(rv, c->normal) * c->normal);
	glm::normalize(tangent);

	// Calculates the tangent impulse scalar
	float jt = -glm::dot(rv, tangent);
	jt /= invMassA + invMassB;

	// Ignore tiny friction impulses
	if (jt == 0) {
		return;
	}

	// Calculate the relative coefficient of static friction
	float mu = glm::sqrt(glm::length(c->A->physicsComponent->GetStaticFriction()) + glm::length(c->B->physicsComponent->GetStaticFriction()));

	// Clamps the magnitude of friction based on static friction and creates the friction impulse vector
	if (glm::abs(jt) < j * mu) {
		frictionImpulse = jt * tangent;
	}
	else {
		// Calculate the relative coefficient of kinetic friction
		float mu2 = glm::sqrt(glm::length(c->A->physicsComponent->GetKineticFriction()) + glm::length(c->B->physicsComponent->GetKineticFriction()));
		frictionImpulse = -j * tangent * mu2;
	}


	// FIX FOR CURRENT PHYISCS ENGINE BUILD
	// LIMITS IMPULSE TO PROVIDE LESS REALISTIC PHYSICS
	// MAKES FAST FLYING OBJECTS ON COLLISIONS IMPOSSIBLE
	if (frictionImpulse.x >= 600.0f || frictionImpulse.x <= -600.0f) {
		frictionImpulse.x = 0.0f;
	}

	if (frictionImpulse.y >= 600.0f || frictionImpulse.y <= -600.0f) {
		frictionImpulse.y = 0.0f;
	}

	if (frictionImpulse.z >= 600.0f || frictionImpulse.z <= -600.0f) {
		frictionImpulse.z = 0.0f;
	}

	// Changes the velocity of  objects based on friction impulse and inverse mass
	c->A->physicsComponent->SetVelocity(c->A->physicsComponent->GetVelocity() - invMassA * frictionImpulse);
	c->B->physicsComponent->SetVelocity(c->B->physicsComponent->GetVelocity() + invMassB * frictionImpulse);
}

void PhysicsEngine::ClosestPointOnAABB(glm::vec3 p, glm::vec3 c, glm::vec3 r, glm::vec3 & q) {
	// If the point is outside the box, clamp it to the box
	// Else, keep it as is
	for (int i = 0; i < 3; i++) {
		float v = p[i];
		v = glm::max(v, c[i] - r[i]);
		v = glm::min(v, c[i] + r[i]);
		q[i] = v;
	}
}

float PhysicsEngine::SqDistancePointAABB(glm::vec3 p, Box b) {
	float sqDist = 0.0f;

	// Count any excess distance outside box extents for each axis
	for (int i = 0; i < 3; i++) {
		float v = p[i];
		if (v < b.c[i] - b.r[i]) sqDist += (b.c[i] - b.r[i] - v) * (b.c[i] - b.r[i] - v);
		if (v > b.c[i] + b.r[i]) sqDist += (v - b.c[i] + b.r[i]) * (v - b.c[i] + b.r[i]);
	}
	return sqDist;
}
