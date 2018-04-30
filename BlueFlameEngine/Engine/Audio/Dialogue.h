#pragma once
#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <string>
#include "SoundEffectSelector.h"
#include "../Timers/Cooldown.h"

namespace ENGINE {

	class Dialogue {
	public:
		Dialogue();
		virtual ~Dialogue();
		
		/// Pure viturals
		bool const Load(const std::vector<std::string> fileNames);
		virtual void SetVolume(int volume);
		void Destroy();
	protected:
		//all dialogue is stored in dialogueOptions, via soundeffectselectors.
		std::vector<SoundEffectSelector*> dialogueOptions;

		Cooldown minTimeBetweenSpeech;

		//enum dialogueStates {}; //defined by the inheriting classes
	private:
		
	};
}

#endif