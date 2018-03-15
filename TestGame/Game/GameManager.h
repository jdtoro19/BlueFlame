#pragma once
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <BlueFlameEngine\Engine\BFEngine.h>
#include <BlueFlameEngine\Engine\Rendering\2D\TextUI.h>
#include <BlueFlameEngine\Engine\Timers\Cooldown.h>

using namespace ENGINE;

namespace GAME {

	class GameManager {

	public:
		GameManager();
		~GameManager();

		void Initialize();
		void Update();

	//private:
		Cooldown roundTimer;
		TextUI* roundTimerText;

	};

}

#endif
