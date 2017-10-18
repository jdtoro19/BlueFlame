#ifndef SCENE_H
#define SCENE_H

#include <SDL/SDL.h>
#include <iostream>
#include <vector>
#include "GameObject.h"
#include "Light.h"

namespace ENGINE {

	class Scene
	{
	public:
		Scene() {};
		~Scene() {};
		virtual void Update(const float deltaTime) = 0;
		virtual void Render() = 0;
		virtual void HandleEvents(SDL_Event events) = 0;
		virtual void AddObject(GameObject* c) { objectList.push_back(c); };
		virtual void RemoveObject(GameObject* c) {};
		virtual void AddLightObject(Light* c) { lightObjectList.push_back(c); objectList.push_back(c); };
		virtual std::vector<GameObject*> GetObjectList() { return objectList; };
		virtual std::vector<Light*> GetLightObjectList() { return lightObjectList; };
		virtual glm::vec3 GetStartPos() { return startPos; };

	protected:
		std::vector<GameObject*> objectList;
		std::vector<Light*> lightObjectList;
		glm::vec3 startPos = glm::vec3();
	};
}
#endif