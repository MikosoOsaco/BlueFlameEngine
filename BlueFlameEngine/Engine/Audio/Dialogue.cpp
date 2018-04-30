#include "Dialogue.h"

using namespace ENGINE;

Dialogue::Dialogue() {

}

Dialogue::~Dialogue() {

}

bool const Dialogue::Load(const std::vector<std::string> fileNames) {
	
	SoundEffectSelector *SES = new SoundEffectSelector();

	bool totalsuccess = true;
	bool tempsuccess;

	for each (std::string file in fileNames) {
		if ((tempsuccess = SES->LoadNewSoundEffect(file)) == false) {
			totalsuccess = false;
		}	
	}

	dialogueOptions.push_back(SES);

	return totalsuccess;
	
}

void Dialogue::SetVolume(int volume) {
	for each (SoundEffectSelector* SE in dialogueOptions) {
		SE->SetVolume(volume);
	}
}
void Dialogue::Destroy() {
	//someone will program this later
}