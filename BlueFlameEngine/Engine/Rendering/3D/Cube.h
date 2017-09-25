#ifndef CUBE_H
#define CUBE_H

#include "../../Core/GameObject.h"
#include "../../Core/RenderComponent.h"
#include "../../Graphics/Shader.h"

namespace ENGINE {

	class Cube : public GameObject {
	public:
		Cube();
		~Cube();

		void Update();
		void Render(Shader* shader);

		RenderComponent* renderComponent;
	};

}
#endif
