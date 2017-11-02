#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "../Graphics/Shader.h" 
#include "../Core/Window.h" 
#include "../Core/Light.h" 
#include "../Core/GameObject.h"
#include "../Core/ResourceManager.h"
#include "../Camera/Camera.h"
#include "3D/Skybox.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace ENGINE {

	// Renders all game objects and the skybox
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		// Initialize
		void Initialize(Window* w);

		// Rendering Functions
		void PreRender(Window* window, Camera* camera, bool splitscreen);
		void Render(Camera* camera, std::vector<GameObject*> objectList, 
									std::vector<Light*> dirLightList, 
									std::vector<Light*> pointLightList, 
									std::vector<Light*> spotLightList
		);
		void PostRender(Window* window);		

		// Skybox
		void SetSkybox(Skybox* skybox);
		void RenderSkybox(Camera* camera);

		// Shader manager
		ResourceManager<Shader>* GetShaderManager();

		// Used for clean up
		void OnDestroy();

		void renderQuad();

		void SetUpFrameBuffers(Window* window);

	private:
		// Skybox
		Skybox* skybox;

		// Projection
		glm::mat4 projection;

		// Shader manager
		ResourceManager<Shader>* shaderManager;

		Shader* blur;
		Shader* bloom;

		unsigned int hdrFBO, colorBuffers[2], rboDepth, pingpongFBO[2], pingpongColorbuffers[2];

		unsigned int quadVAO = 0;
		unsigned int quadVBO;

		unsigned int attachments[2];
	};
}

#endif