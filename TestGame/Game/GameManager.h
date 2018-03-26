#pragma once
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <BlueFlameEngine\Engine\BFEngine.h>
#include <BlueFlameEngine\Engine\Rendering\2D\TextUI.h>
#include <BlueFlameEngine\Engine\Rendering\2D\SliderUI.h>
#include <BlueFlameEngine\Engine\Timers\Cooldown.h>
#include "../Player/Player.h"

using namespace ENGINE;

namespace GAME {

	class GameManager {

	public:
		GameManager(std::vector<Player*> players);
		~GameManager();

		void Initialize();
		void Update();

		
		void StartMatch();

		void StartTimer();

		double GetMatchTimeRemaining() { return roundTimer.secondsLeft(); };

		void GameOver();

		bool IsGameOver() { return gameOver; };

		void Reset();

		bool canContinue = false;

	private:
		int matchTime;

		Cooldown roundTimer;
		TextUI* roundTimerText;

		TextUI* team1Text;
		TextUI* team2Text;

		TextUI* gameoverText;
		ImageUI* gameoverBack;

		ImageUI* end;

		SliderUI* p1Meter;
		SliderUI* p2Meter;
		SliderUI* p3Meter;
		SliderUI* p4Meter;

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
	};

}

#endif
