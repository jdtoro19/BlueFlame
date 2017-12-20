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
		void PreRender(Window* window, Camera* camera, bool splitscreen, bool postprocess);
		void Render(Camera* camera, std::vector<GameObject*> objectList, 
									std::vector<Light*> dirLightList, 
									std::vector<Light*> pointLightList, 
									std::vector<Light*> spotLightList
		);
		void PostRender(Window* window, bool splitscreen, bool postprocess);

		// Skybox
		void RenderSkybox(Skybox* sceneSkybox, Camera* camera);

		// Shader manager
		ResourceManager<Shader>* GetShaderManager();

		// Used for clean up
		void OnDestroy();

		void RenderQuad();

		void UseBloom();

		void SetUpFrameBuffers(Window* window);

		void EnableAA();
		void EnableShadows();
		void EnableBloom(bool set) { useBloom = set; };

		void EnableBlackAndWhite(bool set) { blackAndWhite = set; };
		void EnableInvertedColours(bool set);
		void EnableBlur(bool set);

	private:
		// Projection
		glm::mat4 projection;

		// Shader manager
		ResourceManager<Shader>* shaderManager;

		Shader* blur;
		Shader* bloom;
		Shader* screen;

		bool useBloom = false;

		bool blackAndWhite = false;
		bool invertedColors = false;
		bool screenBlur = false;

		unsigned int hdrFBO, colorBuffers[2], rboDepth, pingpongFBO[2], pingpongColorbuffers[2];

		unsigned int quadVAO = 0;
		unsigned int quadVBO;

		unsigned int attachments[2];

		unsigned int framebuffer, textureColorbuffer, rbo;

		void RenderBlackAndWhite();
	};
}

#endif