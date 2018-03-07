#pragma once
#ifndef TVTGAMESCENE_H
#define TVTGAMESCENE_H

#include <BlueFlameEngine\Engine\BFEngine.h>
#include <BlueFlameEngine\Engine\Core\Scene.h>
#include <BlueFlameEngine\Engine\Core\Light.h>
#include <BlueFlameEngine\Engine\Core\ResourceManager.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Cube.h> 
#include <BlueFlameEngine\Engine\Rendering\3D\Projectile.h> 
#include <BlueFlameEngine\Engine\Rendering\3D\Skybox.h> 
#include <BlueFlameEngine\Engine\Rendering\3D\Model.h>
#include <BlueFlameEngine\Engine\Graphics\Shader.h> 
#include <BlueFlameEngine\Engine\Math\PhysicsEngine.h>
#include <BlueFlameEngine\Engine\InputHandling\InputHandler.h>
#include <BlueFlameEngine\Engine\Rendering\2D\TextUI.h>
#include "../Game/ProjectileManager.h"
#include "../Player/Player.h"
#include "../Player/WindPlayer.h"

using namespace ENGINE;

namespace GAME {

	class TvTGameScene : public Scene
	{
	public:
		TvTGameScene();
		~TvTGameScene();

		// Every scene requires these five methods to be implemented
		bool Initialize();
		void Update(const float deltaTime);
		void FixedUpdate(const float deltaTime);
		void HandleEvents(SDL_Event events);
		void HandleStates(const Uint8 *state);

	private:
		// Lights
		Light* pointLight;
		Light* pointLight2;
		Light* pointLight3;
		Light* pointLight4;
		Light* blueLight;
		Light* redLight;
		Light* dirLight;

		// Objects
		Player* player1;  //P1
		Player* player2;  //P2
		Player* player3;  //P3
		Player* player4;  //P4

		Model* platform;
		Model* topRing;
		Model* middleRing;
		Model* bottomRing;

		// shaders
		Shader* defaultShader;
		Shader* skyboxShader;
		Shader* lightShader;

		// shader handles / reference to shader in the renderers shader manager
		ResourceHandle<Shader> defaultShaderHandle;
		ResourceHandle<Shader> skyboxShaderHandle;
		ResourceHandle<Shader> lightShaderHandle;

		// scene manager
		SceneManager* sceneManager;

		// Projectile Manager
		ProjectileManager* projectileManager;

		// other variables
		float moveSpeed = 2;
		float deltaTime;

		vector<Player*> playerList;

		bool fire = true;
		float timer = 0.3f;
	};
}
#endif

