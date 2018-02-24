#pragma once
#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <string>
#include "SoundEffectSelector.h"

namespace ENGINE {

	class Dialogue {
	public:
		Dialogue();
		virtual ~Dialogue();
		
		/// Pure viturals
		bool const Load(const std::vector<std::string> fileNames) const;
		void SetVolume(int volume);
		void Destroy();
	protected:
		//all dialogue is stored in dialogueOptions, via soundeffectselectors.
		std::vector<SoundEffectSelector*> dialogueOptions;

		enum dialogueStates {}; //defined by the inheriting classes
	private:
		
	};
}

#endif