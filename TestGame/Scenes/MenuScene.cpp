#include "MenuScene.h"

using namespace GAME;

MenuScene::MenuScene()
{
}

MenuScene::~MenuScene()
{
	sceneManager = nullptr;
}

void MenuScene::Initialize()
{
	// Make reference to the scene manager
	sceneManager = BFEngine::GetInstance()->GetSceneManager();

	// Set camera options
	cameraList[0]->Yaw = 0.0f;
	cameraList[0]->Zoom = 105.0f;

	// Set screen options
	sceneManager->EnableFullscreen(true);
	sceneManager->ShowFPS(true);

	// Load shaders
	skyboxShader = new Shader("Shaders/skybox.vs", "Shaders/skybox.fs");

	// Put shaders into shader manager
	skyboxShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("skybox"), skyboxShader);

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
	titleText = new TextUI();
	titleText->SetFont("Resources/Fonts/ka1.ttf");
	titleText->SetText("ALL OUT BRAWLER");
	titleText->SetColour(0.0, 0.0f, 0.0f);
	titleText->SetSize(1.3f);
	titleText->SetSpacing(9.0f);
	titleText->SetPosition(sceneManager->GetScreenWidth() / 2 - titleText->GetLength() / 2, sceneManager->GetScreenHeight() / 2);

	startText = new TextUI();
	startText->SetText("Press Start/Space to play");
	startText->SetColour(0.6f, 0.6f, 0.6f);
	startText->SetSize(0.30f);
	startText->SetSpacing(50.0f);
	startText->SetPosition(sceneManager->GetScreenWidth() / 2 - startText->GetLength() / 2, titleText->GetPosition().y + 300.0f);

	creditText = new TextUI();
	creditText->SetText("(C) 20XX STUDIOS");
	creditText->SetColour(0.0f, 0.0f, 0.0f);
	creditText->SetSize(0.30f);
	creditText->SetPosition(sceneManager->GetScreenWidth() - creditText->GetLength() - 15.0f, sceneManager->GetScreenHeight() - 15.0f);
	
	AddUIObject(titleText);
	AddUIObject(startText);
	AddUIObject(creditText);	
}

void MenuScene::Update(const float deltaTime)
{
	float fade = (sin(z+=deltaTime) / 2.0f) + 0.5f;
	startText->SetAlpha(fade);
	cameraList[0]->SetRotationY(cameraList[0]->Yaw += deltaTime * 5);	
}

void MenuScene::HandleEvents(SDL_Event events)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_SPACE]) {
		sceneManager->NextScene();
	}

	if (events.jbutton.button == 7) //start button
	{
		sceneManager->NextScene();
	}
}

void MenuScene::Render()
{

}

void MenuScene::Draw()
{

}