#pragma once
#ifndef MENUSELECTSCENE_H
#define MENUSELECTSCENE_H

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
#include <BlueFlameEngine\Engine\Rendering\2D\SliderUI.h>
#include "DemoScene.h"
#include "TestScene.h"
#include "GameTestScene.h"

using namespace ENGINE;

namespace GAME {

	class MenuSelectScene : public Scene
	{
	public:
		MenuSelectScene();
		~MenuSelectScene();

		// Every scene requires these three methods to be implemented
		bool Initialize();
		void Update(const float deltaTime);
		void HandleEvents(SDL_Event events);
		void HandleStates(const Uint8 *state);

		SceneManager* sceneManager;
	private:
		// shaders
		Shader* skyboxShader;

		// shader handles / reference to shader in the renderers shader manager
		ResourceHandle<Shader> skyboxShaderHandle;

		// UI
		TextUI* titleText;

		ButtonUI* buttonTest;
		ButtonUI* buttonLighting;
		ButtonUI* buttonDemo;
		ButtonUI* buttonExit;

		SliderUI* sliderTEST;
	};
}
#endif

