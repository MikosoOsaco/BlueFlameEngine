#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "ContactData.h"
#include <memory>
#include "../../Engine/Core/GameObject.h"

namespace ENGINE {

	class PhysicsEngine {
	private:
		// Constructor and deconstructor is private because the physics engine is a singleton
		PhysicsEngine();
		~PhysicsEngine();

	public:

		// Main Update function called every frame use functions on the object list as needed for the game
		void Update(float deltaTime);

		// Returns the current instance of the physics engine
		static PhysicsEngine* GetInstance();

		// Function adds all physics objects to the physics engine
		void AddObjectList(std::vector<GameObject*> physicsObjectList);

		// Function generates the pairs to be collided with each other based on the objectlist given
		void GeneratePairs(std::vector<GameObject*> physicsObjectList);

		// Function returns true if both collision components are colliding
		 static bool isColliding(ContactData *c);
		 static bool isColliding(CollisionComponent *a, CollisionComponent *b);

		 // Function corrects the penatration into other objects when colliding by changing its position
		 void PenatrationCorrection(ContactData *c);
		 
		 // Function applies impulse to both objects when colliding by changing its velocities
		 void ApplyImpulse(ContactData *c);

		 // Basic Primitive Tests to be used when needed
		 static void ClosestPointOnAABB(glm::vec3 p, glm::vec3 c, glm::vec3 r, glm::vec3 &q);
		 float SqDistancePointAABB(glm::vec3 p, Box b);

		 // Singleton pointers
		 static std::unique_ptr<PhysicsEngine> PhysicsEngineInstance;
		 friend std::default_delete<PhysicsEngine>;

		 // List of objects to collide
		 std::vector<ContactData*> contactList;

		

	};
}

#endif

