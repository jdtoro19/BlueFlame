#ifndef MODEL_H
#define MODEL_H

#include "../../Core/GameObject.h"
#include "../../Graphics/Shader.h"
#include "ModelComponent.h"
#include "../../Math/CollisionComponent.h"
#include "../../Math/PhysicsComponent.h"
#include <iostream>

namespace ENGINE {

	class Model : public GameObject
	{
	public:
		Model();
		Model(std::string path);
		~Model();

		void Update(const float deltaTime);
		void Render(Shader* shader);

		ModelComponent* model;
		//CollisionComponent* collisionComponent;
		//PhysicsComponent* physicsComponent;
	};
}

#endif

