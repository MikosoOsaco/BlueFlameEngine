#pragma once
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <BlueFlameEngine\Engine\BFEngine.h>
#include <BlueFlameEngine\Engine\Rendering\2D\TextUI.h>
#include <BlueFlameEngine\Engine\Rendering\2D\SliderUI.h>
#include <BlueFlameEngine\Engine\Timers\Cooldown.h>
#include <BlueFlameEngine\Engine\Audio\Announcer.h>
#include <BlueFlameEngine\Engine\Core\SceneManager.h>
#include "../Player/Player.h"
#include "../Player/PlayerWidget.h"

using namespace ENGINE;

namespace GAME {

	// This class manages the game rules, the players, and the hud
	class GameManager {

	public:
		GameManager(std::vector<Player*> players);
		~GameManager();

		void Initialize();
		void Update();

		// Enables match UI
		void StartMatch();
		// Starts match countdown
		void StartTimer();

		// Match status
		void Reset();
		void GameOver();
		bool IsGameOver() { return gameOver; };
		double GetMatchTimeRemaining() { return roundTimer.secondsLeft(); };

		// UI
		TextUI* roundTimerText;
		TextUI* team1Text;
		TextUI* team2Text;
		TextUI* gameoverText;
		ImageUI* gameoverBack;
		ImageUI* divider;
		ImageUI* end;
		PlayerWidget* p1Widget;
		PlayerWidget* p2Widget;
		PlayerWidget* p3Widget;
		PlayerWidget* p4Widget;

		// Timers
		Cooldown roundTimer;
		Cooldown endGameCD;
		Cooldown voiceCD;

		// Player lists
		std::vector<Player*> playerList;
		std::vector<Player*> team1List;
		std::vector<Player*> team2List;

		// Game status
		int matchTime;
		int outTeam1;
		int outTeam2;
		bool timeUp;
		bool teamOut;
		bool gameOver;
		bool endGame = false;
		bool canContinue = false;
		bool announcer30 = false;
		bool player1voice = false;
		bool player2voice = false;
		bool player3voice = false;
		bool player4voice = false;
		std::string time;

		// Announcer
		Announcer announcer;

		// Scene Manager
		SceneManager* sceneManager;
	};
}
#endif
