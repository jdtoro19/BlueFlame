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

using namespace ENGINE;

namespace GAME {

	class DemoScene : public Scene
	{
	public:
		DemoScene();
		~DemoScene();

		// Every scene requires these five methods to be implemented
		void Initialize();
		void Update(const float deltaTime);
		void Render();
		void Draw();
		void HandleEvents(SDL_Event events);

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
			
		// skybox
		Skybox* skybox;	

		// shaders
		Shader* defaultShader;
		Shader* skyboxShader;

		// shader handles / reference to shader in the renderers shader manager
		ResourceHandle<Shader> defaultShaderHandle;
		ResourceHandle<Shader> skyboxShaderHandle;

		// scene manager
		SceneManager* sceneManager;

		// other variables
		float moveSpeed = 2;
		float deltaTime;

		bool fire = true;
		float timer = 0.3f;
	};
}
#endif

