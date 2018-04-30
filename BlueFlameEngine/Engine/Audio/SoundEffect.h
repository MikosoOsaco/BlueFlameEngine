#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H

#include <string>
#include "AudioFile.h"
#include <SDL\SDL_mixer.h>


//totally different from audiofile.h
//included for effect

namespace ENGINE {

	class SoundEffect : public AudioFile {
	public:
		SoundEffect();
		SoundEffect(int volume);

		/// Delete the possible constructors and operators 
		SoundEffect(const SoundEffect&) = delete;
		SoundEffect(SoundEffect&&) = delete;
		SoundEffect& operator=(const SoundEffect&) = delete;
		SoundEffect& operator=(SoundEffect&&) = delete;

		~SoundEffect();

		virtual bool Load(const std::string& fileName) override;
		virtual void Play(const int loopCount = 0) const override;
		virtual void Play(int channel);
		virtual void SetVolume(int volume) const override;
		virtual void Destroy() override;
		/*void Stop() const;
		void Pause() const;
		void Resume() const;
		void TogglePause() const;*/
		Mix_Chunk* mixChunk = nullptr;
	protected:
		int volume = 0;
	private:
		
	};
}

#endif
