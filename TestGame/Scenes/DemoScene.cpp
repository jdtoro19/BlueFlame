#include "DemoScene.h"

using namespace GAME;

DemoScene::DemoScene()
{
}

DemoScene::~DemoScene()
{
}

bool DemoScene::Initialize()
{
	// Make reference to the scene manager
	sceneManager = BFEngine::GetInstance()->GetSceneManager();

	// Set screen options
	sceneManager->EnableSplitscreen(true);
	sceneManager->EnableFullscreen(false);
	sceneManager->ShowFPS(true);
	sceneManager->CaptureMouse(true);

	// Set the options for the first camera
	cameraList[0]->Position = glm::vec3(0.0f, 6.0f, 10.0f);
	cameraList[0]->SetRotationX(-25.0f);
	// Set the options for the second camera
	cameraList[1]->Position = glm::vec3(0.0f, 6.0f, -10.0f);
	cameraList[1]->SetRotationY(90.0f);
	cameraList[1]->SetRotationX(-25.0f);

	// Load shaders
	defaultShader = new Shader("Shaders/model.vs", "Shaders/model.fs");
	skyboxShader = new Shader("Shaders/skybox.vs", "Shaders/skybox.fs");

	// Put shaders into shader manager
	defaultShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("default"), defaultShader);
	skyboxShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("skybox"), skyboxShader);

	// Lights
	//
	// Initialize
	pointLight = new Light(LightComponent::Light_Type::POINTLIGHT);
	// Set which shader this object is using
	pointLight->SetShader(defaultShaderHandle);
	// Set type and colour of object's mesh
	pointLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	pointLight->renderComponent->SetColour(0.0f, 0.0f, 1.0f);
	pointLight->renderComponent->CanRender(false);
	// Set position and scale of object
	pointLight->SetWorldPosition(0.0f, 5.0f, 0.0f);
	pointLight->SetWorldScale(0.5f);
	// Set what type of light and what colour this object emits
	pointLight->lightComponent->SetColour(glm::vec3(1.0f, 2.0f, 1.0f));
	//	

	// Make directional light
	dirLight = new Light(LightComponent::Light_Type::DIRECTIONAL);
	dirLight->lightComponent->SetDirection(glm::vec3(1.0f, -1.0f, 1.0f));

	// Make Projectile Manager
	projectileManager = new ProjectileManager();
	
	// Make Players
	//
	// Player 1
	player1 = new EarthPlayer();
	player1->SetShader(defaultShaderHandle);
	player1->SetWorldPosition(glm::vec3(-2.0f, 0.0f, 3.0f));
	player1->SetPlayerNumber(Player::PLAYERNUMBER::PLAYER1);
	player1->SetPlayerTeam(Player::PLAYERTEAM::TEAM1);
	// Bind Controller
	if (BFEngine::GetInstance()->numPlayers > 0) {
		BFEngine::GetInstance()->players[BFEngine::GetInstance()->indexOfPlayer[0]].playerControls(player1);
	}

	// Player 2
	player2 = new IcePlayer();
	player2->SetShader(defaultShaderHandle);
	player2->SetWorldPosition(glm::vec3(2.0f, 0.0f, 3.0f));
	player2->SetPlayerNumber(Player::PLAYERNUMBER::PLAYER2);
	player2->SetPlayerTeam(Player::PLAYERTEAM::TEAM1);
	// Bind Controller
	if (BFEngine::GetInstance()->numPlayers > 1) {
		BFEngine::GetInstance()->players[BFEngine::GetInstance()->indexOfPlayer[1]].playerControls(player2);
	}

	// Player 3
	player3 = new FirePlayer();
	player3->SetShader(defaultShaderHandle);
	player3->SetWorldPosition(glm::vec3(2.0f, 0.0f, -3.0f));
	player3->SetPlayerNumber(Player::PLAYERNUMBER::PLAYER3);
	player3->SetPlayerTeam(Player::PLAYERTEAM::TEAM2);
	// Bind Controller
	if (BFEngine::GetInstance()->numPlayers > 2) {
		BFEngine::GetInstance()->players[BFEngine::GetInstance()->indexOfPlayer[2]].playerControls(player3);
		BFEngine::GetInstance()->players[BFEngine::GetInstance()->indexOfPlayer[2]].setTeam(1);
	}

	// Player 4
	player4 = new EarthPlayer();
	player4->SetShader(defaultShaderHandle);
	player4->SetWorldPosition(glm::vec3(-2.0f, 0.0f, -3.0f));
	player4->SetPlayerNumber(Player::PLAYERNUMBER::PLAYER4);
	player4->SetPlayerTeam(Player::PLAYERTEAM::TEAM2);
	// Bind Controller
	if (BFEngine::GetInstance()->numPlayers > 3) {
		BFEngine::GetInstance()->players[BFEngine::GetInstance()->indexOfPlayer[3]].playerControls(player4);
		BFEngine::GetInstance()->players[BFEngine::GetInstance()->indexOfPlayer[3]].setTeam(1);
	}

	// Enemy Target set up
	player1->SetEnemyTeam(player3, player4);
	player2->SetEnemyTeam(player4, player3);
	player3->SetEnemyTeam(player1, player2);
	player4->SetEnemyTeam(player2, player1);

	// Arena
	floor = new Cube();
	floor->SetShader(defaultShaderHandle);
	floor->renderComponent->SetColour(0.1f, 0.1f, 0.1f);
	floor->physicsComponent->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
	floor->SetWorldScale(11.0f, 1.0f, 11.0f);
	floor->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	floor->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	floor->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	floor->physicsComponent->SetMass(0.0f);

	wall = new Cube();
	wall->SetShader(defaultShaderHandle);
	wall->renderComponent->SetColour(0.1f, 0.1f, 0.1f);
	wall->physicsComponent->SetPosition(glm::vec3(5.5f, 0.0f, 0.0f));
	wall->SetWorldScale(1.0f, 2.0f, 11.0f);
	wall->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	wall->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::PERFECT_NON_ELASTIC);
	wall->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	wall->physicsComponent->SetMass(0.0f);

	wall1 = new Cube();
	wall1->SetShader(defaultShaderHandle);
	wall1->renderComponent->SetColour(0.1f, 0.1f, 0.1f);
	wall1->physicsComponent->SetPosition(glm::vec3(-5.5f, 0.0f, 0.0f));
	wall1->SetWorldScale(1.0f, 2.0f, 11.0f);
	wall1->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	wall1->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::PERFECT_NON_ELASTIC);
	wall1->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	wall1->physicsComponent->SetMass(0.0f);

	middleWall = new Cube();
	middleWall->SetShader(defaultShaderHandle);
	middleWall->renderComponent->SetColour(1.0f, 0.0f, 0.0f);
	middleWall->physicsComponent->SetPosition(glm::vec3(0.0f, -0.50f, 0.0f));
	middleWall->SetWorldScale(11.0f, 0.1f, 1.0f);
	middleWall->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	middleWall->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::PERFECT_NON_ELASTIC);
	middleWall->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::PERFECT_ROUGH);
	middleWall->physicsComponent->SetMass(0.0f);

	projectileManager->AddEnvironment(floor);
	projectileManager->AddEnvironment(wall);
	projectileManager->AddEnvironment(wall1);

	// Make skybox, load its textures, set properties, and give to the renderer
	skybox = new Skybox();
	std::vector<char*> faces;
	faces.push_back("Resources/Textures/Skyboxes/right.jpg");
	faces.push_back("Resources/Textures/Skyboxes/left.jpg");
	faces.push_back("Resources/Textures/Skyboxes/top.jpg");
	faces.push_back("Resources/Textures/Skyboxes/bottom.jpg");
	faces.push_back("Resources/Textures/Skyboxes/back.jpg");
	faces.push_back("Resources/Textures/Skyboxes/front.jpg");
	skybox->LoadTextures(faces);
	skybox->SetShader(skyboxShaderHandle);

	// add objects and lights to their lists
	AddObject(player1);
	AddObject(player2);
	AddObject(player3);
	AddObject(player4);
	AddObject(floor);
	AddObject(wall);
	AddObject(wall1);
	AddObject(middleWall);
	AddObject(projectileManager->GetProjectileRenderer());
	AddLightObject(dirLight);
	AddLightObject(pointLight);

	// update phyics list
	PhysicsEngine::GetInstance()->AddObjectList(objectList);

	// Add players to the projectile manager
	projectileManager->AddPlayer(player1);
	projectileManager->AddPlayer(player2);
	projectileManager->AddPlayer(player3);
	projectileManager->AddPlayer(player4);

	playerList.push_back(player1);
	playerList.push_back(player2);
	playerList.push_back(player3);
	playerList.push_back(player4);

	moveSpeed = 200.0f;

	return true;
}

void DemoScene::Update(const float deltaTime)
{
	sceneManager->DebugText("Z to reload scene");
	sceneManager->DebugText("Right Arrow Key for Light Attack");
	sceneManager->DebugText("Press 3 times for combo");
	sceneManager->DebugText("E for shield");

	this->deltaTime = deltaTime;

	// Timer for firing (Keyboard controls only)
	timer -= deltaTime;
	if (timer < 0) {
		timer = 0.3f;
		fire = true;
	}

	// Update object list for physics
	PhysicsEngine::GetInstance()->AddObjectList(objectList);

	// Update physics and projectile manager
	PhysicsEngine::GetInstance()->Update(deltaTime);
	projectileManager->Update(deltaTime);

	// Update projectiles in projectile manager
	if (projectileManager->GetProjectileList().size() != NULL) {
		for (size_t i = 0; i < projectileManager->GetProjectileList().size(); ++i) {
			if (projectileManager->GetProjectileList().at(i)->deleted) {
				projectileManager->RemoveProjectile(projectileManager->GetProjectileList().at(i));
			}
		}
	}

	// Update Controller
	if (InputHandler::GetInstance()->areJoysticksLive()) {
		// Player Movement
		for (int i = 0; i < InputHandler::GetInstance()->jCheck(); i++) {

			glm::vec3 mods = InputHandler::GetInstance()->playerMotion(i);

			mods *= BFEngine::GetInstance()->players[i].inverted();

			//glm::vec3 temp = BFEngine::GetInstance()->players[i].pObject()->physicsComponent->GetVelocity();

			//BFEngine::GetInstance()->players[i].pObject()->physicsComponent->SetVelocity(temp + glm::vec3(mods.x * moveSpeed * deltaTime, 0.0f, mods.y * moveSpeed * deltaTime));

			if (InputHandler::GetInstance()->playerMotion(i).x > 0.01f) {
				playerList.at(i)->Movement(Player::PLAYERMOVEMENT::RIGHT, deltaTime);
			}
			if (InputHandler::GetInstance()->playerMotion(i).x < -0.01f) {
				playerList.at(i)->Movement(Player::PLAYERMOVEMENT::LEFT, deltaTime);
			}
			if (InputHandler::GetInstance()->playerMotion(i).y > 0.01f) {
				playerList.at(i)->Movement(Player::PLAYERMOVEMENT::BACKWARD, deltaTime);
			}
			if (InputHandler::GetInstance()->playerMotion(i).y < -0.01f) {
				playerList.at(i)->Movement(Player::PLAYERMOVEMENT::FORWARD, deltaTime);
			}

			// Player fire
			if (BFEngine::GetInstance()->players[i].shotDelay == 0) {
				//if (SDL_JoystickGetButton(BFEngine::GetInstance()->players[i].pStick, 1) == 1) {
				if (SDL_JoystickGetAxis(BFEngine::GetInstance()->players[i].pStick, 5) > 1000) {
					/*
					GameObject* lr = BFEngine::GetInstance()->players[i].pObject(); //line reducer
					Projectile* p = new Projectile(glm::vec3(lr->GetWorldPosition().x, lr->GetWorldPosition().y, lr->GetWorldPosition().z - BFEngine::GetInstance()->players[i].inverted() * lr->collisionComponent->GetBoundingBox().r.z * 2.0f * lr->GetWorldScale().z), glm::vec3(0.0f, 0.0f, 100000.0f), BFEngine::GetInstance()->players[i].inverted());
					glm::vec3 tempColor = BFEngine::GetInstance()->players[i].getTeamColor();
					p->renderComponent->SetColour(tempColor.x, tempColor.y, tempColor.z);
					p->SetShader(defaultShaderHandle);
					AddObject(p);
					PhysicsEngine::GetInstance()->AddObjectList(objectList);
					projectileManager->AddProjectile(p);
					std::cout << "Player " << i << " shot." << std::endl;
					BFEngine::GetInstance()->players[i].weShot();
					*/
				}
			}
			BFEngine::GetInstance()->players[i].tick();
		}
	}
}

void DemoScene::FixedUpdate(const float deltaTime) {


}

void DemoScene::HandleEvents(SDL_Event events)
{
	//joystick
	switch (events.type)
	{
	case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */
		if (events.jaxis.axis == 2) //left trigger
		{
			/* Up-Down movement code goes here */
			//std::cout << "Left Trigger" << std::endl;
			//cube->physicsComponent->SetVelocity(glm::vec3(-1.0f, 0.0f, 0.0f));
		}
		if (events.jaxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) //right trigger
		{
			/* Up-Down movement code goes here */
			//std::cout << "Right Trigger" << std::endl;
			//cube->physicsComponent->SetVelocity(glm::vec3(1.0f, 0.0f, 0.0f));
			playerList.at(events.jbutton.which)->Jump();
		}
		break;

	case SDL_JOYBUTTONDOWN:  /* Handle Joystick Button Presses */
		if (events.jbutton.button == 0) //A button
		{
			/* code goes here */
			//std::cout << "A button was probably pressed by " << events.jbutton.which << std::endl;
			std::vector<Projectile*> p = playerList.at(events.jbutton.which)->SpecialAttack();
			// Add it to the projectile manager if the returned projectile is not null
			if (p.size() > 0) {
				for each (Projectile* subP in p) {
					projectileManager->AddProjectile(subP);
				}
			}
		}
		if (events.jbutton.button == 1) //B button
		{
			/* code goes here */
			std::vector<Projectile*> p = playerList.at(events.jbutton.which)->HeavyAttack();
			// Add it to the projectile manager if the returned projectile is not null
			if (p.size() > 0) {
				for each (Projectile* subP in p) {
					projectileManager->AddProjectile(subP);
				}
			}
		}
		if (events.jbutton.button == 2) //X button
		{
			/* code goes here */
			//std::cout << "X button was probably pressed." << std::endl;
			std::vector<Projectile*> p = playerList.at(events.jbutton.which)->LightAttack();
			// Add it to the projectile manager if the returned projectile is not null
			if (p.size() > 0) {
				for each (Projectile* subP in p) {
					projectileManager->AddProjectile(subP);
				}
			}
		}
		if (events.jbutton.button == 3) //Y button
		{
			/* code goes here */
			//std::cout << "Y button was probably pressed." << std::endl;
			std::vector<Projectile*> p = playerList.at(events.jbutton.which)->MediumAttack();
			// Add it to the projectile manager if the returned projectile is not null
			if (p.size() > 0) {
				for each (Projectile* subP in p) {
					projectileManager->AddProjectile(subP);
				}
			}
		}
		if (events.jbutton.button == 4) //left bumper
		{
			/* code goes here */
			//std::cout << "Left bumper was probably pressed." << std::endl;
			//cube->physicsComponent->SetVelocity(glm::vec3(0.0f, 0.0f, -1.0f));
			playerList.at(events.jbutton.which)->EnableTarget();
		}
		if (events.jbutton.button == 5) //right bumper
		{
			/* code goes here */
			//std::cout << "right bumper was probably pressed." << std::endl;
			//cube->physicsComponent->SetVelocity(glm::vec3(0.0f, 0.0f, 1.0f));
			playerList.at(events.jbutton.which)->SwitchTarget();
		}
		if (events.jbutton.button == 6) //back button
		{
			/* code goes here */
			//std::cout << "back button was probably pressed." << std::endl;
			sceneManager->SetIsQuit(true);
		}
		if (events.jbutton.button == 7) //start button
		{
			/* code goes here */
			//std::cout << "start was probably pressed." << std::endl;
			sceneManager->EnableSplitscreen(true);
			sceneManager->EnableFullscreen(false);
			sceneManager->SwitchScene(new DemoScene());
		}
		if (events.jbutton.button == 8) //left joystick
		{
			/* code goes here */
			//std::cout << "left joystick was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 9) //right joystick
		{
			/* code goes here */
			//std::cout << "right joystick was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 10) //xbox key
		{
			/* code goes here */
			//std::cout << "xbox key was probably pressed." << std::endl;
			//xbox key closes the game
			//BFEngine::GetInstance()->TerminateGame();
		}
		if (events.jbutton.button == 11) //left trigger
		{
			/* code goes here */
			//std::cout << "left trigger was probably pressed." << std::endl;
		}
		if (events.jbutton.button == 12) //right trigger
		{
			/* code goes here */
			//std::cout << "right trigger was probably pressed." << std::endl;			
		}
		break;

	case SDL_JOYBALLMOTION:  /* Handle Joyball Motion */
		if (events.jball.ball == 0)
		{
			/* ball handling */
		}
		break;

	case SDL_JOYHATMOTION:  /* Handle Hat Motion */
		if (events.jhat.value & SDL_HAT_UP)
		{
			/* Do up stuff here */
			cout << "up" << endl;
		}
		if (events.jhat.value & SDL_HAT_DOWN)
		{
			/* Do down stuff here */
			cout << "down" << endl;
		}
		if (events.jhat.value & SDL_HAT_LEFT)
		{
			/* Do left stuff here */
			cout << "left" << endl;
		}

		if (events.jhat.value & SDL_HAT_RIGHT)
		{
			/* Do right  stuff here */
			cout << "right" << endl;
		}
		break;
	}

	// PLAYER
	// Light Attack
	if (events.type == SDL_KEYDOWN && events.key.repeat == 0)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_LEFT:
			// Create projectile
			// LightAttack returns a projectile
			std::vector<Projectile*> p = player1->LightAttack();
			// Add it to the projectile manager if the returned projectile is not null
			if (p.size() > 0) {
				for each (Projectile* subP in p) {
					projectileManager->AddProjectile(subP);
				}
			}
			break;
		}
	}
	if (events.type == SDL_KEYDOWN && events.key.repeat == 0)
	{
		if (events.key.keysym.sym == SDLK_UP) {
			player1->Jump();
		}

		if (events.key.keysym.sym == SDLK_DOWN) {
			std::vector<Projectile*> p = player1->MediumAttack();
			// Add it to the projectile manager if the returned projectile is not null
			if (p.size() > 0) {
				for each (Projectile* subP in p) {
					projectileManager->AddProjectile(subP);
				}
			}
		}

		if (events.key.keysym.sym == SDLK_RIGHT) {
			std::vector<Projectile*> p = player1->HeavyAttack();
			// Add it to the projectile manager if the returned projectile is not null
			if (p.size() > 0) {
				for each (Projectile* subP in p) {
					projectileManager->AddProjectile(subP);
				}
			}
		}

		if (events.key.keysym.sym == SDLK_RCTRL) {
			std::vector<Projectile*> p = player1->SpecialAttack();
			// Add it to the projectile manager if the returned projectile is not null
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
	// only player 1 for now to test
	player1->HandleEvents(events);
	player2->HandleEvents(events);
	player3->HandleEvents(events);
	player4->HandleEvents(events);
}

void DemoScene::HandleStates(const Uint8 *state)
{
	// Call player handle states
	// Only player 1 for now to test
	player1->HandleStates(state);
	player2->HandleStates(state);
	player3->HandleStates(state);
	player4->HandleStates(state);

	/*
	// TEST PROJECTILES
	//
	if (state[SDL_SCANCODE_SPACE]) {
		if (fire) {
			Projectile* p = new Projectile(glm::vec3(player1->GetWorldPosition().x, player1->GetWorldPosition().y, player1->GetWorldPosition().z - player1->collisionComponent->GetBoundingBox().r.z * 2.0f * player1->GetWorldScale().z), glm::vec3(0.0f, 200.0f, 50.0f), player1->targetAngle, player1->dir);
			p->SetActingForce(glm::vec3(0.0f, -4.0f, 0.0f));
			p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 25.0f));
			p->SetWorldScale(0.5f);
			p->SetShader(defaultShaderHandle);
			projectileManager->AddProjectile(p);
			fire = false;
		}
	}

	if (state[SDL_SCANCODE_R]) {
		if (fire) {
			Projectile* p = new Projectile(glm::vec3(player1->GetWorldPosition().x, player1->GetWorldPosition().y + 6.0f, player1->GetWorldPosition().z - player1->collisionComponent->GetBoundingBox().r.z * 2.0f * player1->GetWorldScale().z - 5.0f), glm::vec3(0.0f, -2.0f, 0.0f), player1->targetAngle, player1->dir);
			p->SetShader(defaultShaderHandle);
			p->SetWorldScale(0.25f, 1.0f, 0.25f);
			projectileManager->AddProjectile(p);
			fire = false;
		}
	}

	if (state[SDL_SCANCODE_T]) {
		if (fire) {
			Projectile* p = new Projectile(glm::vec3(player1->GetWorldPosition().x, player1->GetWorldPosition().y, player1->GetWorldPosition().z - player1->collisionComponent->GetBoundingBox().r.z * 2.0f * player1->GetWorldScale().z), glm::vec3(200.0f, 200.0f, 50.0f), player1->targetAngle, player1->dir);
			p->SetActingForce(glm::vec3(-4.0f, -4.0f, 0.0f));
			p->SetKnockbackForce(glm::vec3(-20.0f, 100.0f, 25.0f));
			p->SetWorldRotation(0.0f, 0.0f, 1.0f, 25.0f);
			p->SetWorldScale(1.0f, 0.2f, 0.75f);
			p->SetShader(defaultShaderHandle);
			projectileManager->AddProjectile(p);
			fire = false;
		}
	}
	//
	*/

	// Player movement
	// Only player 1 for now to test
	if (state[SDL_SCANCODE_W]) {
		player1->Movement(Player::PLAYERMOVEMENT::FORWARD, deltaTime);
	}
	if (state[SDL_SCANCODE_S]) {
		player1->Movement(Player::PLAYERMOVEMENT::BACKWARD, deltaTime);
	}
	if (state[SDL_SCANCODE_D]) {
		player1->Movement(Player::PLAYERMOVEMENT::RIGHT, deltaTime);
	}
	if (state[SDL_SCANCODE_A]) {
		player1->Movement(Player::PLAYERMOVEMENT::LEFT, deltaTime);
	}

	// Reload Scene
	if (state[SDL_SCANCODE_Z]) {
		sceneManager->SwitchScene(new DemoScene());
	}

	// ENABLE OR DISABLE FULLSCREEN WITH V AND B
	// ENABLE OR DISABLE SPLITSCREEN WITH N AND M
}