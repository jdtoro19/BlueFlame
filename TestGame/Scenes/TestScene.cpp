#include "TestScene.h"

using namespace GAME;

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
	cube = nullptr;
	cube2 = nullptr;
	floor = nullptr;
	wall = nullptr;
	model = nullptr;
	delete cube;
	delete cube2;
	delete floor;
	delete wall;
	delete model;
}

void TestScene::Initialize() 
{
	// Make reference to the scene manager
	sceneManager = BFEngine::GetInstance()->GetSceneManager();

	// Set the position of the first camera
	cameraList[0]->Position = glm::vec3(0.0f, 1.0f, 3.0f);
	// Set the position for the second camera
	cameraList[1]->Position = glm::vec3(0.0f, 2.0f, 4.0f);

	// Load shaders
	defaultShader = new Shader("Shaders/default.vs", "Shaders/default.fs");
	modelShader = new Shader("Shaders/model.vs", "Shaders/model.fs");
	skyboxShader = new Shader("Shaders/skybox.vs", "Shaders/skybox.fs");
	lightShader = new Shader("Shaders/lamp.vs", "Shaders/lamp.fs");

	// Put shaders into shader manager
	defaultShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("cube"), defaultShader);
	modelShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("model"), modelShader);
	skyboxShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("skybox"), skyboxShader);
	lightShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("light"), lightShader);

	// Lights
	//
	// Initialize
	blueLight = new Light(LightComponent::Light_Type::POINTLIGHT);
	// Set which shader this object is using
	blueLight->SetShader(lightShaderHandle);
	// Set type and colour of object's mesh
	blueLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	blueLight->renderComponent->SetColour(0.0f, 0.0f, 1.0f);
	// Set position and scale of object
	blueLight->SetWorldPosition(-2.0f, 2.0f, 0.0f);
	blueLight->SetWorldScale(0.5f);
	// Set what type of light and what colour this object emits
	blueLight->lightComponent->SetColour(glm::vec3(0.0f, 0.0f, 1.0f));
	//
	
	redLight = new Light();
	redLight->SetShader(lightShaderHandle);
	redLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	redLight->renderComponent->SetColour(1.0f, 0.0f, 0.0f);
	redLight->SetWorldPosition(-2.0f, 0.0f, 0.0f);
	redLight->SetWorldScale(0.5f);
	redLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	redLight->lightComponent->SetColour(glm::vec3(1.0f, 0.0f, 0.0f));

	greenLight = new Light();
	greenLight->SetShader(lightShaderHandle);
	greenLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	greenLight->renderComponent->SetColour(0.0f, 1.0f, 0.0f);
	greenLight->SetWorldPosition(2.0f, 0.0f, 0.0f);
	greenLight->SetWorldScale(0.5f);
	greenLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	greenLight->lightComponent->SetColour(glm::vec3(0.0f, 1.0f, 0.0f));

	yellowLight = new Light();
	yellowLight->SetShader(lightShaderHandle);
	yellowLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	yellowLight->renderComponent->SetColour(1.0f, 1.0f, 0.0f);
	yellowLight->SetWorldPosition(2.0f, 2.0f, 0.0f);
	yellowLight->SetWorldScale(0.5f);
	yellowLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	yellowLight->lightComponent->SetColour(glm::vec3(1.0f, 1.0f, 0.0f));

	// Make directional light
	dirLight = new Light(LightComponent::Light_Type::DIRECTIONAL);
	dirLight->lightComponent->SetDirection(glm::vec3(-0.5f, 0.0f, -0.8f));

	// Objects
	cube = new Cube();
	cube->SetShader(defaultShaderHandle);
	cube->renderComponent->SetColour(0.0f, 0.0f, 1.0f);
	cube->physicsComponent->SetPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
	cube->SetWorldScale(0.5f, 0.5f, 0.5f);
	cube->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
	cube->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::PERFECT_ELASTIC);
	cube->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::PERFECT_SMOOTH);
	cube->physicsComponent->SetMass(50.0f);

	cube2 = new Cube();
	cube2->SetShader(defaultShaderHandle);
	cube2->renderComponent->SetColour(1.0f, 0.0f, 0.0f);
	cube2->physicsComponent->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	cube2->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
	cube2->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::PERFECT_ELASTIC);
	cube2->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::PERFECT_SMOOTH);
	cube2->physicsComponent->SetMass(50.0f);

	floor = new Cube();
	floor->SetShader(defaultShaderHandle);
	floor->renderComponent->SetColour(0.0f, 0.0f, 0.0f);
	floor->physicsComponent->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
	floor->SetWorldScale(7.0f, 1.0f, 7.0f);
	floor->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	floor->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	floor->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::PERFECT_SMOOTH);
	floor->physicsComponent->SetMass(0.0f);

	wall = new Cube();
	wall->SetShader(defaultShaderHandle);
	wall->renderComponent->SetColour(0.0f, 0.0f, 0.0f);
	wall->physicsComponent->SetPosition(glm::vec3(3.5f, 0.0f, 0.0f));
	wall->SetWorldScale(1.0f, 2.0f, 7.0f);
	wall->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	wall->physicsComponent->SetMass(0.0f);

	model = new Model("Resources/Models/miku/miku.obj");
	model->SetShader(modelShaderHandle);
	model->physicsComponent->SetPosition(glm::vec3(1.0f, 1.0f, 0.0f));
	model->SetWorldScale(0.02f);
	///model->SetWorldScale(0.5f);
    ///model->SetWorldRotation(glm::vec3(1.0f, 0.0f, 0.0f), 80.0f);
	model->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
	model->physicsComponent->SetMass(50.0f);

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
	sceneManager->GetRenderer()->SetSkybox(skybox);

	// add objects and lights to their lists
	AddObject(cube);
	AddObject(cube2);
	AddObject(floor);
	AddObject(model);
	AddObject(wall);
	AddLightObject(dirLight);
	AddLightObject(blueLight);
	AddLightObject(redLight);
	AddLightObject(greenLight);
	AddLightObject(yellowLight);
}

void TestScene::Update(const float deltaTime)
{
	//Code to make the model spin
	//model->SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), model->GetWorldRotationAngle() + moveSpeed * deltaTime);

	this->deltaTime = deltaTime;

	// Update objects
	// lights are included in this object list
	if (objectList.size() != NULL) {
		for (size_t i = 0; i < objectList.size(); ++i) {
			objectList.at(i)->Update(deltaTime);
		}
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

			if (i == 0) {
				if (InputHandler::GetInstance()->upDown) {
					modifierY *= -1; //y axis is inverted in up down thats why we're taking negative values
					cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(modifierX * moveSpeed * deltaTime, modifierY * moveSpeed * deltaTime, 0.0f));
				}
				else {
					cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(modifierX * moveSpeed * deltaTime, 0.0f, modifierY * moveSpeed * deltaTime));
				}
			}
			else if (i == 1) {
				if (InputHandler::GetInstance()->upDown) {
					modifierY *= -1; //y axis is inverted in up down thats why we're taking negative values
					cube2->SetWorldPosition(cube2->GetWorldPosition() + glm::vec3(modifierX * moveSpeed * deltaTime, modifierY * moveSpeed * deltaTime, 0.0f));
				}
				else {
					cube2->SetWorldPosition(cube2->GetWorldPosition() + glm::vec3(modifierX * moveSpeed * deltaTime, 0.0f, modifierY * moveSpeed * deltaTime));
				}
			}


		}

	}
	else {
		//std::cout << "No joystick events today." << std::endl;
	}
}

void TestScene::Render() 
{
	// Rendering handled by SceneManager.....for now...
}

void TestScene::Draw() 
{
	// Used to draw 2d objects or UI elements
}

void TestScene::HandleEvents(SDL_Event events)
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
			cube->physicsComponent->SetVelocity(glm::vec3(-1.0f, 0.0f, 0.0f));
		}
		if (events.jaxis.axis == 5) //right trigger
		{
			/* Up-Down movement code goes here */
			std::cout << "Right Trigger" << std::endl;
			cube->physicsComponent->SetVelocity(glm::vec3(1.0f, 0.0f, 0.0f));
		}


		break;

	case SDL_JOYBUTTONDOWN:  /* Handle Joystick Button Presses */
		if (events.jbutton.button == 0) //A button
		{
			/* code goes here */
			std::cout << "A button was probably pressed." << std::endl;
			cube->Jump(glm::vec3(0.0f, 2.0f, 0.0f));
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
			cube->physicsComponent->SetVelocity(glm::vec3(0.0f, 0.0f, -1.0f));
		}
		if (events.jbutton.button == 5) //right bumper
		{
			/* code goes here */
			std::cout << "right bumper was probably pressed." << std::endl;
			cube->physicsComponent->SetVelocity(glm::vec3(0.0f, 0.0f, 1.0f));
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

	// CUBE 1
	// Movement
	if (state[SDL_SCANCODE_SPACE]) {
		cube->Jump(glm::vec3(0.0f, 2.0f, 0.0f));
	}
	if (state[SDL_SCANCODE_W]) {
		//cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(0.0f, 0.0f, -moveSpeed * deltaTime));
		cube->physicsComponent->SetVelocity(glm::vec3(0.0f, 0.0f, -1.0f));
	}
	if (state[SDL_SCANCODE_S]) {
		//cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(0.0f, 0.0f, moveSpeed * deltaTime));
		cube->physicsComponent->SetVelocity(glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (state[SDL_SCANCODE_D]) {
		//cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(moveSpeed * deltaTime, 0.0f, 0.0f));
		cube->physicsComponent->SetVelocity(glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (state[SDL_SCANCODE_A]) {
		//cube->SetWorldPosition(cube->GetWorldPosition() + glm::vec3(-moveSpeed * deltaTime, 0.0f, 0.0f));
		cube->physicsComponent->SetVelocity(glm::vec3(-1.0f, 0.0f, 0.0f));
	}
	// Rotation
	if (state[SDL_SCANCODE_E]) {
		cube->SetWorldRotation(glm::vec3(0.0f, 0.0f, 1.0f), cube->GetWorldRotationAngle() - moveSpeed * deltaTime);
	}
	if (state[SDL_SCANCODE_Q]) {
		cube->SetWorldRotation(glm::vec3(0.0f, 0.0f, 1.0f), cube->GetWorldRotationAngle() + moveSpeed * deltaTime);
	}

	// Light
	// Movement
	if (state[SDL_SCANCODE_I]) {
		blueLight->SetWorldPosition(blueLight->GetWorldPosition() + glm::vec3(0.0f, moveSpeed * deltaTime, 0.0f));
	}
	if (state[SDL_SCANCODE_K]) {
		blueLight->SetWorldPosition(blueLight->GetWorldPosition() + glm::vec3(0.0f, -moveSpeed * deltaTime, 0.0f));
	}
	if (state[SDL_SCANCODE_L]) {
		blueLight->SetWorldPosition(blueLight->GetWorldPosition() + glm::vec3(moveSpeed * deltaTime, 0.0f, 0.0f));
	}
	if (state[SDL_SCANCODE_J]) {
		blueLight->SetWorldPosition(blueLight->GetWorldPosition() + glm::vec3(-moveSpeed * deltaTime, 0.0f, 0.0f));
	}
	// Rotation
	if (state[SDL_SCANCODE_O]) {
		blueLight->SetWorldRotation(glm::vec3(0.0f, 0.0f, 1.0f), blueLight->GetWorldRotationAngle() - moveSpeed * deltaTime);
	}
	if (state[SDL_SCANCODE_U]) {
		blueLight->SetWorldRotation(glm::vec3(0.0f, 0.0f, 1.0f), blueLight->GetWorldRotationAngle() + moveSpeed * deltaTime);
	}

	// VIEW/CAMERA
	if (state[SDL_SCANCODE_T]) {
		cameraList[0]->ProcessKeyboard(FORWARD, 0.01f);
	}
	if (state[SDL_SCANCODE_G]) {
		cameraList[0]->ProcessKeyboard(BACKWARD, 0.01f);
	}
	if (state[SDL_SCANCODE_H]) {
		cameraList[0]->ProcessKeyboard(RIGHT, 0.01f);
	}
	if (state[SDL_SCANCODE_F]) {
		cameraList[0]->ProcessKeyboard(LEFT, 0.01f);
	}

	// Reload Scene
	if (state[SDL_SCANCODE_Z]) {
		sceneManager->EnableSplitscreen(false);
		sceneManager->SwitchScene(new TestScene());
	}

	// Reload Scene
	if (state[SDL_SCANCODE_B]) {
		Cube* c = new Cube();
		c->SetShader(defaultShaderHandle);
		c->renderComponent->SetColour(0.0f, 0.0f, 1.0f);
		c->physicsComponent->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
		c->SetWorldScale(0.5f, 0.5f, 0.5f);
		c->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
		c->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::PERFECT_ELASTIC);
		c->physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::PERFECT_SMOOTH);
		c->physicsComponent->SetMass(50.0f);
		AddObject(c);
	}

	// Camera look
	if (events.type == SDL_MOUSEMOTION) {
		cameraList[0]->ProcessMouseMovement((float)events.motion.x, (float)events.motion.y);
	}

	if (events.type == SDL_MOUSEWHEEL) {
		cameraList[0]->ProcessMouseScroll((float)events.wheel.y);
	}
}