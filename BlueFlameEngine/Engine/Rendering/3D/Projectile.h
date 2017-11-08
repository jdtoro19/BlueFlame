#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../../Core/GameObject.h"
#include "../../Core/RenderComponent.h"
#include "../../Math/CollisionComponent.h"
#include "../../Math/PhysicsComponent.h"
#include "../../Math/PhysicsEngine.h"
#include "../../Graphics/Shader.h"

namespace ENGINE {

	class Projectile : public GameObject {
	public:
		Projectile(glm::vec3 p, int dir);
		~Projectile();

		void Jump(glm::vec3 vel);
		void AddVelocity(glm::vec3 vel);

		void Update(const float deltaTime);
		void Render(Shader* shader);

		int rip = NULL;

		RenderComponent* renderComponent;
	};

}
#endif
