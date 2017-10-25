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
		void Update();
		void Render();

	private:
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		Physics_Type physicsType;


	};
}

#endif