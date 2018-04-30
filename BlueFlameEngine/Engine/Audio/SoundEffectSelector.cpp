
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

void SoundEffectSelector::LoadEffectsFromFile(const std::string filenames) {
	Parser p = Parser();
	p.LoadFileIntoParser(filenames, false);

	while (!p.isEmpty()) {
		std::vector<std::string> fileNames = p.ParseBlock('=', '+');
		for each (std::string file in fileNames) {
			LoadNewSoundEffect(file);
		}
	}
}

void SoundEffectSelector::PlayRandom() {
	Randomizer rand = Randomizer();
	int r = rand.rand(0, SE.size());
	SE.at(r)->Play(channel);
}

void SoundEffectSelector::Play(const int x) const {
	if (x < SE.size()) {
		SE.at(x)->Play(channel);
	}
}

void SoundEffectSelector::SetVolume(const int musicVolume) const {
	for each (SoundEffect* s in SE) {
		s->SetVolume(musicVolume);
	}
}

int SoundEffectSelector::sizeOfMe() {
	return SE.size();
}