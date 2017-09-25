#ifndef LIGHT_H
#define LIGHT_H

#include "GameObject.h"
#include "LightComponent.h"

namespace ENGINE {

	class Light : public GameObject
	{
	public:
		Light();
		Light(LightComponent::Light_Type lt);
		~Light();

		LightComponent* lightComponent;
	};
}
#endif
