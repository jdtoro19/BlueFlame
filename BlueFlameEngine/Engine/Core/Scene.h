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
		virtual void Update(const float deltaTime) {};
		virtual void Render() {};
		virtual void HandleEvents(SDL_Event events) {};
		virtual void AddObject(GameObject* c) { objectList.push_back(c); };
		virtual void RemoveObject(GameObject* c) {};
		virtual void AddLightObject(Light* c) { lightObjectList.push_back(c); objectList.push_back(c); };
		virtual std::vector<GameObject*> GetObjectList() { return objectList; };
		virtual std::vector<Light*> GetLightObjectList() { return lightObjectList; };
	protected:
		std::vector<GameObject*> objectList;
		std::vector<Light*> lightObjectList;
	};
}
#endif