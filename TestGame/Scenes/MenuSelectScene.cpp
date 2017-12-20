#include "MenuSelectScene.h"

using namespace GAME;

MenuSelectScene::MenuSelectScene()
{
}

MenuSelectScene::~MenuSelectScene()
{
	sceneManager = nullptr;
}

void MenuSelectScene::Initialize()
{
	// Make reference to the scene manager
	sceneManager = BFEngine::GetInstance()->GetSceneManager();
	
	// If you plan to use the scenemanagers scene vector you dont need to set these values all the time
	/*
	// Set camera options
	cameraList[0]->Yaw = 0.0f;
	cameraList[0]->Zoom = 105.0f;

	// Set screen options
	sceneManager->EnableFullscreen(true);
	sceneManager->ShowFPS(true);
	*/
	// Load shaders
	skyboxShader = new Shader("Shaders/skybox.vs", "Shaders/skybox.fs");

	// Put shaders into shader manager
	skyboxShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("skybox"), skyboxShader);

	// Make skybox, load its textures, set properties, and give to the renderer
	skybox = new Skybox();
	std::vector<char*> faces;
	faces.push_back("Resources/Textures/Skyboxes/ame_nebula/right.jpg");
	faces.push_back("Resources/Textures/Skyboxes/ame_nebula/left.jpg");
	faces.push_back("Resources/Textures/Skyboxes/ame_nebula/top.jpg");
	faces.push_back("Resources/Textures/Skyboxes/ame_nebula/bottom.jpg");
	faces.push_back("Resources/Textures/Skyboxes/ame_nebula/back.jpg");
	faces.push_back("Resources/Textures/Skyboxes/ame_nebula/front.jpg");
	skybox->LoadTextures(faces);
	skybox->SetShader(skyboxShaderHandle);

	// UI
	titleText = new TextUI();
	titleText->SetFont("Resources/Fonts/ka1.ttf");
	titleText->SetText("ALL OUT BRAWLER");
	titleText->SetColour(1.0, 1.0f, 1.0f);
	titleText->SetSize(0.8f);
	titleText->SetSpacing(9.0f);
	titleText->SetPosition(50, -100);

	button = new ButtonUI("Resources/Textures/DefaultButton.png");
	button->SetPosition(-(button->GetWidth() / 2) - 150, (button->GetHeight() / 2) + 650);
	button->SetHeight(100);
	button->SetPadding(-10, 0);
	button->GetText()->SetText("PLAY");
	button->SetTextPadding(75, 35);
	button->GetImage()->SetAlpha(0.5f);

	buttonExit = new ButtonUI("Resources/Textures/lewd.png");
	buttonExit->SetPosition(-(button->GetWidth() / 2) - 150, (button->GetHeight() / 2) + button->GetPosition().y + 100);
	buttonExit->SetHeight(100);
	buttonExit->GetText()->SetText("EXIT");
	buttonExit->GetText()->SetColour(0.0f, 0.0f, 0.0f);
	buttonExit->SetTextPadding(75, 35);
	buttonExit->GetImage()->SetAlpha(0.5f);

	AddUIObject(titleText);
	AddUIObject(button);
	AddUIObject(buttonExit);
}

void MenuSelectScene::Update(const float deltaTime)
{
	cameraList[0]->SetRotationY(cameraList[0]->Yaw += deltaTime * 5);	

	float lastPosition = button->GetPosition().x;
	button->SetPosition(lastPosition += deltaTime * 3000, button->GetPosition().y);
	if (button->GetPosition().x > (button->GetWidth() / 2) + 150) {
		button->SetPosition((button->GetWidth() / 2) + 150, button->GetPosition().y);
	}

	float lastPosition2 = buttonExit->GetPosition().x;
	buttonExit->SetPosition(lastPosition += deltaTime * 3000, buttonExit->GetPosition().y);
	if (buttonExit->GetPosition().x > (buttonExit->GetWidth() / 2) + 150) {
		buttonExit->SetPosition((buttonExit->GetWidth() / 2) + 150, buttonExit->GetPosition().y);
	}

	float lastPosition3 = titleText->GetPosition().y;
	titleText->SetPosition(50, lastPosition3 += deltaTime * 2000);
	if (titleText->GetPosition().y > 100) {
		titleText->SetPosition(50, 100);
	}
}

void MenuSelectScene::HandleEvents(SDL_Event events)
{
	if (button->OnHover(events, sceneManager))
	{
		button->SetSelected(true);
		button->GetImage()->SetScale(1.2f);
		button->GetImage()->SetAlpha(1.0f);
		if (events.type == SDL_MOUSEBUTTONDOWN) {
			if (events.button.button == SDL_BUTTON_LEFT)
			{
				button->GetImage()->SetScale(1.1f);
			}
		}
		if (events.type == SDL_MOUSEBUTTONUP) {
			if (events.button.button == SDL_BUTTON_LEFT)
			{
				if (button->GetSelected()) {
					sceneManager->ClearSceneList();
					sceneManager->SwitchScene(new DemoScene());
				}
			}
		}
	}
	else
	{
		button->SetSelected(false);
		button->GetImage()->SetScale(1);
		button->GetImage()->SetAlpha(0.5f);
	}

	if (buttonExit->OnHover(events, sceneManager))
	{
		buttonExit->SetSelected(true);
		buttonExit->GetImage()->SetScale(1.2f);
		buttonExit->GetImage()->SetAlpha(1.0f);
		if (events.type == SDL_MOUSEBUTTONDOWN) {
			if (events.button.button == SDL_BUTTON_LEFT)
			{
				buttonExit->GetImage()->SetScale(1.1f);
			}
		}
		if (events.type == SDL_MOUSEBUTTONUP) {
			if (events.button.button == SDL_BUTTON_LEFT)
			{
				if (buttonExit->GetSelected()) {
					sceneManager->SetIsQuit(true);
				}
			}
		}
	}
	else
	{
		buttonExit->SetSelected(false);
		buttonExit->GetImage()->SetScale(1);
		buttonExit->GetImage()->SetAlpha(0.5f);
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_Z]) {
		sceneManager->PreviousScene();
	}
}

void MenuSelectScene::Render()
{

}

void MenuSelectScene::Draw()
{

}