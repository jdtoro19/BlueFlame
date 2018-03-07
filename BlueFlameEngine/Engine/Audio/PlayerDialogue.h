#pragma once
#ifndef PLAYERDIALOGUE_H
#define PLAYERDIALOGUE_h

#include <string>
#include "Dialogue.h"
#include "../Core/Parser.h"
#include "../Timers/Cooldown.h"

namespace ENGINE {

	class PlayerDialogue : Dialogue {
	public:
		PlayerDialogue();
		PlayerDialogue(double seconds);
		~PlayerDialogue();

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

		void setDialogueState(DIALOGUESTATE d);
		void playRandomFromCurrentState(bool overrideCD);
		void playRandomFromOtherState(DIALOGUESTATE d, bool overrideCD);
		void sometimesPlayRandomFromCurrentState(int percentage, bool overrideCD);

		//now we're getting serious
		void LoadPlayerDialogue(std::string filename);

	protected:
	private:
		DIALOGUESTATE currentState = CharSelect;
		Parser p = Parser();
		bool hasCooldown = false;

	};
}

#endif