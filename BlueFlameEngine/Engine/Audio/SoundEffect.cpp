
#include <sstream>
#include "SoundEffect.h"

using namespace GAME;

SoundEffect::SoundEffect() {
}

SoundEffect::~SoundEffect() {
	Destroy();
}

bool SoundEffect::Load(const std::string& fileName) {
	Destroy();
	zero = 5;
	mixChunk = Mix_LoadWAV(fileName.c_str());

	if (!mixChunk) {
		//Debug::Log(EMessageType::ERROR, std::string(Mix_GetError()), __FILE__, __LINE__);
		printf(Mix_GetError());
		return false;
	}
	else {
		//Debug::Log(EMessageType::INFO, "Loaded the sound effect", __FILE__, __LINE__);
		printf("Sound effect loaded successfully\n");
	}
	return true;
}

void SoundEffect::Play(const int loopCount) const {
	Play();
}

void SoundEffect::Play() {
	//if (Mix_Playing(1) == false) {
		if (Mix_PlayChannel(-1, mixChunk, 0) == -1) {
			//Debug::Log(EMessageType::ERROR, std::string(Mix_GetError()), __FILE__, __LINE__);
			//printf(Mix_GetError());
		}
	//}
}

void SoundEffect::SetVolume(const int musicVolume) const {
	Mix_VolumeChunk(mixChunk, musicVolume);
}

void SoundEffect::Destroy() {
	Mix_FreeChunk(mixChunk);
	mixChunk = nullptr;
}
/*
void SoundEffect::Stop() const {
	if (Mix_PlayingMusic()) {
		Mix_HaltMusic();
	}
}

void SoundEffect::Pause() const {
	Mix_PauseMusic();
}

void SoundEffect::Resume() const {
	Mix_ResumeMusic();
}

void SoundEffect::TogglePause() const {
	if (Mix_PausedMusic()) {
		Mix_ResumeMusic();
	}
	else {
		Mix_PauseMusic();
	}
}
*/
