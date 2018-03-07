#include "TvTGameScene.h"

using namespace GAME;

TvTGameScene::TvTGameScene()
{
}

TvTGameScene::~TvTGameScene()
{
}

bool TvTGameScene::Initialize()
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
	cameraList[0]->SetRotationX(-35.0f);
	// Set the options for the second camera
	cameraList[1]->Position = glm::vec3(0.0f, 6.0f, -10.0f);
	cameraList[1]->SetRotationY(90.0f);
	cameraList[1]->SetRotationX(-35.0f);

	// Load shaders
	defaultShader = new Shader("Shaders/model.vs", "Shaders/model.fs");
	skyboxShader = new Shader("Shaders/skybox.vs", "Shaders/skybox.fs");
	lightShader = new Shader("Shaders/lamp.vs", "Shaders/lamp.fs");

	// Put shaders into shader manager
	defaultShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("default"), defaultShader);
	skyboxShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("skybox"), skyboxShader);
	lightShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("light"), lightShader);

	// Lights
	//
	// Make point light
	//
	pointLight = new Light();
	pointLight->SetShader(lightShaderHandle);
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
	blueLight->SetWorldPosition(0.0f, -2.99f, 5.0f);
	blueLight->SetWorldScale(11.0f, 5.0f, 0.5f);
	blueLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	blueLight->lightComponent->SetColour(glm::vec3(0.0f, 0.0f, 1.0f));
	//	

	// 
	redLight = new Light();
	redLight->SetShader(lightShaderHandle);
	redLight->renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	redLight->renderComponent->SetColour(1.0f, 0.0f, 0.0f);
	redLight->SetWorldPosition(0.0f, -2.99f, -5.0f);
	redLight->SetWorldScale(11.0f, 5.0f, 0.5f);
	redLight->lightComponent->SetLightType(LightComponent::Light_Type::POINTLIGHT);
	redLight->lightComponent->SetColour(glm::vec3(1.0f, 0.0f, 0.0f));
	//	

	// Make directional light
	dirLight = new Light(LightComponent::Light_Type::DIRECTIONAL);
	dirLight->lightComponent->SetDirection(glm::vec3(1.0f, -1.0f, 1.0f));
	//

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
	//

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

	// Add scene objects
	AddObject(platform);
	AddObject(topRing);
	AddObject(middleRing);
	AddObject(bottomRing);
	AddLightObject(dirLight);
	AddLightObject(pointLight);
	AddLightObject(pointLight2);
	AddLightObject(pointLight3);
	AddLightObject(pointLight4);
	AddLightObject(blueLight);
	AddLightObject(redLight);

	return true;
}
void TvTGameScene::Update(const float deltaTime)
{
	this->deltaTime = deltaTime;
}
void TvTGameScene::FixedUpdate(const float deltaTime)
{
	
}
void TvTGameScene::HandleEvents(SDL_Event events)
{
	// Camera look
	if (events.type == SDL_MOUSEMOTION) {
		cameraList[0]->ProcessMouseMovement((float)events.motion.x, (float)events.motion.y);
	}
}

void TvTGameScene::HandleStates(const Uint8 *state)
{
	// Camera move
	if (state[SDL_SCANCODE_W]) {
		cameraList[0]->ProcessKeyboard(FORWARD, deltaTime);
	}
	if (state[SDL_SCANCODE_S]) {
		cameraList[0]->ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (state[SDL_SCANCODE_D]) {
		cameraList[0]->ProcessKeyboard(RIGHT, deltaTime);
	}
	if (state[SDL_SCANCODE_A]) {
		cameraList[0]->ProcessKeyboard(LEFT, deltaTime);
	}

	if (state[SDL_SCANCODE_Z]) {
		// Set the options for the first camera
		cameraList[0]->Position = glm::vec3(0.0f, 6.0f, 10.0f);
		cameraList[0]->SetRotationX(-35.0f);
		cameraList[0]->SetRotationY(-90.0f);
	}
}