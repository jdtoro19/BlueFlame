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
		 bool isColliding(ContactData *c);

		 // Function corrects the penatration into other objects when colliding by changing its position
		 void PenatrationCorrection(ContactData *c);
		 
		 // Function applies impulse to both objects when colliding by changing its velocities
		 void ApplyImpulse(ContactData *c);

	public:
		static std::unique_ptr<PhysicsEngine> PhysicsEngineInstance;
		friend std::default_delete<PhysicsEngine>;
		std::vector<ContactData*> contactList;

	};
}

#endif

