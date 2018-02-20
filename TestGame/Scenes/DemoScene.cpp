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
	sceneManager->EnableFullscreen(true);
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
	defaultShader = new Shader("Shaders/default.vs", "Shaders/default.fs");
	skyboxShader = new Shader("Shaders/skybox.vs", "Shaders/skybox.fs");

	// Put shaders into shader manager
	defaultShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("cube"), defaultShader);
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
	pointLight->lightComponent->SetColour(glm::vec3(2.0f, 2.0f, 2.0f));
	//	
	
	// Make directional light
	dirLight = new Light(LightComponent::Light_Type::DIRECTIONAL);
	dirLight->lightComponent->SetDirection(glm::vec3(1.0f, -1.0f, 1.0f));

	// Objects

	// Player 1
	cube1 = new Cube();
	//cube1->SetWorldScale(1.0f, 1.5f, 0.1f);
	cube1->SetShader(defaultShaderHandle);
	cube1->renderComponent->SetColour(0.5f, 0.0f, 0.5f);
	cube1->physicsComponent->SetPosition(glm::vec3(-2.0f, 0.0f, 3.0f));
	cube1->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
	cube1->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	cube1->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	cube1->physicsComponent->SetMass(50.0f);

	// Bind Controller
	if (BFEngine::GetInstance()->numPlayers > 0) {
		BFEngine::GetInstance()->players[BFEngine::GetInstance()->indexOfPlayer[0]].playerControls(cube1);
	}

	// Player 2
	cube2 = new Cube();
	//cube2->SetWorldScale(1.0f, 1.5f, 0.1f);
	cube2->SetShader(defaultShaderHandle);
	cube2->renderComponent->SetColour(0.5f, 0.0f, 0.5f);
	cube2->physicsComponent->SetPosition(glm::vec3(2.0f, 0.0f, 3.0f));
	cube2->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
	cube2->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	cube2->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	cube2->physicsComponent->SetMass(50.0f);

	if (BFEngine::GetInstance()->numPlayers > 1) {
		BFEngine::GetInstance()->players[BFEngine::GetInstance()->indexOfPlayer[1]].playerControls(cube2);
	}

	// Player 3
	cube3 = new Cube();
	//cube3->SetWorldScale(1.0f, 1.5f, 0.1f);
	cube3->SetShader(defaultShaderHandle);
	cube3->renderComponent->SetColour(0.0f, 0.5f, 0.0f);
	cube3->physicsComponent->SetPosition(glm::vec3(2.0f, 0.0f, -3.0f));
	cube3->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
	cube3->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	cube3->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	cube3->physicsComponent->SetMass(50.0f);

	if (BFEngine::GetInstance()->numPlayers > 2) {
		BFEngine::GetInstance()->players[BFEngine::GetInstance()->indexOfPlayer[2]].playerControls(cube3);
		BFEngine::GetInstance()->players[BFEngine::GetInstance()->indexOfPlayer[2]].setTeam(1);
	}

	// Player 4
	cube4 = new Cube();
	//cube4->SetWorldScale(1.0f, 1.5f, 0.1f);
	cube4->SetShader(defaultShaderHandle);
	cube4->renderComponent->SetColour(0.0f, 0.5f, 0.0f);
	cube4->physicsComponent->SetPosition(glm::vec3(-2.0f, 0.0f, -3.0f));
	cube4->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
	cube4->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	cube4->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	cube4->physicsComponent->SetMass(50.0f);

	if (BFEngine::GetInstance()->numPlayers > 3) {
		BFEngine::GetInstance()->players[BFEngine::GetInstance()->indexOfPlayer[3]].playerControls(cube4);
		BFEngine::GetInstance()->players[BFEngine::GetInstance()->indexOfPlayer[3]].setTeam(1);
	}

	// Arena
	floor = new Cube();
	floor->SetShader(defaultShaderHandle);
	floor->renderComponent->SetColour(0.1f, 0.1f, 0.1f);
	floor->physicsComponent->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
	//floor->SetWorldScale(11.0f, 1.0f, 11.0f);
	floor->collisionComponent->SetScale(glm::vec3(11.0f, 1.0f, 11.0f));
	floor->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	floor->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	floor->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	floor->physicsComponent->SetMass(0.0f);

	wall = new Cube();
	wall->SetShader(defaultShaderHandle);
	wall->renderComponent->SetColour(0.1f, 0.1f, 0.1f);
	wall->physicsComponent->SetPosition(glm::vec3(5.5f, 0.0f, 0.0f));
	//wall->SetWorldScale(1.0f, 2.0f, 11.0f);
	wall->collisionComponent->SetScale(glm::vec3(1.0f, 2.0f, 11.0f));
	wall->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	wall->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	wall->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	wall->physicsComponent->SetMass(0.0f);

	wall1 = new Cube();
	wall1->SetShader(defaultShaderHandle);
	wall1->renderComponent->SetColour(0.1f, 0.1f, 0.1f);
	wall1->physicsComponent->SetPosition(glm::vec3(-5.5f, 0.0f, 0.0f));
	//wall1->SetWorldScale(1.0f, 2.0f, 11.0f);
	wall1->collisionComponent->SetScale(glm::vec3(1.0f, 2.0f, 11.0f));
	wall1->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	wall1->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	wall1->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	wall1->physicsComponent->SetMass(0.0f);

	middleWall = new Cube();
	middleWall->SetShader(defaultShaderHandle);
	middleWall->renderComponent->SetColour(1.0f, 0.0f, 0.0f);
	middleWall->physicsComponent->SetPosition(glm::vec3(0.0f, -0.50f, 0.0f));
	//middleWall->SetWorldScale(11.0f, 0.1f, 1.0f);
	middleWall->collisionComponent->SetScale(glm::vec3(11.0f, 0.1f, 1.0f));
	middleWall->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	middleWall->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	middleWall->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::PERFECT_ROUGH);
	middleWall->physicsComponent->SetMass(0.0f);

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

	// UI
	text = new TextUI();
	text->SetText("ALL OUT BRAWLER;");
	text->SetColour(0.0f, 0.0f, 0.0f);
	text->SetSize(1.0f);
	text->SetPosition(sceneManager->GetScreenWidth() / 2 - text->GetLength() / 2, sceneManager->GetScreenHeight() - 100);

	// add objects and lights to their lists
	AddPhysicsObject(cube1);
	AddPhysicsObject(cube2);
	AddPhysicsObject(cube3);
	AddPhysicsObject(cube4);
	AddPhysicsObject(floor);
	AddPhysicsObject(wall);
	AddPhysicsObject(wall1);
	AddPhysicsObject(middleWall);
	AddLightObject(dirLight);
	AddLightObject(pointLight);
	AddUIObject(text);

	PhysicsEngine::GetInstance()->AddObjectList(physicsObjectList);

	return true;
}

void DemoScene::Update(const float deltaTime)
{	
	// Timer for firing (Keyboard controls only)
	timer -= deltaTime;
	if (timer < 0) {
		timer = 0.3f;
		fire = true;
	}
	PhysicsEngine::GetInstance()->AddObjectList(physicsObjectList);
	
	sceneManager->DebugText("Text origin is top left", 25.0f, 25.0f);

	PhysicsEngine::GetInstance()->Update(deltaTime);

	//check for joystick so we're not wasting time
	if (InputHandler::GetInstance()->areJoysticksLive()) {
		//joystick motion update
		//maybe we don't handle it as handleevent

		//handle it as a for loop
		//easy to understand player motion
		for (int i = 0; i < InputHandler::GetInstance()->jCheck(); i++) {

			glm::vec3 mods = InputHandler::GetInstance()->playerMotion(i);

			mods *= BFEngine::GetInstance()->players[i].inverted();
			
			glm::vec3 temp = BFEngine::GetInstance()->players[i].pObject()->physicsComponent->GetVelocity();
			
			BFEngine::GetInstance()->players[i].pObject()->physicsComponent->SetVelocity(temp + glm::vec3(mods.x * moveSpeed * deltaTime, 0.0f, mods.y * moveSpeed * deltaTime));

			if (BFEngine::GetInstance()->players[i].shotDelay == 0) {
				//if (SDL_JoystickGetButton(BFEngine::GetInstance()->players[i].pStick, 1) == 1) {
				if (SDL_JoystickGetAxis(BFEngine::GetInstance()->players[i].pStick, 5) > 1000) {
					Cube* lr = BFEngine::GetInstance()->players[i].pObject(); //line reducer
					Projectile* p = new Projectile(glm::vec3(lr->GetWorldPosition().x, lr->GetWorldPosition().y, lr->GetWorldPosition().z - BFEngine::GetInstance()->players[i].inverted() * lr->collisionComponent->GetBoundingBox().r.z * 2.0f * lr->GetWorldScale().z), BFEngine::GetInstance()->players[i].inverted());
					glm::vec3 tempColor = BFEngine::GetInstance()->players[i].getTeamColor();
					p->renderComponent->SetColour(tempColor.x, tempColor.y, tempColor.z);
					p->SetShader(defaultShaderHandle);
					AddPhysicsObject(p);
					PhysicsEngine::GetInstance()->AddObjectList(physicsObjectList);

					std::cout << "Player " << i << " shot." << std::endl;
					BFEngine::GetInstance()->players[i].weShot();
					//TODO: Animation
				}
			}
					BFEngine::GetInstance()->players[i].tick();
					//std::cout << "Player " << i << " tick thing is " << BFEngine::GetInstance()->players[i].shotDelay << std::endl;
				
			

			//BFEngine::GetInstance()->players[i].tick(); //reduces shot delay
		}

	}
	else {
		//std::cout << "No joystick events today." << std::endl;
		//holy shit this got annoying
	}
}

void DemoScene::HandleEvents(SDL_Event events)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	//joystick
	switch (events.type)
	{
	case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */
		if (events.jaxis.axis == 2) //left trigger
		{
			/* Up-Down movement code goes here */
			std::cout << "Left Trigger" << std::endl;
			//cube->physicsComponent->SetVelocity(glm::vec3(-1.0f, 0.0f, 0.0f));
		}
		if (events.jaxis.axis == 5) //right trigger
		{
			/* Up-Down movement code goes here */
			std::cout << "Right Trigger" << std::endl;
			//cube->physicsComponent->SetVelocity(glm::vec3(1.0f, 0.0f, 0.0f));
		}


		break;

	case SDL_JOYBUTTONDOWN:  /* Handle Joystick Button Presses */
		if (events.jbutton.button == 0) //A button
		{
			/* code goes here */
			std::cout << "A button was probably pressed by " << events.jbutton.which << std::endl;
			//+-BFEngine::GetInstance()->players[events.jbutton.which].pObject()->Jump(glm::vec3(0.0f, 2.0f, 0.0f));
			std::cout << "but it thinks its using joystick " << BFEngine::GetInstance()->players[events.jbutton.which].instanceID << endl;
		}
		if (events.jbutton.button == 1) //B button
		{
			/* code goes here */
			

			//if (BFEngine::GetInstance()->players[events.jbutton.which].shotDelay == 0) {
			/*	
			Cube* lr = BFEngine::GetInstance()->players[events.jbutton.which].pObject(); //line reducer
				Projectile* p = new Projectile(glm::vec3(lr->GetWorldPosition().x, lr->GetWorldPosition().y, lr->GetWorldPosition().z - BFEngine::GetInstance()->players[events.jbutton.which].inverted() * lr->collisionComponent->boundingBox->w * lr->GetWorldScale().z), BFEngine::GetInstance()->players[events.jbutton.which].inverted());
					glm::vec3 tempColor = BFEngine::GetInstance()->players[events.jbutton.which].getTeamColor();
					p->renderComponent->SetColour(tempColor.x, tempColor.y, tempColor.z);
					p->SetShader(defaultShaderHandle);
					AddPhysicsObject(p);
					PhysicsEngine::GetInstance()->AddObjectList(physicsObjectList);

					std::cout << "Player " << BFEngine::GetInstance()->indexOfPlayer[events.jbutton.which] << " shot." << std::endl;
					*/
					//BFEngine::GetInstance()->players[events.jbutton.which].weShot();
			//}
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
			//cube->physicsComponent->SetVelocity(glm::vec3(0.0f, 0.0f, -1.0f));
		}
		if (events.jbutton.button == 5) //right bumper
		{
			/* code goes here */
			std::cout << "right bumper was probably pressed." << std::endl;
			//cube->physicsComponent->SetVelocity(glm::vec3(0.0f, 0.0f, 1.0f));
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
			sceneManager->EnableSplitscreen(true);
			sceneManager->EnableFullscreen(true);
			sceneManager->SwitchScene(new DemoScene());
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
			//BFEngine::GetInstance()->TerminateGame();
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

	// CUBE 1
	// Movement
	if (state[SDL_SCANCODE_SPACE]) {
		if (fire) {
			Projectile* p = new Projectile(glm::vec3(cube1->GetWorldPosition().x, cube1->GetWorldPosition().y, cube1->GetWorldPosition().z - cube1->collisionComponent->GetBoundingBox().r.z * 2.0f * cube1->GetWorldScale().z), 1);
			p->SetShader(defaultShaderHandle);
			AddPhysicsObject(p);
			PhysicsEngine::GetInstance()->AddObjectList(physicsObjectList);
			fire = false;
		}
	}
	if (state[SDL_SCANCODE_W]) {
		//cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(0.0f, 0.0f, -moveSpeed * deltaTime));
		cube1->physicsComponent->SetVelocity(glm::vec3(0.0f, 0.0f, -moveSpeed * deltaTime * 200));
	}
	if (state[SDL_SCANCODE_S]) {
		//cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(0.0f, 0.0f, moveSpeed * deltaTime));
		cube1->physicsComponent->SetVelocity(glm::vec3(0.0f, 0.0f, moveSpeed * deltaTime * 200));
	}
	if (state[SDL_SCANCODE_D]) {
		//cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(moveSpeed * deltaTime, 0.0f, 0.0f));
		cube1->physicsComponent->SetVelocity(glm::vec3(moveSpeed * deltaTime * 200, 0.0f, 0.0f));
	}
	if (state[SDL_SCANCODE_A]) {
		//cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(-moveSpeed * deltaTime, 0.0f, 0.0f));
		cube1->physicsComponent->SetVelocity(glm::vec3(-moveSpeed * deltaTime * 200, 0.0f, 0.0f));
	}

	// Reload Scene
	if (state[SDL_SCANCODE_Z]) {
		sceneManager->EnableSplitscreen(true);
		sceneManager->EnableFullscreen(true);
		sceneManager->SwitchScene(new DemoScene());
	}

	// ENABLE OR DISABLE FULLSCREEN WITH V AND B
	// ENABLE OR DISABLE SPLITSCREEN WITH N AND M
}