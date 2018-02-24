#ifndef SOUNDEFFECTSELECTOR_H
#define SOUNDEFFECTSELECTOR_H

#include <string>
#include <SDL\SDL_mixer.h>
#include "SoundEffect.h"
#include "../Timers/MasterClock.h"
#include <vector>

namespace ENGINE {

	class SoundEffectSelector {
	public:
		SoundEffectSelector();
		~SoundEffectSelector();

		bool LoadNewSoundEffect(const std::string fileName);
		void PlayRandom();
		void Play(const int x) const;
		void SetVolume(const int musicVolume) const;

	protected:
	private:
		std::vector<SoundEffect*> SE;
	};
}

#endif
