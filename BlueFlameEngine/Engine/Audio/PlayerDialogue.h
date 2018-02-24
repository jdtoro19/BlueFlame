#pragma once
#ifndef PLAYERDIALOGUE_H
#define PLAYERDIALOGUE_h

#include <string>
#include "Dialogue.h"

namespace ENGINE {

	class PlayerDialogue : Dialogue {
	public:
		PlayerDialogue();
		virtual ~PlayerDialogue();

		//enum dialogueStates {}; //defined by the inheriting classes
		enum DIALOGUESTATE {
			CharSelect,
			MatchStart,
			WinMatch,
			LoseMatch,
			Moving,
			Idle,
			TakingDamage,
			RegularProjectile,
			SpecialProjectile,
			Falling,
			Interaction
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
		DIALOGUESTATE currentState = CharSelect;


	};
}

#endif