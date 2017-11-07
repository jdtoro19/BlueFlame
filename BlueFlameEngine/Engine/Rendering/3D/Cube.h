#ifndef CUBE_H
#define CUBE_H

#include "../../Core/GameObject.h"
#include "../../Core/RenderComponent.h"
#include "../../Math/CollisionComponent.h"
#include "../../Math/PhysicsComponent.h"
#include "../../Graphics/Shader.h"

namespace ENGINE {

	class Cube : public GameObject {
	public:
		Cube();
		~Cube();

		void Jump(glm::vec3 vel);
		void AddVelocity(glm::vec3 vel);

		void Update(const float deltaTime);
		void Render(Shader* shader);

		RenderComponent* renderComponent;
		//CollisionComponent* collisionComponent;
		//PhysicsComponent* physicsComponent;
	};

}
#endif
