#ifndef SCENE_H
#define SCENE_H

#include <SDL/SDL.h>
#include <iostream>
#include <vector>
#include "GameObject.h"
#include "Light.h"
#include "../Camera/Camera.h"
#include "../Math/PhysicsEngine.h"

namespace ENGINE {

	class Scene
	{
	public:
		Scene() {};
		~Scene() {};
		virtual void Initialize() = 0;
		virtual void Update(const float deltaTime) = 0;
		virtual void Render() = 0;
		virtual void Draw() = 0;
		virtual void HandleEvents(SDL_Event events) = 0;
		virtual void AddObject(GameObject* c) { objectList.push_back(c); };
		virtual void AddPhysicsObject(GameObject* c) { physicsObjectList.push_back(c); objectList.push_back(c); };
		virtual void RemoveObject(GameObject* c) {};
		virtual void AddLightObject(Light* c) { 
			if (c->lightComponent->GetLightType() == LightComponent::Light_Type::DIRECTIONAL) {
				dirLightList.push_back(c); objectList.push_back(c);
			}
			else if (c->lightComponent->GetLightType() == LightComponent::Light_Type::POINTLIGHT) {
				pointLightList.push_back(c); objectList.push_back(c);
			}
			else if (c->lightComponent->GetLightType() == LightComponent::Light_Type::SPOTLIGHT) {
				spotLightList.push_back(c); objectList.push_back(c);
			}
		};
		virtual std::vector<GameObject*> GetObjectList() { return objectList; };
		virtual std::vector<GameObject*> GetPhysicsObjectList() { return physicsObjectList; };
		virtual std::vector<Light*> GetDirLightList() { return dirLightList; };
		virtual std::vector<Light*> GetPointLightList() { return pointLightList; };
		virtual std::vector<Light*> GetSpotLightList() { return spotLightList; };
		virtual std::vector<Camera*> GetCameraList() { return cameraList; };

	protected:
		std::vector<GameObject*> objectList;
		std::vector<GameObject*> physicsObjectList;
		std::vector<Light*> dirLightList;
		std::vector<Light*> pointLightList;
		std::vector<Light*> spotLightList;
		std::vector<Camera*> cameraList = {
			new Camera(glm::vec3()),
			new Camera(glm::vec3())
		};
	};
}
#endif