#include "GameManager.h"

using namespace GAME;

GameManager::GameManager()
{

}

GameManager::~GameManager()
{

}

void GameManager::Initialize()
{
	roundTimer = Cooldown(60);
	roundTimer.startCD();

	// UI
	roundTimerText = new TextUI();
	//roundTimerText->SetFont("Resources/Fonts/ka1.ttf");
	roundTimerText->SetText("Ready?");
	roundTimerText->SetColour(1.0f, 1.0f, 1.0f);
	roundTimerText->SetSize(1.0f);
	roundTimerText->SetSpacing(9.0f);
	roundTimerText->SetPosition((BFEngine::GetInstance()->GetSceneManager()->GetScreenWidth() / 2 - 60.0f), 90.0f);

	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddUIObject(roundTimerText);
}

void GameManager::Update()
{
	roundTimerText->SetText(std::to_string((int)roundTimer.secondsLeft()));
}