#pragma once
#ifndef ANNOUNCER_H
#define ANNOUNCER_H

#include <string>
#include "Dialogue.h"

namespace ENGINE {

	class Announcer : Dialogue {
	public:
		Announcer();
		virtual ~Announcer();

		//enum dialogueStates {}; //defined by the inheriting classes
		enum DIALOGUESTATE {
			TitleScreen,
			CharSelect,
			MatchStart,
			TimeRunningOut,
			TimeEnds,
			WinScreen
		};

		inline void setDialogueState(DIALOGUESTATE d) { currentState = d; }
		inline void playRandomFromCurrentState() { dialogueOptions.at(currentState)->PlayRandom(); };
		inline void playRandomFromOtherState(DIALOGUESTATE d) { dialogueOptions.at(d)->PlayRandom(); };
		inline void sometimesPlayRandomFromCurrentState(int percentage) {
			if (Clock::GetInstance()->generateRandomNumber() > percentage) {
				playRandomFromCurrentState();
			}
		}

	protected:
	private:
		DIALOGUESTATE currentState = TitleScreen;

		
	};
}

#endif