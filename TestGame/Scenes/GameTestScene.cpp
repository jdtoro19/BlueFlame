#include "GameTestScene.h"

using namespace GAME;

GameTestScene::GameTestScene()
{
}

GameTestScene::~GameTestScene()
{
	bgm->Stop();
}

bool GameTestScene::Initialize()
{
	// Make reference to the scene manager
	sceneManager = BFEngine::GetInstance()->GetSceneManager();

	// Scene Options
	sceneManager->CaptureMouse(true);
	sceneManager->EnableFullscreen(false);

	// Set the position of the first camera
	cameraList[0]->Position = glm::vec3(0.0f, 1.0f, 3.0f);
	// Set the position for the second camera
	cameraList[1]->Position = glm::vec3(0.0f, 2.0f, 4.0f);

	// Load shaders
	defaultShader = new Shader("Shaders/model.vs", "Shaders/model.fs");
	skyboxShader = new Shader("Shaders/skybox.vs", "Shaders/skybox.fs");
	lightShader = new Shader("Shaders/lamp.vs", "Shaders/lamp.fs");

	// Put shaders into shader manager
	defaultShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("default"), defaultShader);
	skyboxShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("skybox"), skyboxShader);
	lightShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("light"), lightShader);

	// Particle
	particle = new ParticleSystem(sceneManager->GetRenderer()->GetShaderManager());

	// Lights
	pointLight = new Light();
	pointLight->SetShader(lightShaderHandle);
	pointLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	pointLight->renderComponent->SetColour(1.0f, 1.0f, 1.0f);
	pointLight->SetWorldPosition(-3.0f, 0.0f, 0.0f);
	pointLight->SetWorldScale(0.5f);
	pointLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	pointLight->lightComponent->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));

	// Make directional light
	dirLight = new Light(LightComponent::Light_Type::DIRECTIONAL);
	dirLight->lightComponent->SetDirection(glm::vec3(-0.5f, 0.0f, -0.8f));
	dirLight->lightComponent->ambient = glm::vec3(0.2f, 0.2f, 0.2f);

	// Player
	player = new WindPlayer();
	player->SetShader(defaultShaderHandle);
	player->EnableTarget();

	// Floor
	floor = new Cube();
	floor->SetShader(defaultShaderHandle);
	floor->renderComponent->SetColour(0.1f, 0.1f, 0.1f);
	floor->physicsComponent->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
	floor->SetWorldScale(11.0f, 1.0f, 11.0f);
	floor->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	floor->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	floor->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	floor->physicsComponent->SetMass(0.0f);

	// Make skybox, load its textures, set properties, and give to the renderer
	skybox = new Skybox();
	std::vector<char*> faces;
	faces.push_back("Resources/Textures/Skyboxes/grimmnight/right.png");
	faces.push_back("Resources/Textures/Skyboxes/grimmnight/left.png");
	faces.push_back("Resources/Textures/Skyboxes/grimmnight/top.png");
	faces.push_back("Resources/Textures/Skyboxes/grimmnight/bottom.png");
	faces.push_back("Resources/Textures/Skyboxes/grimmnight/back.png");
	faces.push_back("Resources/Textures/Skyboxes/grimmnight/front.png");
	skybox->LoadTextures(faces);
	skybox->SetShader(skyboxShaderHandle);

	// add objects and lights to their lists
	AddObject(player);
	AddObject(floor);
	AddLightObject(pointLight);
	AddLightObject(dirLight);
	AddObject(particle);

	bgm = new Music();
	if (!bgm->Load("Resources/Audio/bgm.mp3")) {
		std::cout << "BGM could not load" << std::endl;
	}
	bgm->Play(-1);

	sEffect = new SoundEffect();
	if (!sEffect->Load("Resources/Audio/IceManTest.wav")) {
		std::cout << "Sound effect could not load" << std::endl;
	}
	sEffect->SetVolume(50);

	return true;
}

void GameTestScene::Update(const float deltaTime)
{
	sceneManager->DebugText("Z to reload scene");
	sceneManager->DebugText("X to load gameplay test scene");
	sceneManager->DebugText("C to load lighting test scene");
	sceneManager->DebugText("7 to release mouse");
	sceneManager->DebugText("8 to capture mouse");
	sceneManager->DebugText(player->GetWorldPosition());
	sceneManager->DebugText("STUN: " + std::to_string(player->GetStunTimer()));
	sceneManager->DebugText("Press space for sound effect");
	sceneManager->DebugText(player->GetPlayerState());

	PhysicsEngine::GetInstance()->AddObjectList(objectList);
	PhysicsEngine::GetInstance()->Update(deltaTime);

	player->SetTarget(pointLight->GetWorldPosition());

	// Timer for firing (Keyboard controls only)
	timer -= deltaTime;
	if (timer <= 0) {
		timer = 0;
		fire = true;
	}

	//check for joystick so we're not wasting time
	if (InputHandler::GetInstance()->areJoysticksLive()) {
		//joystick motion update
		//maybe we don't handle it as handleevent

		//handle it as a for loop

		for (int i = 0; i < InputHandler::GetInstance()->jCheck(); i++) {

			Sint32 x_move = SDL_JoystickGetAxis(InputHandler::GetInstance()->joystick[i], 0);
			Sint32 y_move = SDL_JoystickGetAxis(InputHandler::GetInstance()->joystick[i], 1);

			//tareing the joysticks

			//if (x_move + InputHandler::GetInstance()->TareX[0])

			x_move += InputHandler::GetInstance()->TareX[i];
			y_move += InputHandler::GetInstance()->TareY[i];

			float modifierX = 0;
			float modifierY = 0;

			Sint16 jStickMod = 10000;

			if (x_move < 320 && x_move > -320) {
				modifierX = 0;
			}
			else if (x_move >= 320) {

				modifierX = x_move / jStickMod; //right
				cameraList[0]->ProcessKeyboard(RIGHT, 0.01f * modifierX);
			}
			else if (x_move <= -320) {
				modifierX = x_move / jStickMod; //left
				cameraList[0]->ProcessKeyboard(LEFT, 0.01f * -modifierX);
			}

			if (y_move < 320 && y_move > -320) {
				modifierY = 0;
			}

			else if (y_move >= 320) {
				modifierY = y_move / jStickMod; //down
				cameraList[0]->ProcessKeyboard(BACKWARD, 0.01f * modifierY);
			}
			else if (y_move <= -320) {
				modifierY = y_move / jStickMod; //up
				cameraList[0]->ProcessKeyboard(FORWARD, 0.01f * -modifierY);
			}
			//cout << x_move << " " << x_move / jStickMod << " " << y_move << " " << y_move / jStickMod << endl;
			//std::cout << "ModX: " << modifierX << " ModY: " << modifierY << std::endl;
		}

	}
	else {
		//std::cout << "No joystick events today." << std::endl;
	}
}

void GameTestScene::HandleEvents(SDL_Event events)
{
	//joystick
	switch (events.type)
	{
	case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */
		if (events.jaxis.axis == 2) //left trigger
		{
			/* Up-Down movement code goes here */
			std::cout << "Left Trigger" << std::endl;
		}
		if (events.jaxis.axis == 5) //right trigger
		{
			/* Up-Down movement code goes here */
			std::cout << "Right Trigger" << std::endl;
		}


		break;

	case SDL_JOYBUTTONDOWN:  /* Handle Joystick Button Presses */
		if (events.jbutton.button == 0) //A button
		{
			/* code goes here */
			std::cout << "A button was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 1) //B button
		{
			/* code goes here */
			std::cout << "B button was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 2) //X button
		{
			/* code goes here */
			std::cout << "X button was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 3) //Y button
		{
			/* code goes here */
			std::cout << "Y button was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 4) //left bumper
		{
			/* code goes here */
			std::cout << "Left bumper was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 5) //right bumper
		{
			/* code goes here */
			std::cout << "right bumper was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 6) //back button
		{
			/* code goes here */
			std::cout << "back button was probably pressed." << std::endl;
			BFEngine::GetInstance()->TerminateGame();
		}
		if (events.jbutton.button == 7) //start button
		{
			/* code goes here */
			std::cout << "start was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 8) //left joystick
		{
			/* code goes here */
			std::cout << "left joystick was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 9) //right joystick
		{
			/* code goes here */
			std::cout << "right joystick was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 10) //xbox key
		{
			/* code goes here */
			std::cout << "xbox key was probably pressed." << std::endl;
			//xbox key closes the game
			BFEngine::GetInstance()->TerminateGame();
		}
		if (events.jbutton.button == 11) //left trigger
		{
			/* code goes here */
			std::cout << "left trigger was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 12) //right trigger
		{
			/* code goes here */
			std::cout << "right trigger was probably pressed." << std::endl;
		}
		break;

	case SDL_JOYBALLMOTION:  /* Handle Joyball Motion */
		if (events.jball.ball == 0)
		{
			/* ball handling */
		}
		break;
	}

	// PLAYER
	// Movement
	if (events.type == SDL_KEYDOWN && events.key.repeat == 0)
	{
		//Select surfaces based on key press
		switch (events.key.keysym.sym)
		{
		case SDLK_LEFT:
			player->LightAttack();
			if (fire) {
				fire = false;
				timer = 0.3f;
				sEffect->Play();
			}
			break;
		}
	}

	// Camera look
	if (events.type == SDL_MOUSEMOTION) {
		cameraList[0]->ProcessMouseMovement((float)events.motion.x, (float)events.motion.y);
	}

	if (events.type == SDL_MOUSEWHEEL) {
		cameraList[0]->ProcessMouseScroll((float)events.wheel.y);
	}

	player->HandleEvents(events);
}

void GameTestScene::HandleStates(const Uint8 *state)
{
	player->HandleStates(state);

	if (state[SDL_SCANCODE_T]) {
		player->Movement(Player::PLAYERMOVEMENT::FORWARD, deltaTime);
	}
	if (state[SDL_SCANCODE_G]) {
		player->Movement(Player::PLAYERMOVEMENT::BACKWARD, deltaTime);
	}
	if (state[SDL_SCANCODE_H]) {
		player->Movement(Player::PLAYERMOVEMENT::RIGHT, deltaTime);
	}
	if (state[SDL_SCANCODE_F]) {
		player->Movement(Player::PLAYERMOVEMENT::LEFT, deltaTime);
	}

	// Light
	// Movement
	if (state[SDL_SCANCODE_I]) {
		pointLight->SetWorldPosition(pointLight->GetWorldPosition() + glm::vec3(0.0f, moveSpeed * deltaTime, 0.0f));
	}
	if (state[SDL_SCANCODE_K]) {
		pointLight->SetWorldPosition(pointLight->GetWorldPosition() + glm::vec3(0.0f, -moveSpeed * deltaTime, 0.0f));
	}
	if (state[SDL_SCANCODE_L]) {
		pointLight->SetWorldPosition(pointLight->GetWorldPosition() + glm::vec3(moveSpeed * deltaTime, 0.0f, 0.0f));
	}
	if (state[SDL_SCANCODE_J]) {
		pointLight->SetWorldPosition(pointLight->GetWorldPosition() + glm::vec3(-moveSpeed * deltaTime, 0.0f, 0.0f));
	}

	// VIEW/CAMERA
	if (state[SDL_SCANCODE_W]) {
		cameraList[0]->ProcessKeyboard(FORWARD, deltaTime * moveSpeed);
	}
	if (state[SDL_SCANCODE_S]) {
		cameraList[0]->ProcessKeyboard(BACKWARD, deltaTime * moveSpeed);
	}
	if (state[SDL_SCANCODE_D]) {
		cameraList[0]->ProcessKeyboard(RIGHT, deltaTime * moveSpeed);
	}
	if (state[SDL_SCANCODE_A]) {
		cameraList[0]->ProcessKeyboard(LEFT, deltaTime * moveSpeed);
	}

	// Reload Scene
	if (state[SDL_SCANCODE_Z]) {
		bgm->Stop();
		sceneManager->EnableSplitscreen(false);
		sceneManager->SwitchScene(new GameTestScene());
	}

	// Reload Scene
	if (state[SDL_SCANCODE_X]) {
		bgm->Stop();
		sceneManager->EnableSplitscreen(false);
		sceneManager->SwitchScene(new DemoScene());
	}

	// Reload Scene
	if (state[SDL_SCANCODE_C]) {
		bgm->Stop();
		sceneManager->EnableSplitscreen(false);
		sceneManager->SwitchScene(new TestScene());
	}

	if (state[SDL_SCANCODE_7]) {
		sceneManager->CaptureMouse(false);
	}

	if (state[SDL_SCANCODE_8]) {
		sceneManager->CaptureMouse(true);
	}
}