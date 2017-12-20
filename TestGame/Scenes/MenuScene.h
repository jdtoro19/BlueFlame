#pragma once
#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <BlueFlameEngine\Engine\BFEngine.h>
#include <BlueFlameEngine\Engine\Core\Scene.h>
#include <BlueFlameEngine\Engine\Core\Light.h>
#include <BlueFlameEngine\Engine\Core\ResourceManager.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Skybox.h> 
#include <BlueFlameEngine\Engine\Graphics\Shader.h> 
#include <BlueFlameEngine\Engine\InputHandling\InputHandler.h>
#include <BlueFlameEngine\Engine\Rendering\2D\TextUI.h>
#include <BlueFlameEngine\Engine\Rendering\2D\ImageUI.h>
#include <BlueFlameEngine\Engine\Rendering\2D\ButtonUI.h>
#include "DemoScene.h"
#include "TestScene.h"
#include "MenuSelectScene.h"

using namespace ENGINE;

namespace GAME {

	class MenuScene : public Scene
	{
	public:
		MenuScene();
		~MenuScene();

		// Every scene requires these five methods to be implemented
		void Initialize();
		void Update(const float deltaTime);
		void Render();
		void Draw();
		void HandleEvents(SDL_Event events);

	private:
		// shaders
		Shader* skyboxShader;

		// shader handles / reference to shader in the renderers shader manager
		ResourceHandle<Shader> skyboxShaderHandle;

		// UI
		TextUI* titleText;
		TextUI* startText;
		TextUI* creditText;

		SceneManager* sceneManager;

		float z = 0;
	};
}
#endif

