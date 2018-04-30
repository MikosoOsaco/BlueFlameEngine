#ifndef SOUNDEFFECTSELECTOR_H
#define SOUNDEFFECTSELECTOR_H

#include <string>
#include <SDL\SDL_mixer.h>
#include "SoundEffect.h"
#include "../Timers/MasterClock.h"
#include "../Core/Parser.h"
#include "../Math/Randomizer.h"
#include <vector>

namespace ENGINE {

	class SoundEffectSelector {
	public:
		SoundEffectSelector();
		~SoundEffectSelector();

		bool LoadNewSoundEffect(const std::string fileName); //load one file
		void LoadEffectsFromFile(const std::string filenames); //load multiple files
		void PlayRandom();
		void Play(const int x) const;
		void SetVolume(const int musicVolume) const;
		void SetChannel(int c) { channel = c; };
		int sizeOfMe();

	protected:
		std::vector<SoundEffect*> SE;
		int channel = 1;
	private:
		
	};
}

#endif
