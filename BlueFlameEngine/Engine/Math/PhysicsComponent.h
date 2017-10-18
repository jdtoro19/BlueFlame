#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "..\Core\Component.h"
#include "..\Rendering\3D\ModelComponent.h"
#include "..\Core\RenderComponent.h"
#include "Box.h"
#include <glm\glm.hpp>

namespace ENGINE {

	class PhysicsComponent : public Component {
	public:
		enum Collider_Type {
			BOX,
			SPHERE
		};

		enum Physics_Type {
			DYNAMIC,
			STATIC
		};

		PhysicsComponent();
		~PhysicsComponent();

		void setColliderType(Collider_Type ct, RenderComponent &render);
		void setColliderType(Collider_Type ct, ModelComponent &model);
		void setPhysicsType(Physics_Type pt);
		void Update();
		void Render();

	private:
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		Collider_Type colliderType;
		Physics_Type physicsType;


	};
}

#endif