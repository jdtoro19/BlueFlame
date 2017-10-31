#include "TestScene.h"

using namespace GAME;

TestScene::TestScene()
{
	startPos = glm::vec3(-1.0f, 1.0f, 3.0f);

	cube = new Cube();
	cube->renderComponent->SetColour(0.31f, 0.5f, 1.0f);
	cube->physicsComponent->SetPosition(glm::vec3(-3.0f, 0.0f, 0.0f));
	cube->SetWorldScale(0.5f, 0.5f, 0.5f);
	cube->physicsComponent->setPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
	cube->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::PERFECT_ELASTIC);
	cube->physicsComponent->SetMass(50.0f);

	cube2 = new Cube();	
	cube2->renderComponent->SetColour(1.0f, 0.5f, 0.31f);
	cube2->physicsComponent->SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
	cube2->physicsComponent->setPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
	cube2->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::PERFECT_ELASTIC);
	cube2->physicsComponent->SetMass(50.0f);

	floor = new Cube();
	floor->renderComponent->SetColour(0.3f, 0.3f, 0.3f);
	floor->physicsComponent->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
	floor->SetWorldScale(7.0f, 1.0f, 7.0f);
	floor->physicsComponent->setPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	floor->physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::NON_ELASTIC);
	floor->physicsComponent->SetMass(0.0f);

	wall = new Cube();
	wall->renderComponent->SetColour(0.3f, 0.3f, 0.3f);
	wall->physicsComponent->SetPosition(glm::vec3(3.5f, 0.0f, 0.0f));
	wall->SetWorldScale(1.0f, 2.0f, 7.0f);
	wall->physicsComponent->setPhysicsType(PhysicsComponent::Physics_Type::STATIC);
	wall->physicsComponent->SetMass(0.0f);

	model = new Model("Resources/Models/miku/miku.obj");
	model->physicsComponent->SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	model->SetWorldScale(0.02f);
	model->physicsComponent->setPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
	model->physicsComponent->SetMass(50.0f);
	//model->SetIsModel(false);

	dirLight = new Light(LightComponent::Light_Type::DIRECTIONAL);

	// SKYBOX
	skybox = new Skybox();
	std::vector<char*> faces;
	faces.push_back("Resources/Textures/Skyboxes/right.jpg");
	faces.push_back("Resources/Textures/Skyboxes/left.jpg");
	faces.push_back("Resources/Textures/Skyboxes/top.jpg");
	faces.push_back("Resources/Textures/Skyboxes/bottom.jpg");
	faces.push_back("Resources/Textures/Skyboxes/back.jpg");
	faces.push_back("Resources/Textures/Skyboxes/front.jpg");
	skybox->LoadTextures(faces);
	BFEngine::GetInstance()->GetSceneManager()->GetRenderer()->SetSkybox(skybox);

	AddObject(cube);
	AddObject(cube2);
	AddObject(floor);
	AddObject(model);
	AddObject(wall);
	AddLightObject(dirLight);
}

TestScene::~TestScene()
{
}

void TestScene::Update(const float deltaTime)
{
	//Code to make the model spin
	//model->SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), model->GetWorldRotationAngle() + moveSpeed * deltaTime);

	this->deltaTime = deltaTime;

	if (objectList.size() != NULL) {
		for (size_t i = 0; i < objectList.size(); ++i) {
			objectList.at(i)->Update(deltaTime);
		}
	}

	if (lightObjectList.size() != NULL) {
		for (size_t i = 0; i < lightObjectList.size(); ++i) {
			lightObjectList.at(i)->Update(deltaTime);
		}
	}

	// Test collisions
	if (Physics::isColliding(cube->collisionComponent, floor->collisionComponent)) {
		Physics::Collide(cube->physicsComponent, cube->collisionComponent, floor->physicsComponent, floor->collisionComponent);
	}
	else {

	}

	if (Physics::isColliding(cube->collisionComponent, model->collisionComponent)) {
		Physics::Collide(cube->physicsComponent, cube->collisionComponent, model->physicsComponent, model->collisionComponent);
	}

	if (Physics::isColliding(cube2->collisionComponent, floor->collisionComponent)) {
		Physics::Collide(cube2->physicsComponent, cube2->collisionComponent, floor->physicsComponent, floor->collisionComponent);
	}
	else {

	}

	if (Physics::isColliding(cube->collisionComponent, cube2->collisionComponent)) {
		Physics::Collide(cube->physicsComponent, cube->collisionComponent, cube2->physicsComponent, cube2->collisionComponent);
	}
	else {

	}

	if (Physics::isColliding(model->collisionComponent, floor->collisionComponent)) {
		Physics::Collide(model->physicsComponent, model->collisionComponent, floor->physicsComponent, floor->collisionComponent);
	}

	if (Physics::isColliding(model->collisionComponent, cube2->collisionComponent)) {
		Physics::Collide(model->physicsComponent, model->collisionComponent, cube2->physicsComponent, cube2->collisionComponent);
	}

	if (Physics::isColliding(model->collisionComponent, wall->collisionComponent)) {
		Physics::Collide(model->physicsComponent, model->collisionComponent, wall->physicsComponent, wall->collisionComponent);
	}

	if (Physics::isColliding(cube->collisionComponent, wall->collisionComponent)) {
		Physics::Collide(cube->physicsComponent, cube->collisionComponent, wall->physicsComponent, wall->collisionComponent);
	}

	if (Physics::isColliding(cube2->collisionComponent, wall->collisionComponent)) {
		Physics::Collide(cube2->physicsComponent, cube2->collisionComponent, wall->physicsComponent, wall->collisionComponent);
	}
}

void TestScene::Render() 
{
	// Rendering handled by SceneManager.....for now...
}

void TestScene::HandleEvents(SDL_Event events)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);

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

	// Model
	// Movement
	if (state[SDL_SCANCODE_I]) {
		model->SetWorldPosition(model->GetWorldPosition() + glm::vec3(0.0f, 0.0f, -moveSpeed * deltaTime));
	}
	if (state[SDL_SCANCODE_K]) {
		model->SetWorldPosition(model->GetWorldPosition() + glm::vec3(0.0f, 0.0f, moveSpeed * deltaTime));
	}
	if (state[SDL_SCANCODE_L]) {
		model->SetWorldPosition(model->GetWorldPosition() + glm::vec3(moveSpeed * deltaTime, 0.0f, 0.0f));
	}
	if (state[SDL_SCANCODE_J]) {
		model->SetWorldPosition(model->GetWorldPosition() + glm::vec3(-moveSpeed * deltaTime, 0.0f, 0.0f));
	}
	// Rotation
	if (state[SDL_SCANCODE_O]) {
		model->SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), model->GetWorldRotationAngle() - moveSpeed * deltaTime);
	}
	if (state[SDL_SCANCODE_U]) {
		model->SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), model->GetWorldRotationAngle() + moveSpeed * deltaTime);
	}

	// Reload Scene
	if (state[SDL_SCANCODE_Z]) {
		BFEngine::GetInstance()->GetSceneManager()->SwitchScene(new TestScene());
	}
}

