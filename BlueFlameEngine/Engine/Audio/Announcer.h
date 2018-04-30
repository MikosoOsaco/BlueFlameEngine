#pragma once
#ifndef ANNOUNCER_H
#define ANNOUNCER_h

#include <string>
#include "Dialogue.h"
#include "../Core/Parser.h"
#include "../Timers/Cooldown.h"

namespace ENGINE {

	class Announcer : public Dialogue {
	public:
		Announcer();
		Announcer(double seconds);
		~Announcer();

		enum DIALOGUESTATE {
			Title,
			CharacterSelect,
			CharacterSelectAlt,
			MatchStart,
			MatchStartAlt,
			MatchEnding,
			MatchEnded,
			NoContest,
			RedTeam,
			RedTeamRoll,
			BlueTeam,
			BlueTeamRoll,
			CharSelectAlex,
			CharSelectFlint,
			CharSelectJack,
			CharSelectKal,
			CharSelectOki
		};

		void setDialogueState(DIALOGUESTATE d);
		void playRandomFromCurrentState(bool overrideCD);
		void playRandomFromOtherState(DIALOGUESTATE d, bool overrideCD);
		void sometimesPlayRandomFromCurrentState(int percentage, bool overrideCD);
		void playSpecifiedFromState(DIALOGUESTATE d, int x);
		void playIdle();
		void stopPlaying() { Mix_HaltChannel(channel); };

		void LoadDialogue(std::string filename);

		int channel = 1;

	private:
		DIALOGUESTATE currentState = MatchStart;
		Parser p = Parser();
		bool hasCooldown = false;
		Cooldown idleTimer;

	};
}

#endif