#pragma once
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <BlueFlameEngine\Engine\BFEngine.h>
#include <BlueFlameEngine\Engine\Rendering\2D\TextUI.h>
#include <BlueFlameEngine\Engine\Rendering\2D\SliderUI.h>
#include <BlueFlameEngine\Engine\Timers\Cooldown.h>
#include <BlueFlameEngine\Engine\Audio\Announcer.h>
#include <BlueFlameEngine\Engine\Core\SceneManager.h>
#include "../Player/Player.h"

using namespace ENGINE;

namespace GAME {

	// This class manages the game rules, the players, and the hud
	class GameManager {

	public:
		GameManager(std::vector<Player*> players);
		~GameManager();

		void Initialize();
		void Update();

		// Enables match UI
		void StartMatch();
		// Starts match countdown
		void StartTimer();

		double GetMatchTimeRemaining() { return roundTimer.secondsLeft(); };

		void GameOver();

		bool IsGameOver() { return gameOver; };

		void Reset();

		bool canContinue = false;

		Announcer announcer;

	//private:
		SceneManager* sceneManager;

		int matchTime;

		Cooldown roundTimer;
		TextUI* roundTimerText;

		TextUI* team1Text;
		TextUI* team2Text;

		TextUI* gameoverText;
		ImageUI* gameoverBack;

		ImageUI* end;

		SliderUI* p1Meter;
		ImageUI* p1Portrait;
		SliderUI* p2Meter;
		ImageUI* p2Portrait;
		SliderUI* p3Meter;
		ImageUI* p3Portrait;
		SliderUI* p4Meter;
		ImageUI* p4Portrait;

		ImageUI* divider;

		std::vector<Player*> playerList;

		std::vector<Player*> team1List;
		std::vector<Player*> team2List;

		int outTeam1;
		int outTeam2;

		bool timeUp;
		bool teamOut;
		Cooldown endGameCD;

		bool gameOver;

		std::string time;

		bool player1voice = false;
		bool player2voice = false;
		bool player3voice = false;
		bool player4voice = false;

		bool endGame = false;

		Cooldown voiceCD;

		bool announcer30 = false;

		std::string GetPlayerPortrait(int playerIndex);
	};

}

#endif
