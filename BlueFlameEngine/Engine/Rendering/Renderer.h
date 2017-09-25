#ifndef RENDERER_H
#define RENDERER_H

#include "../Graphics/Shader.h" 
#include "../Core/Window.h" 
#include "../Core/Light.h" 
#include "../Core/GameObject.h"
#include "../Camera/Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace ENGINE {

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void Render(Window* window, Camera* camera, glm::mat4 projection);
		void AddObject(std::vector<GameObject*> list);
		void ClearObjects();
		void AddLightObject(std::vector<Light*> list);

	private:
		Shader* shader;
		Shader* lightShader;
		Shader* modelShader;
		std::vector<GameObject*> objectList;
		std::vector<Light*> lightObjectList;
	};
}

#endif