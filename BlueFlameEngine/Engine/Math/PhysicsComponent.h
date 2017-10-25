#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "..\Core\Component.h"

#include <glm\glm.hpp>

namespace ENGINE {

	class PhysicsComponent : public Component {
	public:

		enum Physics_Type {
			DYNAMIC,
			STATIC
		};

		PhysicsComponent();
		~PhysicsComponent();

		void setPhysicsType(Physics_Type pt);
		PhysicsComponent::Physics_Type getPhysicsType();

		void setVelocity(glm::vec3 vel);
		void setPosition(glm::vec3 pos);
		void setAcceleration(glm::vec3 accel);

		glm::vec3 getPosition();

		void Update(float deltaTime, glm::vec3 pos);
		void Render();

	private:
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		Physics_Type physicsType;


	};
}

#endif