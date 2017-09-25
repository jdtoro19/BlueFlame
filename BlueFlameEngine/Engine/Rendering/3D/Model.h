#ifndef MODEL_H
#define MODEL_H

#include "../../Core/GameObject.h"
#include "../../Graphics/Shader.h"
#include "ModelComponent.h"
#include <iostream>

namespace ENGINE {

	class Model : public GameObject
	{
	public:
		Model();
		Model(std::string path);
		~Model();

		void Update();
		void Render(Shader* shader);

		ModelComponent* model;
	};
}

#endif

