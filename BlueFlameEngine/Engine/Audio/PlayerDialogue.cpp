#include "PlayerDialogue.h"

using namespace ENGINE;

PlayerDialogue::PlayerDialogue() {
	minTimeBetweenSpeech = Cooldown(0);
	hasCooldown = false;
}

PlayerDialogue::PlayerDialogue(double averageSecondsBetweenLines) {
	minTimeBetweenSpeech = Cooldown(averageSecondsBetweenLines);
	hasCooldown = true;
}

PlayerDialogue::~PlayerDialogue() {

}

void PlayerDialogue::setDialogueState(DIALOGUESTATE d) {
	currentState = d;
}

void PlayerDialogue::playRandomFromCurrentState(bool overrideCD) {
	if (overrideCD || !hasCooldown) { //if there's a cooldown, makes sure that enough time has passed to talk again
		dialogueOptions.at(currentState)->PlayRandom();
	}
	else { //you can override the current cooldown and play no matter what with overrideCD = true;
		if (minTimeBetweenSpeech.checkOffCD()) {
			dialogueOptions.at(currentState)->PlayRandom();
			minTimeBetweenSpeech.startCD();
		}
	}

}

void PlayerDialogue::playRandomFromOtherState(DIALOGUESTATE d, bool overrideCD) {
	if (overrideCD || !hasCooldown) { //if there's a cooldown, makes sure that enough time has passed to talk again
		dialogueOptions.at(d)->PlayRandom();
	}
	else { //you can override the current cooldown and play no matter what with overrideCD = true;
		if (minTimeBetweenSpeech.checkOffCD()) {
			dialogueOptions.at(d)->PlayRandom();
			minTimeBetweenSpeech.startCD();
		}
	}
}

void PlayerDialogue::sometimesPlayRandomFromCurrentState(int percentage, bool overrideCD) {
	if (Clock::GetInstance()->generateRandomNumber() > percentage) {
		playRandomFromCurrentState(overrideCD);
	}
}

void PlayerDialogue::playSpecifiedFromState(DIALOGUESTATE d, int x) {
	if (x > dialogueOptions.at(d)->sizeOfMe() - 1) {
		x = dialogueOptions.at(d)->sizeOfMe() - 1;
	}
	dialogueOptions.at(d)->Play(x);
}

//now we're getting serious
void PlayerDialogue::LoadPlayerDialogue(std::string filename) {
	p.LoadFileIntoParser(filename, false);
	while (!p.isEmpty()) {
		Load(p.ParseBlock('=', '+'));
	}
}
