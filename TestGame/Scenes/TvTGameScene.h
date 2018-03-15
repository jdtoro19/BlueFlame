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
#include <BlueFlameEngine\Engine\Rendering\2D\ImageUI.h>
#include <BlueFlameEngine\Engine\Timers\Cooldown.h>
#include "../Game/ProjectileManager.h"
#include "../Game/GameManager.h"
#include "../Player/Player.h"
#include "../Player/WindPlayer.h"
#include "MenuSelectScene.h"

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

		// scene manager
		SceneManager* sceneManager;

	private:
		// shaders
		Shader* defaultShader;
		Shader* skyboxShader;
		Shader* lightShader;

		// shader handles / reference to shader in the renderers shader manager
		ResourceHandle<Shader> defaultShaderHandle;
		ResourceHandle<Shader> skyboxShaderHandle;
		ResourceHandle<Shader> lightShaderHandle;

		// Camera functions
		void CameraMove(ENGINE::Camera_Movement direction, float yaw, float pitch, float speed, int camera);

		// Intro 
		void PlayIntro();
		void SkipIntro();
		void PlayRoundStart();

		// UI
		ImageUI* fadeImage;
		TextUI* roundText;
		TextUI* roundTextFade;

		// Players
		Player* player1;
		Player* player2;
		Player* player3;
		Player* player4;

		// Map
		Model* platform;
		Model* topRing;
		Model* middleRing;
		Model* bottomRing;
		Cube* floor;

		// Lights
		Light* pointLight;
		Light* pointLight2;
		Light* pointLight3;
		Light* pointLight4;
		Light* blueLight;
		Light* redLight;
		Light* dirLight;

		// Set up mathods
		void SetUpPlayers();
		void SetUpArena();

		// Particles
		ParticleSystem* particle1;
		ParticleSystem* particle2;
		ParticleSystem* particle3;
		ParticleSystem* particle4;

		// Projectile Manager
		ProjectileManager* projectileManager;

		// Game Manager
		GameManager* gameManager;

		Cooldown roundTimer;
		TextUI* roundTimerText;

		// Player List
		vector<Player*> playerList;

		// Intro Variables
		Cooldown cameraCD;
		Cooldown roundCD;
		float fadeAlpha = 1.0f;
		bool cameraSwitch1 = false;
		bool cameraSwitch2 = false;
		bool cameraSwitch3 = false;
		bool playingIntro = true;
		bool ready = false;
		bool startText = false;
		bool roundStart = false;
		bool playAudio = true;

		// Audio
		Music* bgm;
	};
}
#endif
