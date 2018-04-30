#include "CharacterSelectScene.h"

using namespace GAME;

CharacterSelectScene::CharacterSelectScene()
{

}

CharacterSelectScene::~CharacterSelectScene()
{
	bgm->Stop();
	sceneManager = nullptr;
}

bool CharacterSelectScene::Initialize()
{
	// Make reference to the scene manager
	sceneManager = BFEngine::GetInstance()->GetSceneManager();

	// Set screen options
	sceneManager->EnableSplitscreen(false);
	sceneManager->ShowFPS(false);
	sceneManager->CaptureMouse(true);

	// Load shaders
	skyboxShader = new Shader("Shaders/skybox.vs", "Shaders/skybox.fs");

	// Put shaders into shader manager
	skyboxShaderHandle = sceneManager->GetRenderer()->GetShaderManager()->put(std::string("skybox"), skyboxShader);

	// Make skybox, load its textures, set properties, and give to the renderer
	skybox = new Skybox();
	std::vector<char*> faces;
	faces.push_back("Resources/Textures/Skyboxes/Select/front.png");
	faces.push_back("Resources/Textures/Skyboxes/Select/front.png");
	faces.push_back("Resources/Textures/Skyboxes/Select/front.png");
	faces.push_back("Resources/Textures/Skyboxes/Select/front.png");
	faces.push_back("Resources/Textures/Skyboxes/Select/front.png");
	faces.push_back("Resources/Textures/Skyboxes/Select/front.png");
	skybox->LoadTextures(faces);
	skybox->SetShader(skyboxShaderHandle);

	// Music
	bgm = new Music();
	if (!bgm->Load("Resources/Audio/20XX Theme Menu.mp3")) {
		std::cout << "BGM could not load" << std::endl;
	}
	bgm->Play(-1);
	bgm->SetVolume(100);

	// UI
	titleText = new TextUI();
	titleText->SetFont("Resources/Fonts/ka1.ttf");
	titleText->SetText("CHAMPION SELECT");
	titleText->SetColour(1.0, 1.0f, 1.0f);
	titleText->SetSize(0.8f);
	titleText->SetSpacing(9.0f);
	titleText->SetPosition(50, 100);

	crosshair1 = new Crosshair("Resources/Textures/CharacterSelectScreen/crosshair.png", 200, 300);
	crosshair1->GetImage()->SetName("crosshair1");
	crosshair1->GetPlayerInput()->setPlayerNum(0);

	if (Settings::getInstance()->networkedGame && !Settings::getInstance()->isServer) {
		crosshair1->GetPlayerInput()->makeNetworked();
		
	}
	else {
		//player1->GetPlayerInput()->AddAnyController();
	}
	crosshairList.push_back(crosshair1);

	crosshair2 = new Crosshair("Resources/Textures/CharacterSelectScreen/crosshairP2.png", 200, 900);
	crosshair2->GetImage()->SetName("crosshair2");
	crosshair2->GetPlayerInput()->setPlayerNum(1);
	//crosshair2->GetPlayerInput()->AddAnyController();
	if (Settings::getInstance()->networkedGame && !Settings::getInstance()->isServer) {
		crosshair2->GetPlayerInput()->makeNetworked();
		
	}
	else {
		//player2->GetPlayerInput()->AddAnyController();
	}
	crosshairList.push_back(crosshair2);

	crosshair3 = new Crosshair("Resources/Textures/CharacterSelectScreen/crosshairP3.png", 1700, 300);
	crosshair3->GetImage()->SetName("crosshair3");
	crosshair3->GetPlayerInput()->setPlayerNum(2);
	//crosshair3->GetPlayerInput()->AddAnyController();
	if (Settings::getInstance()->networkedGame && ((Settings::getInstance()->isServer && !Settings::getInstance()->spectatorMode) || (!Settings::getInstance()->isServer && Settings::getInstance()->spectatorMode))) {
		crosshair3->GetPlayerInput()->makeNetworked();
		
	}
	else {
		//player3->GetPlayerInput()->AddAnyController();
	}
	crosshairList.push_back(crosshair3);

	crosshair4 = new Crosshair("Resources/Textures/CharacterSelectScreen/crosshairP4.png", 1700, 900);
	crosshair4->GetImage()->SetName("crosshair4");
	crosshair4->GetPlayerInput()->setPlayerNum(3);
	//crosshair4->GetPlayerInput()->AddAnyController();
	if (Settings::getInstance()->networkedGame && ((Settings::getInstance()->isServer && !Settings::getInstance()->spectatorMode) || (!Settings::getInstance()->isServer && Settings::getInstance()->spectatorMode))) {
		crosshair4->GetPlayerInput()->makeNetworked();
		
	}
	else {
		//player4->GetPlayerInput()->AddAnyController();
	}
	crosshairList.push_back(crosshair4);

	player1Back = new ImageUI();
	player1Back->SetImage("Resources/Textures/CharacterSelectScreen/blueBack.png");
	player1Back->SetName("Player1Back");
	player1Back->SetPosition(200, 300);
	player1Back->SetHeight(250);
	player1Back->SetWidth(250);
	player1Port = new PlayerPortrait(200, 300, 300, 460);

	player2Back = new ImageUI();
	player2Back->SetImage("Resources/Textures/CharacterSelectScreen/blueBack.png");
	player2Back->SetName("Player2Back");
	player2Back->SetPosition(200, 900);
	player2Back->SetHeight(250);
	player2Back->SetWidth(250);
	player2Port = new PlayerPortrait(200, 900, 300, 740);

	player3Back = new ImageUI();
	player3Back->SetImage("Resources/Textures/CharacterSelectScreen/redBack.png");
	player3Back->SetName("Player3Back");
	player3Back->SetPosition(1700, 300);
	player3Back->SetHeight(250);
	player3Back->SetWidth(250);
	player3Port = new PlayerPortrait(1700, 300, 1600, 460);

	player4Back = new ImageUI();
	player4Back->SetImage("Resources/Textures/CharacterSelectScreen/redBack.png");
	player4Back->SetName("Player4Back");
	player4Back->SetPosition(1700, 900);
	player4Back->SetHeight(250);
	player4Back->SetWidth(250);
	player4Port = new PlayerPortrait(1700, 900, 1600, 740);

	champAlexTrix = new ButtonUI("Resources/Textures/CharacterSelectScreen/Alex_Trix.png");
	champAlexTrix->GetImage()->SetName("Alex_Trix");
	champAlexTrix->SetPosition(760, 780);
	champAlexTrix->SetHeight(180);
	champAlexTrix->SetWidth(180);

	champFlintDamascus = new ButtonUI("Resources/Textures/CharacterSelectScreen/Flint_Damascus.png");
	champFlintDamascus->GetImage()->SetName("Flint_Damascus");
	champFlintDamascus->SetPosition(760, 420);
	champFlintDamascus->SetHeight(180);
	champFlintDamascus->SetWidth(180);

	champJackCole = new ButtonUI("Resources/Textures/CharacterSelectScreen/Jack_Cole.png");
	champJackCole->GetImage()->SetName("Jack_Cole");
	champJackCole->SetPosition(1120, 780);
	champJackCole->SetHeight(180);
	champJackCole->SetWidth(180);

	champKalOrr = new ButtonUI("Resources/Textures/CharacterSelectScreen/Kal_Orr.png");
	champKalOrr->GetImage()->SetName("Kal_Orr");
	champKalOrr->SetPosition(940, 600);
	champKalOrr->SetHeight(180);
	champKalOrr->SetWidth(180);

	champOkiCaeli = new ButtonUI("Resources/Textures/CharacterSelectScreen/Oki_Caeli.png");
	champOkiCaeli->GetImage()->SetName("Oki_Caeli");
	champOkiCaeli->SetPosition(1120, 420);
	champOkiCaeli->SetHeight(180);
	champOkiCaeli->SetWidth(180);

	champMystery = new ImageUI();
	champMystery->SetImage("Resources/Textures/CharacterSelectScreen/Mystery_Champ2.png");
	champMystery->SetName("Mystery_Champ");
	champMystery->SetPosition(940, 240);
	champMystery->SetHeight(180);
	champMystery->SetWidth(180);

	champMystery2 = new ImageUI();
	champMystery2->SetImage("Resources/Textures/CharacterSelectScreen/Mystery_Champ2.png");
	champMystery2->SetName("Mystery_Champ2");
	champMystery2->SetPosition(940, 960);
	champMystery2->SetHeight(180);
	champMystery2->SetWidth(180);

	champMystery3 = new ImageUI();
	champMystery3->SetImage("Resources/Textures/CharacterSelectScreen/Mystery_Champ2.png");
	champMystery3->SetName("Mystery_Champ2");
	champMystery3->SetPosition(1300, 600);
	champMystery3->SetHeight(180);
	champMystery3->SetWidth(180);

	champMystery4 = new ImageUI();
	champMystery4->SetImage("Resources/Textures/CharacterSelectScreen/Mystery_Champ2.png");
	champMystery4->SetName("Mystery_Champ2");
	champMystery4->SetPosition(580, 600);
	champMystery4->SetHeight(180);
	champMystery4->SetWidth(180);

	blueJoiner = new ImageUI();
	blueJoiner->SetImage("Resources/Textures/CharacterSelectScreen/greyJoin.png");
	blueJoiner->SetName("Blue_Joiner");
	blueJoiner->SetPosition(100, 600);
	blueJoiner->SetHeight(349);
	blueJoiner->SetWidth(152);

	redJoiner = new ImageUI();
	redJoiner->SetImage("Resources/Textures/CharacterSelectScreen/greyJoin2.png");
	redJoiner->SetName("Red_Joiner");
	redJoiner->SetPosition(1800, 600);
	redJoiner->SetHeight(349);
	redJoiner->SetWidth(152);

	portraitList.push_back(player1Port);
	portraitList.push_back(player2Port);
	portraitList.push_back(player3Port);
	portraitList.push_back(player4Port);


	//add objects to relevant scene lists crosshair1
	{
	AddUIObject(titleText);
	AddUIObject(blueJoiner);
	AddUIObject(redJoiner);

	////
	//Setup player portraits and text boxes here
	AddUIObject(player1Back);
	SetUpPlayerPorts(player1Port);

	AddUIObject(player2Back);
	SetUpPlayerPorts(player2Port);

	AddUIObject(player3Back);
	SetUpPlayerPorts(player3Port);

	AddUIObject(player4Back);
	SetUpPlayerPorts(player4Port);
	//setup end
	/////

	AddUIObject(champAlexTrix);
	AddUIObject(champFlintDamascus);
	AddUIObject(champJackCole);
	AddUIObject(champKalOrr);
	AddUIObject(champOkiCaeli);

	AddUIObject(champMystery);
	AddUIObject(champMystery2);
	AddUIObject(champMystery3);
	AddUIObject(champMystery4);
	}

	start = new ImageUI;
	start->SetImage("Resources/Textures/start.png");
	start->SetPosition(960.0f, 540.0f);
	start->SetScale(1.0f);
	start->SetVisible(false);
	AddUIObject(start);

	// Loading Screen
	loadingScreen = new ImageUI();
	loadingScreen->SetImage("Resources/Textures/Loading.png");
	loadingScreen->SetPosition(sceneManager->GetScreenWidth() / 2, sceneManager->GetScreenHeight() / 2);
	loadingScreen->SetVisible(false);

	AddUIObject(loadingScreen);

	loadingCD = Cooldown(3.0f);
	cameraTimer = 0.4f;

	//Audio
	announcer = new Announcer;
	announcer->LoadDialogue("Resources/Audio/AnnouncerAudio.txt");
	announcer->playSpecifiedFromState(Announcer::CharacterSelect, 3);
	announcer->channel = -1;

	//everything loaded fine
	return true;
}

void CharacterSelectScene::FixedUpdate(const float deltaTime)
{
	cameraTimer -= deltaTime;
	if (cameraTimer <= 0) {
		cameraTimer = 0.4f;
		if (!sceneManager->IsSplitScreen()) {
			sceneManager->EnableSplitscreen(true);
		}
		else
		{
			sceneManager->EnableSplitscreen(false);
		}
	}

	//rotate portrait backgrounds
	portBackRot += deltaTime * 2;
	player1Back->SetRotationAngle(portBackRot);
	player2Back->SetRotationAngle(portBackRot);
	player3Back->SetRotationAngle(-portBackRot);
	player4Back->SetRotationAngle(-portBackRot);
}

void CharacterSelectScene::Update(const float deltaTime)
{
	//Update Crosshair Movement
	for (int i = 0; i < crosshairList.size(); i++)
	{
		crosshairList.at(i)->Update(deltaTime);
	}

	//update portraits and textboxes for alpha
	for (int i = 0; i < portraitList.size(); i++)
	{
		portraitList.at(i)->Update(deltaTime);
	}

	//check to see that all players have selected
	if (loading == false &&
		crosshair1->GetCharSelected() != PlayerPortrait::CHARTYPE::NONE &&
		crosshair2->GetCharSelected() != PlayerPortrait::CHARTYPE::NONE &&
		crosshair3->GetCharSelected() != PlayerPortrait::CHARTYPE::NONE &&
		crosshair4->GetCharSelected() != PlayerPortrait::CHARTYPE::NONE)
	{
		ready = true;
		sceneManager->GetRenderer()->EnableInvertedColours(true);
		start->SetVisible(true);
	}
	else {
		ready = false;
		sceneManager->GetRenderer()->EnableInvertedColours(false);
		start->SetVisible(false);
	}



	//check for networked inputs and pass them to the appropriate player
	if (Settings::getInstance()->networkedGame && !loading) {
		if (Settings::getInstance()->isServer) {
			if (!Settings::getInstance()->spectatorMode) {
				std::string dataChunk = crosshair1->GetPlayerInput()->UpdateJoystickState() + crosshair2->GetPlayerInput()->UpdateJoystickState();
				//std::cout << dataChunk << std::endl;
				BFEngine::GetInstance()->sendData(dataChunk);
			}
			else {
				std::string dataChunk = crosshair1->GetPlayerInput()->UpdateJoystickState() + crosshair2->GetPlayerInput()->UpdateJoystickState() + crosshair3->GetPlayerInput()->UpdateJoystickState() + crosshair4->GetPlayerInput()->UpdateJoystickState();
				//std::cout << dataChunk << std::endl;
				BFEngine::GetInstance()->sendData(dataChunk);
			}
		}
		else if (!Settings::getInstance()->spectatorMode) {
			std::string dataChunk = crosshair3->GetPlayerInput()->UpdateJoystickState() + crosshair4->GetPlayerInput()->UpdateJoystickState();
			//std::cout << dataChunk << std::endl;
			BFEngine::GetInstance()->sendData(dataChunk);
		}
		//we don't send data as the spectator client

		if (Settings::getInstance()->isServer && Settings::getInstance()->spectatorMode) {
			//skip receiving since they're not sending data back
		}
		else {
			//read input from the other game
			std::string externalInput = BFEngine::GetInstance()->receiveData();
			//cout << "first set of data received: " << externalInput << endl;
			if (externalInput != "") {
				if (externalInput == "move" && !loading) {
					loadingCD.startCD();
					loading = true;
					loadingScreen->SetVisible(true);
					sceneManager->GetRenderer()->EnableInvertedColours(false);
					bgm->Stop();
					//announcer->playSpecifiedFromState(Announcer::CharacterSelect, 5);
				}
				else {
					if (externalInput.length() >= 70) {
						//break it into components
						std::string input1 = externalInput.substr(0, 35);
						std::string input2 = externalInput.substr(35, 35);
						//check first string
						std::string playerNum = input1.substr(0, 1);
						if (playerNum == "0" || playerNum == "1" || playerNum == "2" || playerNum == "3") {
							//cout << "PlayerNum: " << playerNum << std::endl;
							crosshairList.at(std::stoi(playerNum))->GetPlayerInput()->ParseNetworkInputs(input1);
						}
						playerNum = input2.substr(0, 1);
						if (playerNum == "0" || playerNum == "1" || playerNum == "2" || playerNum == "3") {
							//cout << "PlayerNum: " << playerNum << std::endl;
							crosshairList.at(std::stoi(playerNum))->GetPlayerInput()->ParseNetworkInputs(input2);
						}
					}
					//if spectator mode is on we'll have more controllers to parse
					if (Settings::getInstance()->spectatorMode && externalInput.length() >= 140) {
						std::string input1 = externalInput.substr(70, 35);
						std::string input2 = externalInput.substr(105, 35);
						//check first string
						std::string playerNum = input1.substr(0, 1);
						if (playerNum == "0" || playerNum == "1" || playerNum == "2" || playerNum == "3") {
							//cout << "PlayerNum: " << playerNum << std::endl;
							crosshairList.at(std::stoi(playerNum))->GetPlayerInput()->ParseNetworkInputs(input1);
						}
						playerNum = input2.substr(0, 1);
						if (playerNum == "0" || playerNum == "1" || playerNum == "2" || playerNum == "3") {
							//cout << "PlayerNum: " << playerNum << std::endl;
							crosshairList.at(std::stoi(playerNum))->GetPlayerInput()->ParseNetworkInputs(input2);
						}
					}
				}
			}
		}
	}

	//handles buttons
	for (int i = 0; i < crosshairList.size(); i++)
	{
		crosshairList.at(i)->GetPlayerInput()->DebugState();
		HandleNetworkEvents(i);
	}

	//moved below to help
	if (!loadingCD.checkOffCD() && loadingCD.secondsLeft() <= 1)
	{
		sceneManager->SwitchScene(new TvTGameScene());
	}
}

void CharacterSelectScene::HandleEvents(SDL_Event events)
{
	///BANNED FROM USE EXCEPT FOR LOCAL CONTROLLER THINGIES
	if (events.type == SDL_JOYBUTTONDOWN) {

		if (events.jbutton.button == 7) //Start button
		{
			if (!ready) {
				bool canSelect = true;

				if (usedCrosshairList.size() != 0)
				{
					for (int i = 0; i < usedCrosshairList.size(); i++)
					{
						if (events.jbutton.which == usedCrosshairList.at(i)) {
							canSelect = false;
						}
					}
				}

				if (canSelect) {
					//create new controller
					ControllerPressStart(events.jbutton.which);
				}
			}
		}
	}

	//starts cpu only mode
	if (events.type == SDL_KEYDOWN && events.key.repeat == 0)
	{
		if (events.key.keysym.sym == SDLK_c) {
			for (int i = 0; i < 4; i++) {
				int randNum = Clock::GetInstance()->generateRandomNumber();
				int randomPlayer = 0;
				if (randNum < 2) {
					randomPlayer = 0;
				}
				else if (randNum < 4) {
					randomPlayer = 1;
				}
				else if (randNum < 6) {
					randomPlayer = 2;
				}
				else if (randNum < 8) {
					randomPlayer = 3;
				}
				else {
					randomPlayer = 4;
				}
				sceneManager->saveData.push_back(randomPlayer);
			}
			sceneManager->justCPUS = true;
			loadingCD.startCD();
			loading = true;
			loadingScreen->SetVisible(true);
			sceneManager->GetRenderer()->EnableInvertedColours(false);
			bgm->Stop();
			announcer->playSpecifiedFromState(Announcer::CharacterSelect, 5);
		}
	}
}

void CharacterSelectScene::HandleNetworkEvents(int i) //they're also local events now
{
	if (!loading) {
		//first we check where the player is
		if (champAlexTrix->OnHover(crosshairList.at(i)->GetImage()))
		{
			//set player portrait to Alex Type (this sets the alpha portrait and text to visible)
			portraitList.at(i)->SetCharType(PlayerPortrait::CHARTYPE::ALEX);
		}
		else if (champFlintDamascus->OnHover(crosshairList.at(i)->GetImage()))
		{
			portraitList.at(i)->SetCharType(PlayerPortrait::CHARTYPE::FLINT);
		}
		else if (champKalOrr->OnHover(crosshairList.at(i)->GetImage()))
		{
			portraitList.at(i)->SetCharType(PlayerPortrait::CHARTYPE::KAL);
		}
		else if (champJackCole->OnHover(crosshairList.at(i)->GetImage()))
		{
			portraitList.at(i)->SetCharType(PlayerPortrait::CHARTYPE::JACK);
		}
		else if (champOkiCaeli->OnHover(crosshairList.at(i)->GetImage()))
		{
			portraitList.at(i)->SetCharType(PlayerPortrait::CHARTYPE::OKI);
		}
		else
		{
			//if not hovering over any champions, update portrait and text to show nothing and update chartype
			portraitList.at(i)->SetCharType(PlayerPortrait::CHARTYPE::NONE);
		}
		int buttonPressed = 7;
		if (crosshairList.at(i)->GetPlayerInput()->NewButtonPress(buttonPressed)) {

			if (crosshairList.at(i)->GetPlayerInput()->controllerState.at(buttonPressed) == 1 && portraitList.at(i)->GetCharType() != PlayerPortrait::CHARTYPE::NONE) { // a button


				if (portraitList.at(i)->GetCharType() == PlayerPortrait::CHARTYPE::ALEX)
				{
					announcer->playRandomFromOtherState(Announcer::CharSelectAlex, true);
				}
				else if (portraitList.at(i)->GetCharType() == PlayerPortrait::CHARTYPE::FLINT)
				{
					announcer->playRandomFromOtherState(Announcer::CharSelectFlint, true);
				}
				else if (portraitList.at(i)->GetCharType() == PlayerPortrait::CHARTYPE::KAL)
				{
					announcer->playRandomFromOtherState(Announcer::CharSelectKal, true);
				}
				else if (portraitList.at(i)->GetCharType() == PlayerPortrait::CHARTYPE::JACK)
				{
					announcer->playRandomFromOtherState(Announcer::CharSelectJack, true);
				}
				else if (portraitList.at(i)->GetCharType() == PlayerPortrait::CHARTYPE::OKI)
				{
					announcer->playRandomFromOtherState(Announcer::CharSelectOki, true);
				}
				crosshairList.at(i)->SetCanMove(false);
				crosshairList.at(i)->SetVisible(false);
				crosshairList.at(i)->SetCharSelected(portraitList.at(i)->GetCharType());
			}
		}
		buttonPressed = 8;
		if (crosshairList.at(i)->GetPlayerInput()->NewButtonPress(buttonPressed)) {
			if (crosshairList.at(i)->GetPlayerInput()->controllerState.at(buttonPressed) == 1 && portraitList.at(i)->GetCharType() != PlayerPortrait::CHARTYPE::NONE) { // b button

																																										//if crosshair pressed B, allow movement, set visibility of crosshair to true, set chartype to nothing since its unselected
				crosshairList.at(i)->SetCanMove(true);
				crosshairList.at(i)->SetVisible(true);
				crosshairList.at(i)->SetCharSelected(PlayerPortrait::CHARTYPE::NONE);
			}
		}


		buttonPressed = 10; //y i think
		if (crosshairList.at(i)->GetPlayerInput()->NewButtonPress(buttonPressed) && Settings::getInstance()->replaySystemEnabled) { //play replay
																																	//force char select through with through with cpus
			MoveToGameScene();
		}


		buttonPressed = 14;
		if (crosshairList.at(i)->GetPlayerInput()->NewButtonPress(buttonPressed)) {
			if (crosshairList.at(i)->GetPlayerInput()->controllerState.at(buttonPressed) == 1) {
				if (ready) {
					MoveToGameScene();
				}
				else
				{
					//this doesn't do anything anymore
					if (!ready) {
						if (Settings::getInstance()->networkedGame && crosshairList.at(i)->GetPlayerInput()->isNetworked() && !crosshairList.at(i)->GetEnabled()) {
							crosshairList.at(i)->SetCanMove(true);
							crosshairList.at(i)->SetEnabled(true);
							AddUIObject(crosshairList.at(i)->GetImage());
						}
					}
				}
			}
		}

		buttonPressed = 9; //x i think
		if (crosshairList.at(i)->GetPlayerInput()->NewButtonPress(buttonPressed) && Settings::getInstance()->replaySystemEnabled) { //play replay
			loadingCD.startCD();
			loading = true;
			loadingScreen->SetVisible(true);
			sceneManager->GetRenderer()->EnableInvertedColours(false);
			bgm->Stop();
			Settings::getInstance()->PlayReplay();
		}
	}
}

void CharacterSelectScene::HandleStates(const Uint8 *state)
{
	if (state[SDL_SCANCODE_P]) {
		sceneManager->SwitchScene(new TvTGameScene());
	}

	if (state[SDL_SCANCODE_Z]) {
		InputManager::GetInstance()->ClearControllers();
		InputManager::GetInstance()->initalizeControllers();
		sceneManager->controllers.clear();
		sceneManager->saveData.clear();
		sceneManager->SwitchScene(new CharacterSelectScene());
	}
}

void CharacterSelectScene::ControllerPressStart(SDL_JoystickID jID)
{
	// Bind Controller
	for (int i = 0; i < crosshairList.size(); i++)
	{
		if (crosshairList.at(i)->GetEnabled() == true)
		{
			continue;
		}

		crosshairList.at(i)->GetPlayerInput()->CheckForController(jID);
		crosshairList.at(i)->SetCanMove(true);
		crosshairList.at(i)->SetEnabled(true);
		usedCrosshairList.push_back(jID);
		AddUIObject(crosshairList.at(i)->GetImage());
		break;
	}
}

void CharacterSelectScene::SetUpPlayerPorts(PlayerPortrait* tempP)
{
	std::vector<ImageUI*> imgList;

	imgList = tempP->GetImages();

	for (int i = 0; i < imgList.size(); i++)
	{
		AddUIObject(imgList.at(i));
	}
}

void CharacterSelectScene::MoveToGameScene() {
	for (int i = 0; i < crosshairList.size(); i++) {
		sceneManager->controllers.push_back(crosshairList.at(i)->GetPlayerInput()->GetJoystick());
		if (crosshairList.at(i)->GetCharSelected() == PlayerPortrait::CHARTYPE::ALEX)
		{
			sceneManager->saveData.push_back(0);
		}
		else if (crosshairList.at(i)->GetCharSelected() == PlayerPortrait::CHARTYPE::FLINT)
		{
			sceneManager->saveData.push_back(1);
		}
		else if (crosshairList.at(i)->GetCharSelected() == PlayerPortrait::CHARTYPE::JACK)
		{
			sceneManager->saveData.push_back(2);
		}
		else if (crosshairList.at(i)->GetCharSelected() == PlayerPortrait::CHARTYPE::KAL)
		{
			sceneManager->saveData.push_back(3);
		}
		else if (crosshairList.at(i)->GetCharSelected() == PlayerPortrait::CHARTYPE::OKI)
		{
			sceneManager->saveData.push_back(4);
		}
		else
		{
			sceneManager->saveData.push_back(0);
		}
	}
	if (Settings::getInstance()->networkedGame && Settings::getInstance()->isServer) {
		std::string dataChunk = "move";
		BFEngine::GetInstance()->sendData(dataChunk);
	}
	loadingCD.startCD();
	loading = true;
	loadingScreen->SetVisible(true);
	sceneManager->GetRenderer()->EnableInvertedColours(false);
	bgm->Stop();
	announcer->playSpecifiedFromState(Announcer::CharacterSelect, 5);
}