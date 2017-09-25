#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "Component.h"
#include <glm\glm.hpp>

namespace ENGINE {

	class LightComponent : public Component
	{
	public:
		enum Light_Type {
			NONE,
			DIRECTIONAL,
			POINTLIGHT,
			SPOTLIGHT
		};

		LightComponent();
		LightComponent(Light_Type lt);
		~LightComponent();

		void SetLightType(Light_Type lt);
		LightComponent::Light_Type GetLightType();

		glm::vec3 direction;

		float cutOff;
		float outerCutOff;

		float constant;
		float linear;
		float quadratic;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

	private:
		Light_Type lightType = NONE;

	};
}
#endif

