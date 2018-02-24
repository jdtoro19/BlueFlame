
#include <sstream>
#include "SoundEffectSelector.h"
#include "../Timers/MasterClock.h"

using namespace ENGINE;
SoundEffectSelector::SoundEffectSelector() {
}

SoundEffectSelector::~SoundEffectSelector() {
	//holy shit did Ivan just program garbage collection
	for (int i = 0; i < SE.size(); i++) {
		SE.back()->Destroy();
		SE.pop_back();
	}
	SE.clear();
}

bool SoundEffectSelector::LoadNewSoundEffect(const std::string fileName) {
	
	SoundEffect* tempSE = new SoundEffect();
	bool result = tempSE->Load(fileName);

	SE.push_back(tempSE);
	return result;
}

void SoundEffectSelector::PlayRandom() {
	int rand = Clock::GetInstance()->boundedRandomNumber(SE.size() - 1);
	SE.at(rand)->Play();
}

void SoundEffectSelector::Play(const int x) const {
	if (x < SE.size()) {
		SE.at(x)->Play();
	}
}

void SoundEffectSelector::SetVolume(const int musicVolume) const {
	for each (SoundEffect* s in SE) {
		s->SetVolume(musicVolume);
	}
}