#include "Music.h"

using namespace ENGINE;

Music::Music() :mixMusic(nullptr) {}

Music::~Music() {
	Destroy();
}

bool Music::Load(const std::string& fileName) {
	Destroy();
	mixMusic = Mix_LoadMUS(fileName.c_str());

	if (!mixMusic) {
		//Debug::Log(EMessageType::ERROR, std::string(Mix_GetError()), __FILE__, __LINE__);
		printf(Mix_GetError());
		return false;
	}
	else {
		//Debug::Log(EMessageType::INFO, "Loaded the music file", __FILE__, __LINE__);
		printf("Loaded the music File");
	}
	return true;
}

void Music::Play(const int loopCount) const {
	Stop();
	if (Mix_PlayMusic(mixMusic, loopCount) != 0) {
		//Debug::Log(EMessageType::ERROR, std::string(Mix_GetError()), __FILE__, __LINE__);
		printf(Mix_GetError());
	}
}

void Music::SetVolume(const int musicVolume) const {
	Mix_VolumeMusic(musicVolume);
}

void Music::Destroy() {
	Mix_FreeMusic(mixMusic);
	mixMusic = nullptr;
}

void Music::Stop() const {
	if (Mix_PlayingMusic()) {
		Mix_HaltMusic();
	}
}

void Music::Pause() const {
	Mix_PauseMusic();
}

void Music::Resume() const {
	Mix_ResumeMusic();
}

void Music::TogglePause() const {
	if (Mix_PausedMusic()) {
		Mix_ResumeMusic();
	}
	else {
		Mix_PauseMusic();
	}
}
