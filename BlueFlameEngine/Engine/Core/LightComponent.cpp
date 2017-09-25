#include "LightComponent.h"

using namespace ENGINE;

LightComponent::LightComponent()
{
	SetLightType(NONE);
}

LightComponent::LightComponent(Light_Type lt)
{
	SetLightType(lt);
}

LightComponent::~LightComponent()
{
}

void LightComponent::SetLightType(Light_Type lt) {
	if (lt == NONE) {
		lightType = NONE;
		direction = glm::vec3(0.0f, 0.0f, 0.0f);
		cutOff = 0.0f;
		outerCutOff = 0.0f;
		constant = 0.0f;
		linear = 0.0f;
		quadratic = 0.0f;
		ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
		specular = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	else if (lt == DIRECTIONAL) {
		lightType = DIRECTIONAL;
		direction = glm::vec3(-0.2f, -0.3f, -0.1f);
		ambient = glm::vec3(0.6f, 0.6f, 0.6f);
		diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
		specular = glm::vec3(0.6f, 0.6f, 0.6f);
		// NOT USED
		cutOff = 0.0f;
		outerCutOff = 0.0f;
		constant = 0.0f;
		linear = 0.0f;
		quadratic = 0.0f;
	}
	else if (lt == POINTLIGHT) {
		lightType = POINTLIGHT;
		constant = 1.0f;
		linear = 0.09;
		quadratic = 0.032;
		ambient = glm::vec3(0.05f, 0.05f, 0.05f);
		diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
		specular = glm::vec3(1.0f, 1.0f, 1.0f);
		// NOT USED
		direction = glm::vec3(0.0f, 0.0f, 0.0f);
		cutOff = 0.0f;
		outerCutOff = 0.0f;
	}
}

LightComponent::Light_Type LightComponent::GetLightType()
{
	return lightType;
}
