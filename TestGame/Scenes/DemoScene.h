#pragma once
#ifndef DEMOSCENE_H
#define DEMOSCENE_H

#include <BlueFlameEngine\Engine\BFEngine.h>
#include <BlueFlameEngine\Engine\Core\Scene.h>
#include <BlueFlameEngine\Engine\Core\Light.h>
#include <BlueFlameEngine\Engine\Core\ResourceManager.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Cube.h> 
#include <BlueFlameEngine\Engine\Rendering\3D\Projectile.h> 
#include <BlueFlameEngine\Engine\Rendering\3D\Skybox.h> 
#include <BlueFlameEngine\Engine\Graphics\Shader.h> 
#include <BlueFlameEngine\Engine\Math\PhysicsEngine.h>
#include <BlueFlameEngine\Engine\InputHandling\InputHandler.h>
#include <BlueFlameEngine\Engine\Rendering\2D\TextUI.h>

using namespace ENGINE;

namespace GAME {

	class DemoScene : public Scene
	{
	public:
		DemoScene();
		~DemoScene();

		// Every scene requires these three methods to be implemented
		bool Initialize();
		void Update(const float deltaTime);
		void HandleEvents(SDL_Event events);

		SceneManager* sceneManager;
	private:
		// Lights
		Light* pointLight;
		Light* dirLight;

		// Objects
		Cube* cube1;  //P1
		Cube* cube2;  //P2
		Cube* cube3;  //P3
		Cube* cube4;  //P4

		Cube* floor;
		Cube* wall;
		Cube* wall1;
		Cube* middleWall;	

		// UI
		TextUI* text;

		// shaders
		Shader* defaultShader;
		Shader* skyboxShader;

		// shader handles / reference to shader in the renderers shader manager
		ResourceHandle<Shader> defaultShaderHandle;
		ResourceHandle<Shader> skyboxShaderHandle;

		// other variables
		float moveSpeed = 2;

		bool fire = true;
		float timer = 0.3f;
	};
}
#endif

