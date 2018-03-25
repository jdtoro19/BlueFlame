#pragma once
#ifndef PLAYERDIALOGUE_H
#define PLAYERDIALOGUE_h

#include <string>
#include "Dialogue.h"
#include "../Core/Parser.h"
#include "../Timers/Cooldown.h"

namespace ENGINE {

	class PlayerDialogue : public Dialogue {
	public:
		PlayerDialogue();
		PlayerDialogue(double seconds);
		~PlayerDialogue();

		enum DIALOGUESTATE {
			MatchStart,
			WinMatch,
			LoseMatch,
			Moving,
			Idle,
			TakingDamage,
			RegularProjectile,
			SpecialProjectile,
			Falling,
			Interaction,
			Rare,
			SoundEffects,
			CharSelectAlex,
			CharSelectFlint,
			CharSelectJack,
			CharSelectKal,
			CharSelectOki
		};

		void setDialogueState(DIALOGUESTATE d);
		void playRandomFromCurrentState(bool overrideCD);
		void playRandomFromOtherState(DIALOGUESTATE d, bool overrideCD);
		void sometimesPlayRandomFromCurrentState(int percentage, bool overrideCD);
		void playSpecifiedFromState(DIALOGUESTATE d, int x);
		void playIdle();

		//now we're getting serious
		void LoadPlayerDialogue(std::string filename);

		int channel = 1;

	private:
		DIALOGUESTATE currentState = MatchStart;
		Parser p = Parser();
		bool hasCooldown = false;
		Cooldown idleTimer;

	};
}

#endif