#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "..\Core\Component.h"
#include "Box.h"

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
		void SetVelocity(glm::vec3 vel);

		void SetMass(float _mass);

		void Stop(glm::vec3 otherPos);
		void Start();

		glm::vec3 getPosition();
		glm::vec3 GetVelocity();
		float GetMass();

		void Update(float deltaTime);
		void Render();
		glm::vec3 position;

	private:
		glm::vec3 velocity;
		glm::vec3 acceleration;
		float mass;
		glm::vec3 dimensions;
		Physics_Type physicsType;

		bool isStopped;
	};
}

#endif