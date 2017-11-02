#ifndef LIGHT_H
#define LIGHT_H

#include "GameObject.h"
#include "LightComponent.h"
#include "../Core/RenderComponent.h"

namespace ENGINE {

	class Light : public GameObject
	{
	public:
		Light();
		Light(LightComponent::Light_Type lt);
		~Light();

		void Render(Shader* shader);

		LightComponent* lightComponent;
		RenderComponent* renderComponent;
	};
}
#endif
