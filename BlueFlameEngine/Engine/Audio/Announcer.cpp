#include "Announcer.h"

using namespace ENGINE;

Announcer::Announcer() {
	minTimeBetweenSpeech = Cooldown(0);
	hasCooldown = false;
}

Announcer::Announcer(double averageSecondsBetweenLines) {
	minTimeBetweenSpeech = Cooldown(averageSecondsBetweenLines);
	hasCooldown = false;
}

Announcer::~Announcer() {

}

void Announcer::setDialogueState(DIALOGUESTATE d) { 
	currentState = d; 
}

void Announcer::playRandomFromCurrentState(bool overrideCD) {
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

void Announcer::playRandomFromOtherState(DIALOGUESTATE d, bool overrideCD) {
	dialogueOptions.at(d)->PlayRandom(); 
}

void Announcer::sometimesPlayRandomFromCurrentState(int percentage, bool overrideCD) {
	if (Clock::GetInstance()->generateRandomNumber() > percentage) {
		playRandomFromCurrentState(overrideCD);
	}
}

//now we're getting serious
void Announcer::LoadAnnouncerSet(std::string filename) {
	p.LoadFileIntoParser(filename, false);
	while (!p.isEmpty()) {
		Load(p.ParseBlock('=', '+'));
	}
}
void Announcer::LoadDefaultAnnouncer() {
	LoadAnnouncerSet("defaultfilepath");
}