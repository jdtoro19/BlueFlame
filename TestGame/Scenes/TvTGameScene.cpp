#include "TvTGameScene.h"

using namespace GAME;

TvTGameScene::TvTGameScene()
{
}

TvTGameScene::~TvTGameScene()
{
	bgm->Stop();
}

bool TvTGameScene::Initialize()
{
	// Make reference to the scene manager
	sceneManager = BFEngine::GetInstance()->GetSceneManager();

	// Set screen options
	sceneManager->EnableSplitscreen(false);
	sceneManager->ShowFPS(false);
	sceneManager->CaptureMouse(true);

	// Set the options for the first camera
	cameraList[0]->Position = glm::vec3(7.0f, 0.7f, 7.0f);

	// Load shaders
	defaultShader = new Shader("Shaders/model.vs", "Shaders/model.fs");
	skyboxShader = new Shader("Shaders/skybox.vs", "Shaders/skybox.fs");
	lightShader = new Shader("Shaders/lamp.vs", "Shaders/lamp.fs");

	// Put shaders into shader manager
	defaultShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("default"), defaultShader);
	skyboxShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("skybox"), skyboxShader);
	lightShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("lamp"), lightShader);

	// Music
	bgm = new Music();
	if (!bgm->Load("Resources/Audio/20XX Theme Battle.mp3")) {
		std::cout << "BGM could not load" << std::endl;
	}
	bgm->SetVolume(0);

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

	// UI
	fadeImage = new ImageUI();
	fadeImage->SetImage("Resources/Textures/blackFILL.png");
	fadeImage->SetPosition(sceneManager->GetScreenWidth() / 2, sceneManager->GetScreenHeight() / 2);
	fadeImage->SetScale(4.0f, 11.0f);

	// UI
	roundText = new TextUI();
	roundText->SetFont("Resources/Fonts/ka1.ttf");
	roundText->SetText("Ready?");
	roundText->SetColour(1.0f, 0.5f, 0.0f);
	roundText->SetSize(0.0f);
	roundText->SetSpacing(9.0f);
	roundText->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundText->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
	roundText->SetVisible(false);

	// UI
	roundTextFade = new TextUI();
	roundTextFade->SetFont("Resources/Fonts/ka1.ttf");
	roundTextFade->SetText("Round Start");
	roundTextFade->SetColour(1.0f, 0.5f, 0.0f);
	roundTextFade->SetSize(1.0f);
	roundTextFade->SetSpacing(9.0f);
	roundTextFade->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundText->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
	roundTextFade->SetVisible(false);

	// Make Projectile Manager
	projectileManager = new ProjectileManager();

	// Make Players
	SetUpPlayers();

	// Make Arena
	SetUpArena();

	// Lights
	//
	// Make point light
	//
	pointLight = new Light();
	pointLight->SetShader(defaultShaderHandle);
	pointLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	pointLight->renderComponent->SetColour(1.0f, 1.0f, 1.0f);
	pointLight->renderComponent->CanRender(false);
	pointLight->canRender = false;
	pointLight->SetWorldPosition(-5.25f, -0.5f, 5.5f);
	pointLight->SetWorldScale(0.5f);
	pointLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	pointLight->lightComponent->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));
	//
	
	//
	pointLight2 = new Light();
	pointLight2->SetShader(lightShaderHandle);
	pointLight2->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	pointLight2->renderComponent->SetColour(1.0f, 1.0f, 1.0f);
	pointLight2->renderComponent->CanRender(false);
	pointLight2->canRender = false;
	pointLight2->SetWorldPosition(5.25f, -0.5f, 5.5f);
	pointLight2->SetWorldScale(0.5f);
	pointLight2->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	pointLight2->lightComponent->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));
	//

	//
	pointLight3 = new Light();
	pointLight3->SetShader(lightShaderHandle);
	pointLight3->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	pointLight3->renderComponent->SetColour(1.0f, 1.0f, 1.0f);
	pointLight3->renderComponent->CanRender(false);
	pointLight3->canRender = false;
	pointLight3->SetWorldPosition(5.25f, -0.5f, -5.5f);
	pointLight3->SetWorldScale(0.5f);
	pointLight3->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	pointLight3->lightComponent->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));
	//

	//
	pointLight4 = new Light();
	pointLight4->SetShader(lightShaderHandle);
	pointLight4->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	pointLight4->renderComponent->SetColour(1.0f, 1.0f, 1.0f);
	pointLight4->renderComponent->CanRender(false);
	pointLight4->canRender = false;
	pointLight4->SetWorldPosition(-5.25f, -0.5f, -5.5f);
	pointLight4->SetWorldScale(0.5f);
	pointLight4->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	pointLight4->lightComponent->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));
	//

	//
	blueLight = new Light();
	blueLight->SetShader(lightShaderHandle);
	blueLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	blueLight->renderComponent->SetColour(0.0f, 0.0f, 1.0f);
	blueLight->SetWorldPosition(0.0f, -2.95f, 5.0f);
	blueLight->SetWorldScale(11.0f, 5.0f, 0.5f);
	blueLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	blueLight->lightComponent->SetColour(glm::vec3(0.0f, 0.0f, 1.0f));
	//

	//
	redLight = new Light();
	redLight->SetShader(lightShaderHandle);
	redLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	redLight->renderComponent->SetColour(1.0f, 0.0f, 0.0f);
	redLight->SetWorldPosition(0.0f, -2.95f, -5.0f);
	redLight->SetWorldScale(11.0f, 5.0f, 0.5f);
	redLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	redLight->lightComponent->SetColour(glm::vec3(1.0f, 0.0f, 0.0f));
	//
	
	// Make directional light
	dirLight = new Light(LightComponent::Light_Type::DIRECTIONAL);
	dirLight->lightComponent->SetDirection(glm::vec3(1.0f, -1.0f, 1.0f));
	dirLight->lightComponent->SetColour(glm::vec3(0.2f, 0.2f, 0.2f));
	//

	// fireworks
	particle1 = new ParticleSystem(sceneManager->GetRenderer()->GetShaderManager());
	particle1->SetWorldPosition(-20.0f, 0.0f, 0.0f);
	// fireworks
	particle2 = new ParticleSystem(sceneManager->GetRenderer()->GetShaderManager());
	particle2->SetWorldPosition(20.0f, 0.0f, 0.0f);
	// fireworks
	particle3 = new ParticleSystem(sceneManager->GetRenderer()->GetShaderManager());
	particle3->SetWorldPosition(0.0f, 0.0f, 20.0f);
	// fireworks
	particle4 = new ParticleSystem(sceneManager->GetRenderer()->GetShaderManager());
	particle4->SetWorldPosition(0.0f, 0.0f, -20.0f);

	gameManager = new GameManager(playerList);
	gameManager->Initialize();

	// Add scene objects
	AddObject(projectileManager->GetProjectileRenderer());
	AddLightObject(dirLight);
	AddLightObject(pointLight);
	AddLightObject(pointLight2);
	AddLightObject(pointLight3);
	AddLightObject(pointLight4);
	AddLightObject(blueLight);
	AddLightObject(redLight);
	AddUIObject(fadeImage);
	AddUIObject(roundTextFade);
	AddUIObject(roundText);
	AddObject(particle1);
	AddObject(particle2);
	AddObject(particle3);
	AddObject(particle4);

	// update phyics list
	PhysicsEngine::GetInstance()->AddObjectList(objectList);

	cameraCD = Cooldown(4.5f);
	cameraCD.startCD();

	roundCD = Cooldown(0.5f);

	return true;
}
void TvTGameScene::Update(const float deltaTime)
{
	projectileManager->Update(deltaTime);

	if (playingIntro) {
		PlayIntro();
	}
	if (!playingIntro && !startText) {
		PlayRoundStart();
	}
}

void TvTGameScene::FixedUpdate(const float deltaTime)
{
	// Update object list for physics
	PhysicsEngine::GetInstance()->AddObjectList(objectList);

	// Update physics and projectile manager
	PhysicsEngine::GetInstance()->Update(deltaTime);
	projectileManager->FixedUpdate(deltaTime);
	gameManager->Update();

	// Update projectiles in projectile manager
	if (projectileManager->GetProjectileList().size() != NULL) {
		for (size_t i = 0; i < projectileManager->GetProjectileList().size(); ++i) {
			if (projectileManager->GetProjectileList().at(i)->deleted) {
				projectileManager->RemoveProjectile(projectileManager->GetProjectileList().at(i));
			}
		}
	}
}
void TvTGameScene::HandleEvents(SDL_Event events)
{
	if (playingIntro) {
		if (events.type == SDL_JOYBUTTONDOWN) {
			if (events.jbutton.button == 7) //start button
			{
				SkipIntro();
			}
		}
	}

	if (roundStart) {

		// Camera look
		if (events.type == SDL_MOUSEMOTION) {
			cameraList[0]->ProcessMouseMovement((float)events.motion.x, (float)events.motion.y);
		}

		// PLAYER
		if (events.type == SDL_KEYDOWN && events.key.repeat == 0)
		{
			if (events.key.keysym.sym == SDLK_LEFT) {
				std::vector<Projectile*> p = player1->LightAttack();
				if (p.size() > 0) {
					for each (Projectile* subP in p) {
						projectileManager->AddProjectile(subP);
					}
				}
			}

			if (events.key.keysym.sym == SDLK_UP) {
				player1->Jump();
			}

			if (events.key.keysym.sym == SDLK_DOWN) {
				std::vector<Projectile*> p = player1->MediumAttack();
				if (p.size() > 0) {
					for each (Projectile* subP in p) {
						projectileManager->AddProjectile(subP);
					}
				}
			}

			if (events.key.keysym.sym == SDLK_RIGHT) {
				std::vector<Projectile*> p = player1->HeavyAttack();
				if (p.size() > 0) {
					for each (Projectile* subP in p) {
						projectileManager->AddProjectile(subP);
					}
				}
			}

			if (events.key.keysym.sym == SDLK_RCTRL) {
				std::vector<Projectile*> p = player1->SpecialAttack();
				if (p.size() > 0) {
					for each (Projectile* subP in p) {
						projectileManager->AddProjectile(subP);
					}
				}
			}

			if (events.key.keysym.sym == SDLK_o) {
				player1->SwitchTarget();
			}

			if (events.key.keysym.sym == SDLK_i) {
				player1->EnableTarget();
			}
		}

		// Call player handle events
		player1->HandleEvents(events);
		player2->HandleEvents(events);
		player3->HandleEvents(events);
		player4->HandleEvents(events);

	}

	if (events.jbutton.button == 7) //start button
	{
		if (gameManager->IsGameOver()) {
			Restart();
		}
	}
}

void TvTGameScene::HandleStates(const Uint8 *state)
{
	if (roundStart) {
		// Camera move
		if (state[SDL_SCANCODE_T]) {
			cameraList[0]->ProcessKeyboard(FORWARD, deltaTime);
		}
		if (state[SDL_SCANCODE_G]) {
			cameraList[0]->ProcessKeyboard(BACKWARD, deltaTime);
		}
		if (state[SDL_SCANCODE_H]) {
			cameraList[0]->ProcessKeyboard(RIGHT, deltaTime);
		}
		if (state[SDL_SCANCODE_F]) {
			cameraList[0]->ProcessKeyboard(LEFT, deltaTime);
		}

		if (state[SDL_SCANCODE_Z]) {
			bgm->Stop();
			sceneManager->SwitchScene(new TvTGameScene());
		}

		// Player 1 movement
		if (state[SDL_SCANCODE_W]) {
			player1->Movement(Player::PLAYERMOVEMENT::FORWARD, fixedDeltaTime);
		}
		if (state[SDL_SCANCODE_S]) {
			player1->Movement(Player::PLAYERMOVEMENT::BACKWARD, fixedDeltaTime);
		}
		if (state[SDL_SCANCODE_D]) {
			player1->Movement(Player::PLAYERMOVEMENT::RIGHT, fixedDeltaTime);
		}
		if (state[SDL_SCANCODE_A]) {
			player1->Movement(Player::PLAYERMOVEMENT::LEFT, fixedDeltaTime);
		}

		// Reload Scene
		if (state[SDL_SCANCODE_Z]) {
			sceneManager->SwitchScene(new TvTGameScene());
		}

		// Reload match
		if (state[SDL_SCANCODE_R]) {
			Restart();
		}

		// Call player handle states
		player1->HandleStates(state);
		player2->HandleStates(state);
		player3->HandleStates(state);
		player4->HandleStates(state);
	}

	if (state[SDL_SCANCODE_SPACE]) {
		SkipIntro();
	}
}

void TvTGameScene::CameraMove(ENGINE::Camera_Movement direction, float yaw, float pitch, float speed, int camera)
{
	cameraList[camera]->SetRotationX(pitch);
	cameraList[camera]->SetRotationY(yaw);
	cameraList[camera]->ProcessKeyboard(direction, speed * deltaTime);
}

void TvTGameScene::PlayIntro()
{
	bgm->SetVolume(50);
	if (playAudio) {
		bgm->Play(-1);
		playAudio = false;
	}
	
	if (cameraCD.secondsLeft() > 3) {
		CameraMove(RIGHT, -145.0f, 11.0f, 0.5f, 0);
	}
	else if (cameraCD.secondsLeft() > 1.5) {
		if (!cameraSwitch1) {
			cameraList[0]->Position = glm::vec3(-5.0f, 15.0f, 1.0f);
			cameraSwitch1 = true;
		}
		CameraMove(LEFT, -33.0f, -56.0f, 0.5f, 0);
	}
	else if (cameraCD.secondsLeft() > 0) {
		if (!cameraSwitch2) {
			cameraList[0]->Position = glm::vec3(6.0f, 8.0f, -12.0f);
			cameraList[0]->SetRotationY(-220.0f);
			cameraList[0]->SetRotationX(-29.0f);
			fadeAlpha = 1.0f;
			cameraSwitch2 = true;
		}
		cameraList[0]->SetRotationY(cameraList[0]->Yaw -= deltaTime * 10);
	}
	else if (cameraCD.checkOffCD()) {
		if (!cameraSwitch3) {
			gameManager->StartMatch();
			cameraList[0]->Position = glm::vec3(0.0f, 7.5f, 9.5f);
			cameraList[0]->SetRotationY(-90.0f);
			cameraList[0]->SetRotationX(-25.0f);
			cameraList[1]->Position = glm::vec3(0.0f, 7.5f, -9.5f);
			cameraList[1]->SetRotationY(90.0f);
			cameraList[1]->SetRotationX(-25.0f);

			sceneManager->EnableSplitscreen(true);

			cameraSwitch3 = true;
		}

		cameraList[0]->SetRotationX(cameraList[0]->Pitch -= deltaTime * 10);
		cameraList[1]->SetRotationX(cameraList[0]->Pitch);

		if (cameraList[0]->Pitch <= -45.0f) {
			cameraList[0]->Pitch = -45.0f;
			cameraList[1]->Pitch = -45.0f;
			fadeImage->SetVisible(false);
			roundText->SetVisible(true);
			roundCD.startCD();
			fadeAlpha = 1.0f;
			playingIntro = false;
		}
	}

	fadeAlpha -= deltaTime;
	fadeImage->SetAlpha(fadeAlpha);
	if (fadeAlpha <= 0) {
		fadeAlpha = 0;
	}
}

void TvTGameScene::SkipIntro()
{
	if (playingIntro) {
		playingIntro = false;

		cameraSwitch1 = true;
		cameraSwitch2 = true;
		cameraSwitch3 = true;

		gameManager->StartMatch();

		fadeImage->SetVisible(false);

		sceneManager->EnableSplitscreen(true);

		fadeAlpha = 1.0f;
		fadeImage->SetAlpha(1.0f);

		roundText->SetVisible(true);
		
		roundCD.startCD();

		cameraList[0]->Position = glm::vec3(0.0f, 7.5f, 9.5f);
		cameraList[0]->SetRotationY(-90.0f);
		cameraList[0]->SetRotationX(-45.0f);
		cameraList[1]->Position = glm::vec3(0.0f, 7.5f, -9.5f);
		cameraList[1]->SetRotationY(90.0f);
		cameraList[1]->SetRotationX(-45.0f);
	}
}

void TvTGameScene::PlayRoundStart()
{
	if (!ready) {
		roundText->SetText("Ready?");
		roundText->SetSize(roundText->GetSize() + deltaTime * 5);
		if (roundText->GetSize() >= 1.0f) {
			roundText->SetSize(1.0f);
		}
		roundText->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundText->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
		if (roundCD.checkOffCD()) {
			roundText->SetSize(10.0f);
			roundText->SetText("Round Start");
			ready = true;
			gameManager->StartTimer();
		}
	}
	if (ready) {
		roundText->SetSize(roundText->GetSize() - deltaTime * 40);
		roundText->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundText->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
		if (roundText->GetSize() < 1.0f) {
			roundText->SetSize(1.0f);
			roundText->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundText->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
			roundTextFade->SetVisible(true);
			roundCD.setNewDuration(1.0);
			roundCD.startCD();
			roundStart = true;
		}

		if (roundCD.secondsLeft() > 0.8) {
			roundTextFade->SetSize(roundTextFade->GetSize() + deltaTime);
			roundTextFade->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundTextFade->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
		}
		else if (roundCD.secondsLeft() > 0.6) {
			roundTextFade->SetSize(roundTextFade->GetSize() + deltaTime * 10);
			roundTextFade->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundTextFade->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
		}
		else if (roundCD.secondsLeft() > 0.4) {
			roundTextFade->SetSize(roundTextFade->GetSize() + deltaTime * 5);
			roundTextFade->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundTextFade->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
		}
		if (roundCD.secondsLeft() > 0) {
			fadeAlpha -= deltaTime;
			if (fadeAlpha <= 0) {
				fadeAlpha = 0;
			}
			roundTextFade->SetAlpha(fadeAlpha / 2);
			roundText->SetAlpha(fadeAlpha);
		}
		if (fadeAlpha == 0) {
			roundText->SetVisible(false);
			roundTextFade->SetVisible(false);
			startText = true;
		}
	}
}

void TvTGameScene::SetUpPlayers()
{
	// Add players to the player list
	playerList.push_back(player1);
	playerList.push_back(player2);
	playerList.push_back(player3);
	playerList.push_back(player4);

	// Loop through player list and create character based on save data from chararcter select
	for (unsigned int i = 0; i < playerList.size(); i++)
	{
		if (sceneManager->saveData.size() != 0) {
			if (sceneManager->saveData[i] == 0) {
				playerList.at(i) = new LightningPlayer();
			}
			else if (sceneManager->saveData[i] == 1) {
				playerList.at(i) = new EarthPlayer();
			}
			else if (sceneManager->saveData[i] == 2) {
				playerList.at(i) = new IcePlayer();
			}
			else if (sceneManager->saveData[i] == 3) {
				playerList.at(i) = new FirePlayer();
			}
			else if (sceneManager->saveData[i] == 4) {
				playerList.at(i) = new WindPlayer();
			}
			else
			{
				playerList.at(i) = new WindPlayer();
			}
		}
		else
		{
			playerList.at(i) = new WindPlayer();
		}
	}

	// Assign player reference
	player1 = playerList[0];
	player2 = playerList[1];
	player3 = playerList[2];
	player4 = playerList[3];

	// Set Shaders
	player1->SetShader(defaultShaderHandle);
	player2->SetShader(defaultShaderHandle);
	player3->SetShader(defaultShaderHandle);
	player4->SetShader(defaultShaderHandle);

	// Set Positions
	player1->SetWorldPosition(glm::vec3(-3.0f, 0.0f, 4.0f));
	player2->SetWorldPosition(glm::vec3(3.0f, 0.0f, 4.0f));
	player3->SetWorldPosition(glm::vec3(3.0f, 0.0f, -4.0f));
	player4->SetWorldPosition(glm::vec3(-3.0f, 0.0f, -4.0f));

	// Set player numbers
	player1->SetPlayerNumber(Player::PLAYERNUMBER::PLAYER1);
	player2->SetPlayerNumber(Player::PLAYERNUMBER::PLAYER2);
	player3->SetPlayerNumber(Player::PLAYERNUMBER::PLAYER3);
	player4->SetPlayerNumber(Player::PLAYERNUMBER::PLAYER4);

	// Set player teams
	player1->SetPlayerTeam(Player::PLAYERTEAM::TEAM1);
	player2->SetPlayerTeam(Player::PLAYERTEAM::TEAM1);
	player3->SetPlayerTeam(Player::PLAYERTEAM::TEAM2);
	player4->SetPlayerTeam(Player::PLAYERTEAM::TEAM2);

	// Assign controllers saved from character select if there are any
	if (sceneManager->controllers.size() != 0) {
		player1->GetPlayerInput()->SetJoystick(sceneManager->controllers[0]);
		player2->GetPlayerInput()->SetJoystick(sceneManager->controllers[1]);
		player3->GetPlayerInput()->SetJoystick(sceneManager->controllers[2]);
		player4->GetPlayerInput()->SetJoystick(sceneManager->controllers[3]);
	}

	// Add projectile manager
	player1->AddProjecitleManager(projectileManager);
	player2->AddProjecitleManager(projectileManager);
	player3->AddProjecitleManager(projectileManager);
	player4->AddProjecitleManager(projectileManager);

	// Set the players to not  move when scene starts
	player1->SetCanMove(false);
	player2->SetCanMove(false);
	player3->SetCanMove(false);
	player4->SetCanMove(false);

	// Enemy Target set up
	player1->SetEnemyTeam(player3, player4);
	player2->SetEnemyTeam(player4, player3);
	player3->SetEnemyTeam(player1, player2);
	player4->SetEnemyTeam(player2, player1);

	// Add players to object list
	AddObject(player1);
	AddObject(player2);
	AddObject(player3);
	AddObject(player4);

	// Add players to the projectile manager
	projectileManager->AddPlayer(player1);
	projectileManager->AddPlayer(player2);
	projectileManager->AddPlayer(player3);
	projectileManager->AddPlayer(player4);
}

void TvTGameScene::SetUpArena()
{
	// Make models
	//
	platform = new Model("Resources/Models/platform/PP_steel_podium_platinum.obj");
	platform->SetShader(defaultShaderHandle);
	platform->physicsComponent->SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));
	platform->SetWorldScale(0.13f, 0.1f, 0.06f);
	platform->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	platform->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	platform->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	platform->physicsComponent->SetMass(0.0f);
	platform->collisionComponent->SetLayer(20);
	//

	//
	topRing = new Model("Resources/Models/ring/ring.obj");
	topRing->SetShader(lightShaderHandle);
	topRing->physicsComponent->SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
	topRing->SetWorldScale(0.05f, 0.025f, 0.05f);
	topRing->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	topRing->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	topRing->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	topRing->physicsComponent->SetMass(0.0f);
	topRing->collisionComponent->SetLayer(20);
	//

	//
	middleRing = new Model("Resources/Models/ring/ring.obj");
	middleRing->SetShader(lightShaderHandle);
	middleRing->physicsComponent->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
	middleRing->SetWorldScale(0.15f, 0.025f, 0.15f);
	middleRing->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	middleRing->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	middleRing->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	middleRing->physicsComponent->SetMass(0.0f);
	middleRing->collisionComponent->SetLayer(20);
	//

	//
	bottomRing = new Model("Resources/Models/ring/ring.obj");
	bottomRing->SetShader(lightShaderHandle);
	bottomRing->physicsComponent->SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));
	bottomRing->SetWorldScale(0.15f, 0.5f, 0.15f);
	bottomRing->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	bottomRing->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	bottomRing->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	bottomRing->physicsComponent->SetMass(0.0f);
	bottomRing->collisionComponent->SetLayer(20);
	//

	// floor collision
	floor = new Cube();
	floor->SetShader(defaultShaderHandle);
	floor->renderComponent->SetColour(0.1f, 0.1f, 0.1f);
	floor->physicsComponent->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
	floor->SetWorldScale(11.0f, 1.0f, 12.0f);
	floor->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	floor->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NORMAL_ELASTIC);
	floor->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::SMOOTH);
	floor->physicsComponent->SetMass(0.0f);
	floor->canRender = false;
	//

	projectileManager->AddEnvironment(floor);

	AddObject(platform);
	AddObject(topRing);
	AddObject(middleRing);
	AddObject(bottomRing);
	AddObject(floor);
}

void TvTGameScene::Restart()
{
	fadeImage->SetVisible(true);

	gameManager->Reset();

	sceneManager->EnableSplitscreen(false);
	cameraList[0]->Position = glm::vec3(7.0f, 0.7f, 7.0f);
	cameraList[0]->Yaw = 0.0f;
	cameraList[0]->Pitch = 0.0f;
	cameraList[1]->Position = glm::vec3(7.0f, 0.7f, 7.0f);

	roundText->SetText("Ready?");
	roundText->SetSize(0.0f);
	roundText->SetSpacing(9.0f);
	roundText->SetAlpha(1.0f);
	roundText->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundText->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
	roundText->SetVisible(false);

	roundTextFade->SetSize(1.0f);
	roundTextFade->SetSpacing(9.0f);
	roundTextFade->SetPosition((sceneManager->GetScreenWidth() / 2) - (roundText->GetLength() / 2), sceneManager->GetScreenHeight() / 2);
	roundTextFade->SetVisible(false);

	cameraCD = Cooldown(4.5);
	cameraCD.startCD();

	roundCD = Cooldown(0.5);

	fadeAlpha = 1.0f;
	cameraSwitch1 = false;
	cameraSwitch2 = false;
	cameraSwitch3 = false;
	playingIntro = true;
	ready = false;
	startText = false;
	roundStart = false;
	playAudio = true;
}