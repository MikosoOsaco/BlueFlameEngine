#include "PlayerDialogue.h"

using namespace ENGINE;

PlayerDialogue::PlayerDialogue() {
	minTimeBetweenSpeech = Cooldown(0);
	idleTimer = Cooldown(15.0); //used to play idle dialogue
	hasCooldown = false;
}

PlayerDialogue::PlayerDialogue(double averageSecondsBetweenLines) {
	minTimeBetweenSpeech = Cooldown(averageSecondsBetweenLines);
	idleTimer = Cooldown(15.0);
	hasCooldown = true;
}

PlayerDialogue::~PlayerDialogue() {

}

void PlayerDialogue::setDialogueState(DIALOGUESTATE d) {
	currentState = d;
}

void PlayerDialogue::playRandomFromCurrentState(bool overrideCD) {
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

void PlayerDialogue::playRandomFromOtherState(DIALOGUESTATE d, bool overrideCD) {
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

void PlayerDialogue::sometimesPlayRandomFromCurrentState(int percentage, bool overrideCD) {
	if (Clock::GetInstance()->generateRandomNumber() > percentage) {
		playRandomFromCurrentState(overrideCD);
		idleTimer.startCD();
	}
}

void PlayerDialogue::sometimesPlayRandomFromOtherState(DIALOGUESTATE d, int percentage, bool overrideCD) {
	if (Clock::GetInstance()->generateRandomNumber() > percentage) {
		playRandomFromOtherState(d, overrideCD);
		idleTimer.startCD();
	}
}

void PlayerDialogue::playSpecifiedFromState(DIALOGUESTATE d, int x) {
	if (x > dialogueOptions.at(d)->sizeOfMe() - 1) {
		x = dialogueOptions.at(d)->sizeOfMe() - 1;
	}
	dialogueOptions.at(d)->SetChannel(channel);
	dialogueOptions.at(d)->Play(x);
	idleTimer.startCD();
}

//now we're getting serious
void PlayerDialogue::LoadPlayerDialogue(std::string filename) {
	p.LoadFileIntoParser(filename, false);
	while (!p.isEmpty()) {
		Load(p.ParseBlock('=', '+'));
	}
}

void PlayerDialogue::playIdle() {
	if (idleTimer.checkOffCD()) {
		playRandomFromCurrentState(false);
	}
}
