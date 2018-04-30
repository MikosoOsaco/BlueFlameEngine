#include "GameManager.h"

using namespace GAME;

GameManager::GameManager(std::vector<Player*> players)
{
	playerList = players;
}

GameManager::~GameManager()
{
}

void GameManager::Initialize()
{
	// Get reference to scene manager
	sceneManager = BFEngine::GetInstance()->GetSceneManager();

	// Create announcer
	announcer = Announcer();
	announcer.LoadDialogue("Resources/Audio/AnnouncerAudio.txt");
	announcer.channel = 1;
	announcer.SetVolume(60);

	// Set game status checks
	outTeam1 = 0;
	outTeam2 = 0;
	teamOut = false;
	timeUp = false;
	gameOver = false;

	// Set timers and match time
	matchTime = 92;
	roundTimer = Cooldown(matchTime);
	endGameCD = Cooldown(3.0);
	voiceCD = Cooldown(4.5);

	// UI
	roundTimerText = new TextUI();
	roundTimerText->SetFont("Resources/Fonts/ka1.ttf");
	roundTimerText->SetText(std::to_string(matchTime - 2));
	roundTimerText->SetColour(1.0f, 1.0f, 1.0f);
	roundTimerText->SetSize(1.0f);
	roundTimerText->SetSpacing(9.0f);
	roundTimerText->SetPosition((BFEngine::GetInstance()->GetSceneManager()->GetScreenWidth() / 2 - 95.0f), 80.0f); // 60
	roundTimerText->SetVisible(false);

	team1Text = new TextUI();
	team1Text->SetFont("Resources/Fonts/ka1.ttf");
	team1Text->SetText("Winner");
	team1Text->SetColour(1.0f, 1.0f, 1.0f);
	team1Text->SetSize(1.0f);
	team1Text->SetSpacing(9.0f);
	team1Text->SetPosition((team1Text->GetLength() / 2) - 50, (BFEngine::GetInstance()->GetSceneManager()->GetScreenHeight() / 2) - 200);
	team1Text->SetVisible(false);

	team2Text = new TextUI();
	team2Text->SetFont("Resources/Fonts/ka1.ttf");
	team2Text->SetText("Loser");
	team2Text->SetColour(1.0f, 1.0f, 1.0f);
	team2Text->SetSize(1.0f);
	team2Text->SetSpacing(9.0f);
	team2Text->SetPosition(((team1Text->GetLength() / 2) - 50) + (BFEngine::GetInstance()->GetSceneManager()->GetScreenWidth() / 2), (BFEngine::GetInstance()->GetSceneManager()->GetScreenHeight() / 2) - 200);
	team2Text->SetVisible(false);

	gameoverText = new TextUI();
	gameoverText->SetFont("Resources/Fonts/ka1.ttf");
	gameoverText->SetText("Red Team Wins");
	gameoverText->SetColour(1.0f, 1.0f, 1.0f);
	gameoverText->SetSize(1.0f);
	gameoverText->SetSpacing(9.0f);
	gameoverText->SetPosition((BFEngine::GetInstance()->GetSceneManager()->GetScreenWidth() / 2) - gameoverText->GetLength() / 2, (BFEngine::GetInstance()->GetSceneManager()->GetScreenHeight() / 2));
	gameoverText->SetVisible(false);

	gameoverBack = new ImageUI;
	gameoverBack->SetImage("Resources/Textures/banner.png");
	gameoverBack->SetPosition(960.0f, 510.0f);
	gameoverBack->SetScale(5.0f, 1.5f);
	gameoverBack->SetVisible(false);

	end = new ImageUI;
	end->SetImage("Resources/Textures/end.png");
	end->SetPosition(960.0f, 540.0f);
	end->SetScale(1.0f);
	end->SetVisible(false);

	divider = new ImageUI;
	divider->SetImage("Resources/Textures/divider.png");
	divider->SetPosition(960.0f, 540.0f);
	divider->SetScale(1.0f);
	divider->SetVisible(false);

	std::string playersFromReplay;
	if (Settings::getInstance()->replaySystemEnabled && Settings::getInstance()->playingReplay == true) {
		if (Replay::GetInstance()->TryLoadReplay("defaultReplay.txt")) {
			
			//std::cout << "Replay loaded successfully" << std::endl;
		}
		else {
			//std::cout << "Replay failed to load" << std::endl;
		}
		playersFromReplay = Replay::GetInstance()->PullFromReplay();
	}

	std::vector<int> PlayerImages;

	if (Settings::getInstance()->replaySystemEnabled && Settings::getInstance()->playingReplay) {
		for (int i = 0; i < 4; i++) {
			std::string replaySaveData = playersFromReplay.substr(i, 1);
			if (replaySaveData == "0") {
				PlayerImages.push_back(0);
			}
			else if (replaySaveData == "1") {
				PlayerImages.push_back(1);
			}
			else if (replaySaveData == "2") {
				PlayerImages.push_back(2);
			}
			else if (replaySaveData == "3") {
				PlayerImages.push_back(3);
			}
			else if (replaySaveData == "4") {
				PlayerImages.push_back(4);
			}
			else
			{
				PlayerImages.push_back(0);
			}
		}
	}
	else {
		for each (int x in sceneManager->saveData) {
			PlayerImages.push_back(x);
		}
	}

	if (PlayerImages.size() != 0) {
		p1Widget = new PlayerWidget(PlayerImages[0], 110, 1000);
		p1Widget->SetVisible(false);

		p2Widget = new PlayerWidget(PlayerImages[1], 560, 1000);
		p2Widget->SetVisible(false);

		p3Widget = new PlayerWidget(PlayerImages[2], 1920 - 800, 1000);
		p3Widget->SetVisible(false);

		p4Widget = new PlayerWidget(PlayerImages[3], 1920 - 350, 1000);
		p4Widget->SetVisible(false);
	}
	else
	{
		p1Widget = new PlayerWidget(-1, 110, 1000);
		p1Widget->SetVisible(false);

		p2Widget = new PlayerWidget(-1, 560, 1000);
		p2Widget->SetVisible(false);

		p3Widget = new PlayerWidget(-1, 1920 - 800, 1000);
		p3Widget->SetVisible(false);

		p4Widget = new PlayerWidget(-1, 1920 - 350, 1000);
		p4Widget->SetVisible(false);
	}

	// Add all UI objects
	sceneManager->GetCurrentScene()->AddUIObject(divider);
	sceneManager->GetCurrentScene()->AddUIObject(end);
	sceneManager->GetCurrentScene()->AddUIObject(roundTimerText);
	sceneManager->GetCurrentScene()->AddUIObject(team1Text);
	sceneManager->GetCurrentScene()->AddUIObject(team2Text);
	sceneManager->GetCurrentScene()->AddUIObject(gameoverBack);
	sceneManager->GetCurrentScene()->AddUIObject(gameoverText);
	sceneManager->GetCurrentScene()->AddUIObject(p1Widget);
	sceneManager->GetCurrentScene()->AddUIObject(p2Widget);
	sceneManager->GetCurrentScene()->AddUIObject(p3Widget);
	sceneManager->GetCurrentScene()->AddUIObject(p4Widget);
}

void GameManager::Update()
{
	// Update player special meters
	p1Widget->SetMeterValue(playerList[0]->GetSpecialMeter());
	p2Widget->SetMeterValue(playerList[1]->GetSpecialMeter());
	p3Widget->SetMeterValue(playerList[2]->GetSpecialMeter());
	p4Widget->SetMeterValue(playerList[3]->GetSpecialMeter());

	// Set match time text
	time = std::to_string((int)roundTimer.secondsLeft() - 1);
	if (time.size() == 1) {
		time = "0" + time;
	}
	if (!roundTimer.checkOffCD()) {
		roundTimerText->SetText(time);
	}

	// Check to make sure game is not over
	if (!gameOver) {
		// Loop through players
		for (unsigned int i = 0; i < playerList.size(); ++i) {
			// Check if player is below the arena and mark them as out
			if (playerList[i]->GetWorldPosition().y < -10) {
				playerList[i]->SetIsOut(true);
				playerList[i]->canRender = false;
				playerList[i]->dialogue.playRandomFromOtherState(PlayerDialogue::Falling, true);
				if (playerList[i]->GetPlayerTeam() == Player::PLAYERTEAM::TEAM1) {
					outTeam1++;
				}
				if (playerList[i]->GetPlayerTeam() == Player::PLAYERTEAM::TEAM2) {
					outTeam2++;
				}
			}
			// Check if player is out to disavle gravity
			if (playerList[i]->IsOut()) {
				playerList[i]->physicsComponent->hasGravity = false;
				playerList[i]->SetWorldPosition(0.0f, -5.0f, 0.0f);
			}
		}

		// Make sure no teams are out
		if (!teamOut) {
			// If a team is out play announcer dialogue
			if (outTeam1 == 2 || outTeam2 == 2) {
				timeUp = true;
				teamOut = true;
				roundTimer.refeshCD();
				if (outTeam2 == 2) {
					announcer.playRandomFromOtherState(announcer.BlueTeam, false);
				}
				else
				{
					announcer.playRandomFromOtherState(announcer.RedTeam, false);
				}
			}
		}
	}

	// Player announcer line when time is up
	if (!roundTimer.checkOffCD() && roundTimer.secondsLeft() <= 1 && !teamOut) {
		announcer.playRandomFromOtherState(announcer.MatchEnded, false);
		timeUp = true;
	}

	// Check to see if time is up
	if (timeUp) {
		// Set text depending on which team won or if time ran out
		BFEngine::GetInstance()->GetSceneManager()->GetRenderer()->EnableKernel(true);
		for (unsigned int i = 0; i < playerList.size(); ++i) {
			playerList[i]->SetCanMove(false);
		}
		roundTimerText->SetText("00");
		if (outTeam2 == 2) {
			gameoverText->SetColour(0.0f, 0.0f, 1.0f);
			gameoverText->SetText("Blue Team Wins");
			gameoverText->SetPosition((BFEngine::GetInstance()->GetSceneManager()->GetScreenWidth() / 2) - gameoverText->GetLength() / 2, (BFEngine::GetInstance()->GetSceneManager()->GetScreenHeight() / 2));
			gameoverText->SetVisible(true);
		}
		else if (outTeam1 == 2) {
			gameoverText->SetColour(1.0f, 0.0f, 0.0f);
			gameoverText->SetText("Red Team Wins");
			gameoverText->SetPosition((BFEngine::GetInstance()->GetSceneManager()->GetScreenWidth() / 2) - gameoverText->GetLength() / 2, (BFEngine::GetInstance()->GetSceneManager()->GetScreenHeight() / 2));
			gameoverText->SetVisible(true);
		}
		else {
			gameoverText->SetColour(1.0f, 0.0f, 1.0f);
			gameoverText->SetText("Time's Up");
			gameoverText->SetPosition((BFEngine::GetInstance()->GetSceneManager()->GetScreenWidth() / 2) - gameoverText->GetLength() / 2, (BFEngine::GetInstance()->GetSceneManager()->GetScreenHeight() / 2));
			gameoverText->SetVisible(true);
		}
		gameoverBack->SetVisible(true);
		endGameCD.startCD();
		timeUp = false;
	}

	// Once the someone wins or time is up set game over to true;
	if (!endGameCD.checkOffCD() && endGameCD.secondsLeft() <= 1) {
		gameOver = true;
	}
	if (gameOver && !endGame) {
		GameOver();
	}

	// Check if the game is over
	if (gameOver) {
		// Play all character dialogue lines
		if (outTeam2 == 2) {
			if (!player1voice) {
				team1List[0]->dialogue.playRandomFromOtherState(team1List[0]->dialogue.WinMatch, true);
				player1voice = true;
			}
			if (!player2voice && voiceCD.secondsLeft() < 3.0) {
				team1List[1]->dialogue.playRandomFromOtherState(team1List[1]->dialogue.WinMatch, true);
				player2voice = true;
			}

			if (!player3voice && voiceCD.secondsLeft() < 1.5) {
				team2List[0]->dialogue.playRandomFromOtherState(team2List[0]->dialogue.LoseMatch, true);
				player3voice = true;
			}
			if (!player4voice && voiceCD.secondsLeft() < 0) {
				team2List[1]->dialogue.playRandomFromOtherState(team2List[1]->dialogue.LoseMatch, true);
				player4voice = true;
			}
		}
		else if (outTeam1 == 2) {
			if (!player1voice) {
				team1List[0]->dialogue.playRandomFromOtherState(team1List[0]->dialogue.LoseMatch, true);
				player1voice = true;
			}
			if (!player2voice && voiceCD.secondsLeft() < 3.0) {
				team1List[1]->dialogue.playRandomFromOtherState(team1List[1]->dialogue.LoseMatch, true);
				player2voice = true;
			}

			if (!player3voice && voiceCD.secondsLeft() < 1.5) {
				team2List[0]->dialogue.playRandomFromOtherState(team2List[0]->dialogue.WinMatch, true);
				player3voice = true;
			}
			if (!player4voice && voiceCD.secondsLeft() < 0) {
				team2List[1]->dialogue.playRandomFromOtherState(team2List[1]->dialogue.WinMatch, true);
				player4voice = true;
			}
		}
		else 
		{
			if (!player1voice) {
				team1List[0]->dialogue.playRandomFromOtherState(team1List[0]->dialogue.LoseMatch, true);
				player1voice = true;
			}
			if (!player2voice && voiceCD.secondsLeft() < 3.0) {
				team1List[1]->dialogue.playRandomFromOtherState(team1List[1]->dialogue.LoseMatch, true);
				player2voice = true;
			}

			if (!player3voice && voiceCD.secondsLeft() < 1.5) {
				team2List[0]->dialogue.playRandomFromOtherState(team2List[0]->dialogue.LoseMatch, true);
				player3voice = true;
			}
			if (!player4voice && voiceCD.secondsLeft() < 0) {
				team2List[1]->dialogue.playRandomFromOtherState(team2List[1]->dialogue.LoseMatch, true);
				player4voice = true;
			}
		}
	}

	// Player 30 second announcer warning 
	if (!roundTimer.checkOffCD() && roundTimer.secondsLeft() <= 32 && !announcer30) {
		announcer.playRandomFromOtherState(announcer.MatchEnding, false);
		announcer30 = true;
	}

	// when game ends stop all players from moving
	if (gameOver)
	{
		for (unsigned int i = 0; i < playerList.size(); ++i) {
			playerList[i]->physicsComponent->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
}

void GameManager::StartMatch()
{
	roundTimerText->SetText(std::to_string(matchTime - 2));
	roundTimerText->SetVisible(true);

	p1Widget->SetVisible(true);
	p2Widget->SetVisible(true);
	p3Widget->SetVisible(true);
	p4Widget->SetVisible(true);

	divider->SetVisible(true);

	//set portraits and status bars to true, return to original positions
	p1Widget->SetMeterVisible(true);
	p1Widget->GetPortraitImage()->SetPosition(110, 1000);
	p2Widget->SetMeterVisible(true);
	p2Widget->GetPortraitImage()->SetPosition(560, 1000);
	p3Widget->SetMeterVisible(true);
	p3Widget->GetPortraitImage()->SetPosition(1920 - 800, 1000);
	p4Widget->SetMeterVisible(true);
	p4Widget->GetPortraitImage()->SetPosition(1920 - 350, 1000);
}

void GameManager::StartTimer()
{
	roundTimer.startCD();

	for (unsigned int i = 0; i < playerList.size(); ++i) {
		playerList[i]->SetCanMove(true);
	}
}

void GameManager::GameOver()
{
	BFEngine::GetInstance()->GetSceneManager()->GetRenderer()->EnableKernel(false);

	//move portraits to screen edges
	p1Widget->GetPortraitImage()->SetPosition(110, 1000);
	p2Widget->GetPortraitImage()->SetPosition(310, 1000);
	p3Widget->GetPortraitImage()->SetPosition(1920 - 310, 1000);
	p4Widget->GetPortraitImage()->SetPosition(1920 - 110, 1000);

	gameoverText->SetVisible(false);
	gameoverBack->SetVisible(false);

	roundTimer.refeshCD();
	roundTimerText->SetVisible(false);

	voiceCD.startCD();

	p1Widget->SetMeterVisible(false);
	p2Widget->SetMeterVisible(false);
	p3Widget->SetMeterVisible(false);
	p4Widget->SetMeterVisible(false);

	end->SetVisible(true);

	for (unsigned int i = 0; i < playerList.size(); ++i) {
		playerList[i]->SetIsOut(true);
		playerList[i]->SetCanMove(true);
		playerList[i]->canRender = true;
		playerList[i]->physicsComponent->hasGravity = true;
		playerList[i]->StopBlock();
		if (playerList[i]->GetPlayerTeam() == Player::PLAYERTEAM::TEAM1) {
			team1List.push_back(playerList[i]);
		}
		if (playerList[i]->GetPlayerTeam() == Player::PLAYERTEAM::TEAM2) {
			team2List.push_back(playerList[i]);
		}
	}

	team1List[0]->SetWorldPosition(1.0f, 0.0f, 4.0f);
	team1List[1]->SetWorldPosition(-1.0f, 0.0f, 4.0f);

	team2List[0]->SetWorldPosition(-1.0f, 0.0f, -4.0f);
	team2List[1]->SetWorldPosition(1.0f, 0.0f, -4.0f);

	if (outTeam2 == 2) {
		team1Text->SetText("Winner");
		team1Text->SetColour(glm::vec3(0.0f, 0.0f, 1.0f));
		team1Text->SetVisible(true);

		team2Text->SetText("Loser");
		team2Text->SetColour(glm::vec3(1.0f, 0.0f, 0.0f));
		team2Text->SetVisible(true);
	}
	else if (outTeam1 == 2) {
		team2Text->SetText("Winner");
		team2Text->SetColour(glm::vec3(0.0f, 0.0f, 1.0f));
		team2Text->SetVisible(true);

		team1Text->SetText("Loser");
		team1Text->SetColour(glm::vec3(1.0f, 0.0f, 0.0f));
		team1Text->SetVisible(true);
	}
	else {
		team2Text->SetText("Loser");
		team2Text->SetColour(glm::vec3(1.0f, 0.0f, 0.0f));
		team2Text->SetVisible(true);

		team1Text->SetText("Loser");
		team1Text->SetColour(glm::vec3(1.0f, 0.0f, 0.0f));
		team1Text->SetVisible(true);
	}

	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->GetCameraList()[0]->Position = glm::vec3(0.0f, 0.6f, 1.0f);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->GetCameraList()[0]->SetRotationY(90.0f);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->GetCameraList()[0]->SetRotationX(0.0f);

	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->GetCameraList()[1]->Position = glm::vec3(0.0f, 0.6f, -1.0f);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->GetCameraList()[1]->SetRotationY(-90.0f);
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->GetCameraList()[1]->SetRotationX(0.0f);

	endGame = true;
	canContinue = true;
}

// Reset all players and match variables
void GameManager::Reset()
{
	p1Widget->SetVisible(false);
	p2Widget->SetVisible(false);
	p3Widget->SetVisible(false);
	p4Widget->SetVisible(false);

	divider->SetVisible(false);
	end->SetVisible(false);

	announcer30 = false;

	team1Text->SetVisible(false);
	team2Text->SetVisible(false);
	roundTimer.refeshCD();
	endGameCD.refeshCD();
	voiceCD.refeshCD();
	time = "90";
	roundTimerText->SetVisible(false);
	teamOut = false;
	timeUp = false;
	gameOver = false;
	outTeam1 = 0;
	outTeam2 = 0;
	player1voice = false;
	player2voice = false;
	player3voice = false;
	player4voice = false;
	canContinue = false;
	endGame = false;

	Mix_HaltChannel(playerList[0]->dialogue.channel);
	Mix_HaltChannel(playerList[1]->dialogue.channel);
	Mix_HaltChannel(playerList[2]->dialogue.channel);
	Mix_HaltChannel(playerList[3]->dialogue.channel);

	vector<Player*> team1;
	vector<Player*> team2;

	for (unsigned int i = 0; i < playerList.size(); ++i) {
		playerList[i]->canRender = true;
		playerList[i]->SetIsOut(false);
		playerList[i]->SetCanMove(false);

		if (playerList[i]->GetPlayerNumber() == Player::PLAYERNUMBER::PLAYER1)
		{
			playerList[i]->SetWorldPosition(-3.0f, 0.0f, 4.0f);
		}
		if (playerList[i]->GetPlayerNumber() == Player::PLAYERNUMBER::PLAYER2)
		{
			playerList[i]->SetWorldPosition(3.0f, 0.0f, 4.0f);
		}
		if (playerList[i]->GetPlayerNumber() == Player::PLAYERNUMBER::PLAYER3)
		{
			playerList[i]->SetWorldPosition(3.0f, 0.0f, -4.0f);
		}
		if (playerList[i]->GetPlayerNumber() == Player::PLAYERNUMBER::PLAYER4)
		{
			playerList[i]->SetWorldPosition(-3.0f, 0.0f, -4.0f);
		}

		playerList[i]->SetStats();
	}
}