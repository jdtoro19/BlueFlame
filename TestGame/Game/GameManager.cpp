#include "GameManager.h"

using namespace GAME;

GameManager::GameManager(std::vector<Player*> players)
{
	playerList = players;
}

GameManager::~GameManager()
{

}

void GameManager::Initialize()
{
	outTeam1 = 0;
	outTeam2 = 0;

	gameOver = false;

	matchTime = 90;

	roundTimer = Cooldown(matchTime);

	// UI
	roundTimerText = new TextUI();
	roundTimerText->SetFont("Resources/Fonts/ka1.ttf");
	roundTimerText->SetText(std::to_string(matchTime));
	roundTimerText->SetColour(1.0f, 1.0f, 1.0f);
	roundTimerText->SetSize(1.0f);
	roundTimerText->SetSpacing(9.0f);
	roundTimerText->SetPosition((BFEngine::GetInstance()->GetSceneManager()->GetScreenWidth() / 2 - 95.0f), 80.0f); // 60
	roundTimerText->SetVisible(false);

	// UI
	team1Text = new TextUI();
	team1Text->SetFont("Resources/Fonts/ka1.ttf");
	team1Text->SetText("Winner");
	team1Text->SetColour(1.0f, 1.0f, 1.0f);
	team1Text->SetSize(1.0f);
	team1Text->SetSpacing(9.0f);
	team1Text->SetPosition((team1Text->GetLength() / 2) - 50, (BFEngine::GetInstance()->GetSceneManager()->GetScreenHeight() / 2) - 200);
	team1Text->SetVisible(false);

	team2Text = new TextUI();
	team2Text->SetFont("Resources/Fonts/ka1.ttf");
	team2Text->SetText("Loser");
	team2Text->SetColour(1.0f, 1.0f, 1.0f);
	team2Text->SetSize(1.0f);
	team2Text->SetSpacing(9.0f);
	team2Text->SetPosition(((team1Text->GetLength() / 2) - 50) + (BFEngine::GetInstance()->GetSceneManager()->GetScreenWidth() / 2), (BFEngine::GetInstance()->GetSceneManager()->GetScreenHeight() / 2) - 200);
	team2Text->SetVisible(false);
	
	p1Meter = new SliderUI("Resources/Textures/Green.jpg", "Resources/Textures/blackFILL.png");
	p1Meter->SetPosition(250, 1000);
	p1Meter->SetHeight(100);
	p1Meter->SetValue(0.0f);
	p1Meter->SetVisible(false);

	p2Meter = new SliderUI("Resources/Textures/Green.jpg", "Resources/Textures/blackFILL.png");
	p2Meter->SetPosition(700, 1000);
	p2Meter->SetHeight(100);
	p2Meter->SetValue(0.0f);
	p2Meter->SetVisible(false);

	p3Meter = new SliderUI("Resources/Textures/Green.jpg", "Resources/Textures/blackFILL.png");
	p3Meter->SetPosition(1920 - 700, 1000);
	p3Meter->SetHeight(100);
	p3Meter->SetValue(0.0f);
	p3Meter->SetVisible(false);

	p4Meter = new SliderUI("Resources/Textures/Green.jpg", "Resources/Textures/blackFILL.png");
	p4Meter->SetPosition(1920 - 250, 1000);
	p4Meter->SetHeight(100);
	p4Meter->SetValue(0.0f);
	p4Meter->SetVisible(false);

	divider = new ImageUI;
	divider->SetImage("Resources/Textures/divider.png");
	divider->SetPosition(960.0f, 540.0f);
	divider->SetScale(1.0f);
	divider->SetVisible(false);

	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddUIObject(divider);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddUIObject(roundTimerText);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddUIObject(team1Text);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddUIObject(team2Text);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddUIObject(p1Meter);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddUIObject(p2Meter);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddUIObject(p3Meter);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddUIObject(p4Meter);
}

void GameManager::Update()
{
	p1Meter->SetValue(playerList[0]->GetSpecialMeter());
	p2Meter->SetValue(playerList[1]->GetSpecialMeter());
	p3Meter->SetValue(playerList[2]->GetSpecialMeter());
	p4Meter->SetValue(playerList[3]->GetSpecialMeter());


	time = std::to_string((int)roundTimer.secondsLeft() + 1);

	if (time.size() == 1) {
		time = "0" + time;
	}

	if (!roundTimer.checkOffCD()) {
		roundTimerText->SetText(time);
	}

	if (!gameOver) {

		for (unsigned int i = 0; i < playerList.size(); ++i) {
			if (playerList[i]->GetWorldPosition().y < -10) {
				playerList[i]->SetIsOut(true);
				playerList[i]->canRender = false;
				if (playerList[i]->GetIsTargeting()) {
					playerList[i]->EnableTarget();
				}
				if (playerList[i]->GetPlayerTeam() == Player::PLAYERTEAM::TEAM1) {
					outTeam1++;
				}
				if (playerList[i]->GetPlayerTeam() == Player::PLAYERTEAM::TEAM2) {
					outTeam2++;
				}
			}

			if (playerList[i]->IsOut()) {
				playerList[i]->SetWorldPosition(0.0f, -5.0f, 0.0f);
			}
		}

		if (outTeam1 == 2 || outTeam2 == 2) {
			gameOver = true;
		}
	}

	if (time == "00") {
		gameOver = true;
	}

	if (gameOver) {
		GameOver();
	}
}

void GameManager::StartMatch() 
{
	roundTimerText->SetVisible(true);

	p1Meter->SetVisible(true);
	p2Meter->SetVisible(true);
	p3Meter->SetVisible(true);
	p4Meter->SetVisible(true);

	divider->SetVisible(true);
}

void GameManager::StartTimer()
{
	roundTimer.startCD();

	for (unsigned int i = 0; i < playerList.size(); ++i) {
		playerList[i]->SetCanMove(true);
	}
}

void GameManager::GameOver()
{
	roundTimer.refeshCD();
	roundTimerText->SetVisible(false);

	p1Meter->SetVisible(false);
	p2Meter->SetVisible(false);
	p3Meter->SetVisible(false);
	p4Meter->SetVisible(false);

	for (unsigned int i = 0; i < playerList.size(); ++i) {
		playerList[i]->SetIsOut(true);
		playerList[i]->canRender = true;
		if (playerList[i]->GetIsTargeting()) {
			playerList[i]->EnableTarget();
		}
		if (playerList[i]->GetPlayerTeam() == Player::PLAYERTEAM::TEAM1) {
			team1List.push_back(playerList[i]);
		}
		if (playerList[i]->GetPlayerTeam() == Player::PLAYERTEAM::TEAM2) {
			team2List.push_back(playerList[i]);
		}
	}

	team1List[0]->SetWorldPosition(1.0f, 0.0f, 4.0f);
	team1List[1]->SetWorldPosition(-1.0f, 0.0f, 4.0f);

	team2List[0]->SetWorldPosition(-1.0f, 0.0f, -4.0f);
	team2List[1]->SetWorldPosition(1.0f, 0.0f, -4.0f);

	if (outTeam2 == 2) {
		team1Text->SetText("Winner");
		team1Text->SetColour(glm::vec3(0.0f, 0.0f, 1.0f));
		team1Text->SetVisible(true);

		team2Text->SetText("Loser");
		team2Text->SetColour(glm::vec3(1.0f, 0.0f, 0.0f));
		team2Text->SetVisible(true);
	}
	else if (outTeam1 == 2) {
		team2Text->SetText("Winner");
		team2Text->SetColour(glm::vec3(0.0f, 0.0f, 1.0f));
		team2Text->SetVisible(true);

		team1Text->SetText("Loser");
		team1Text->SetColour(glm::vec3(1.0f, 0.0f, 0.0f));
		team1Text->SetVisible(true);
	}
	else {
		team2Text->SetText("Tie");
		team2Text->SetColour(glm::vec3(1.0f, 0.0f, 1.0f));
		team2Text->SetVisible(true);

		team1Text->SetText("Tie");
		team1Text->SetColour(glm::vec3(1.0f, 0.0f, 1.0f));
		team1Text->SetVisible(true);
	}

	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->GetCameraList()[0]->Position = glm::vec3(0.0f, 0.6f, 1.0f);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->GetCameraList()[0]->SetRotationY(90.0f);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->GetCameraList()[0]->SetRotationX(0.0f);

	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->GetCameraList()[1]->Position = glm::vec3(0.0f, 0.6f, -1.0f);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->GetCameraList()[1]->SetRotationY(-90.0f);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->GetCameraList()[1]->SetRotationX(0.0f);
}

void GameManager::Reset()
{
	p1Meter->SetVisible(false);
	p2Meter->SetVisible(false);
	p3Meter->SetVisible(false);
	p4Meter->SetVisible(false);

	divider->SetVisible(false);

	team1Text->SetVisible(false);
	team2Text->SetVisible(false);
	roundTimer.refeshCD();
	time = "90";
	roundTimerText->SetVisible(false);
	gameOver = false;
	outTeam1 = 0;
	outTeam2 = 0;

	vector<Player*> team1;
	vector<Player*> team2;

	for (unsigned int i = 0; i < playerList.size(); ++i) {
		playerList[i]->canRender = true;
		playerList[i]->SetIsOut(false);
		playerList[i]->SetCanMove(false);
		if (!playerList[i]->GetIsTargeting()) {
			playerList[i]->EnableTarget();
		}

		if (playerList[i]->GetPlayerNumber() == Player::PLAYERNUMBER::PLAYER1)
		{
			playerList[i]->SetWorldPosition(-3.0f, 0.0f, 4.0f);
		}
		if (playerList[i]->GetPlayerNumber() == Player::PLAYERNUMBER::PLAYER2)
		{
			playerList[i]->SetWorldPosition(3.0f, 0.0f, 4.0f);
		}
		if (playerList[i]->GetPlayerNumber() == Player::PLAYERNUMBER::PLAYER3)
		{
			playerList[i]->SetWorldPosition(3.0f, 0.0f, -4.0f);
		}
		if (playerList[i]->GetPlayerNumber() == Player::PLAYERNUMBER::PLAYER4)
		{
			playerList[i]->SetWorldPosition(-3.0f, 0.0f, -4.0f);
		}

		playerList[i]->SetTarget(0);

		playerList[i]->SetStats();
	}
}