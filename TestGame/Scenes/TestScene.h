#pragma once
#ifndef TESTSCENE_H
#define TESTSCENE_H

#include <BlueFlameEngine\Engine\BFEngine.h>
#include <BlueFlameEngine\Engine\Core\Scene.h>
#include <BlueFlameEngine\Engine\Core\Light.h>
#include <BlueFlameEngine\Engine\Core\ResourceManager.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Cube.h> 
#include <BlueFlameEngine\Engine\Rendering\3D\Model.h> 
#include <BlueFlameEngine\Engine\Rendering\3D\Skybox.h> 
#include <BlueFlameEngine\Engine\Graphics\Shader.h> 
#include <BlueFlameEngine\Engine\Math\PhysicsEngine.h>
#include <BlueFlameEngine\Engine\InputHandling\InputHandler.h>
#include "DemoScene.h"

using namespace ENGINE;

namespace GAME {

	class TestScene : public Scene
	{
	public:
		TestScene();
		~TestScene();

		// Every scene requires these five methods to be implemented
		void Initialize();
		void Update(const float deltaTime);
		void Render();
		void Draw();
		void HandleEvents(SDL_Event events);

	private:
		// Lights
		Light* blueLight;
		Light* redLight;
		Light* greenLight;
		Light* yellowLight;
		Light* dirLight;

		// Objects
		Model* model;
		Model* model2;
		Model* model3;
		Model* model4;
		Cube* floor;

		// shaders
		Shader* defaultShader;
		Shader* modelShader;
		Shader* skyboxShader;
		Shader* lightShader;

		// shader handles / reference to shader in the renderers shader manager
		ResourceHandle<Shader> defaultShaderHandle;
		ResourceHandle<Shader> modelShaderHandle;
		ResourceHandle<Shader> skyboxShaderHandle;
		ResourceHandle<Shader> lightShaderHandle;

		// scene manager
		SceneManager* sceneManager;

		// other variables
		float moveSpeed = 2;
		float deltaTime;
	};
}
#endif

