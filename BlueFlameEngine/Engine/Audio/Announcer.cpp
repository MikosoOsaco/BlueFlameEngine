#include "Announcer.h"

using namespace ENGINE;

Announcer::Announcer() {
	minTimeBetweenSpeech = Cooldown(0);
	idleTimer = Cooldown(15.0); //used to play idle dialogue
	hasCooldown = false;
}

Announcer::Announcer(double averageSecondsBetweenLines) {
	minTimeBetweenSpeech = Cooldown(averageSecondsBetweenLines);
	idleTimer = Cooldown(15.0);
	hasCooldown = true;
}

Announcer::~Announcer() {

}

void Announcer::setDialogueState(DIALOGUESTATE d) {
	currentState = d;
}

void Announcer::playRandomFromCurrentState(bool overrideCD) {
	if (overrideCD || !hasCooldown) { //if there's a cooldown, makes sure that enough time has passed to talk again
									  //dialogueOptions.at(currentState)->PlayRandom();
		idleTimer.startCD();
		dialogueOptions.at(currentState)->SetChannel(channel);
		dialogueOptions.at(currentState)->PlayRandom();
	}
	else { //you can override the current cooldown and play no matter what with overrideCD = true;
		if (minTimeBetweenSpeech.checkOffCD()) {
			dialogueOptions.at(currentState)->PlayRandom();
			minTimeBetweenSpeech.startCD();
			idleTimer.startCD();
		}
	}

}

void Announcer::playRandomFromOtherState(DIALOGUESTATE d, bool overrideCD) {
	if (overrideCD) { //if there's a cooldown, makes sure that enough time has passed to talk again
					  //dialogueOptions.at(d)->PlayRandom();
		idleTimer.startCD();
		dialogueOptions.at(d)->SetChannel(channel);
		dialogueOptions.at(d)->PlayRandom();
	}
	else { //you can override the current cooldown and play no matter what with overrideCD = true;
		if (minTimeBetweenSpeech.checkOffCD()) {
			dialogueOptions.at(d)->PlayRandom();
			minTimeBetweenSpeech.startCD();
			idleTimer.startCD();
		}
	}
}

void Announcer::sometimesPlayRandomFromCurrentState(int percentage, bool overrideCD) {
	if (Clock::GetInstance()->generateRandomNumber() > percentage) {
		playRandomFromCurrentState(overrideCD);
		idleTimer.startCD();
	}
}

void Announcer::playSpecifiedFromState(DIALOGUESTATE d, int x) {
	if (x > dialogueOptions.at(d)->sizeOfMe() - 1) {
		x = dialogueOptions.at(d)->sizeOfMe() - 1;
	}
	dialogueOptions.at(d)->SetChannel(channel);
	dialogueOptions.at(d)->Play(x);
	idleTimer.startCD();
}

//now we're getting serious
void Announcer::LoadDialogue(std::string filename) {
	p.LoadFileIntoParser(filename, false);
	while (!p.isEmpty()) {
		Load(p.ParseBlock('=', '+'));
	}
}

void Announcer::playIdle() {
	if (idleTimer.checkOffCD()) {
		playRandomFromCurrentState(false);
	}
}
