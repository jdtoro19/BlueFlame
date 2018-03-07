#pragma once
#ifndef ANNOUNCER_H
#define ANNOUNCER_H

#include <string>
#include "Dialogue.h"
#include "../Core/Parser.h"

namespace ENGINE {

	class Announcer : public Dialogue {
	public:
		Announcer();
		Announcer(double averageSecondsBetweenLines);
		~Announcer();

		//enum dialogueStates {}; //defined by the inheriting classes
		enum DIALOGUESTATE {
			TitleScreen,
			CharSelect,
			MatchStart,
			TimeRunningOut,
			TimeEnds,
			WinScreen
		};

		void setDialogueState(DIALOGUESTATE d);
		void playRandomFromCurrentState(bool overrideCD);
		void playRandomFromOtherState(DIALOGUESTATE d, bool overrideCD);
		void sometimesPlayRandomFromCurrentState(int percentage, bool overrideCD);

		//now we're getting serious
		void LoadAnnouncerSet(std::string filename);
		void LoadDefaultAnnouncer();

	protected:
	private:
		DIALOGUESTATE currentState = TitleScreen;
		Parser p = Parser();
		bool hasCooldown = false;
	};
}

#endif