#include "Light.h"

using namespace ENGINE;

Light::Light()
{
	lightComponent = new LightComponent();
	isModel = true;
}

Light::Light(LightComponent::Light_Type lt)
{
	lightComponent = new LightComponent(lt);
	isModel = true;
}

Light::~Light()
{
}
