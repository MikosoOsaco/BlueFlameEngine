#include "Player.h"

using namespace GAME;

Player::Player()
{
	PlayingIntro = true;
	///AI stuff
	computerPlayer = false;
	CPUMovementCD = Cooldown(0);
	CPUShootingCD = Cooldown(0);
	rand = Randomizer();
	///AI stuff ends

	// Make player parts
	base = new PlayerBase("Resources/Models/Robot_Base_Greybox/Robot_Var_002_Gurran.obj");
	base->SetWorldPosition(0.0f, 0.0f, 0.0f);
	base->SetWorldScale(0.012f);
	base->renderComponent->SetColour(0.2f, 0.7f, 0.0f);

	ring = new PlayerBase();
	ring->SetWorldPosition(0.0f, -0.3f, 0.0f);
	ring->SetWorldScale(0.5f, 0.2f, 0.5f);
	ring->renderComponent->SetColour(0.2f, 0.0f, 0.7f);

	shield = new PlayerBase();
	shield->SetWorldPosition(0.0f, 0.3f, -0.5f);
	shield->SetWorldScale(1.0f, 1.0f, 0.05f);
	shield->renderComponent->SetColour(0.0f, 0.2f, 0.7f);
	shield->renderComponent->CanRender(false);

	// Make player target marker
	marker = new PlayerBase();
	marker->SetWorldPosition(0.0f, 0.0f, 0.0f);
	marker->SetWorldScale(0.2f, 0.3f, 0.2f);
	marker->renderComponent->SetColour(1.0f, 0.0f, 0.0f);

	// Initialize collision component
	collisionComponent = new CollisionComponent();
	collisionComponent->CreateCollisionVolume(CollisionComponent::Collision_Type::BOX, base->renderComponent->getVertexList());
	collisionComponent->SetBoxPadding(glm::vec3(1.0f, 1.2f, 1.0f));

	// Initialize physics componenet
	physicsComponent = new PhysicsComponent();
	physicsComponent->SetAcceleration(glm::vec3(0.0f, -25.0f, 0.0f));
	physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
	physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::VERY_ELASTIC);
	physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::PERFECT_SMOOTH);
	physicsComponent->SetMass(50.0f);

	// Set up shader
	projectileShader = new Shader("Shaders/model.vs", "Shaders/model.fs");
	pShader = BFEngine::GetInstance()->GetSceneManager()->GetRenderer()->GetShaderManager()->put(std::string("projectile"), projectileShader);

	// Set direction to team 1 orientation by default
	dir = -1;

	// Set player to target another enemy by default
	isTargeting = false;

	// Set cooldowns
	movementCD = Cooldown(0.0);
	shieldCD = Cooldown(2.0);
	healthIncrementCD = Cooldown(0.1);

	// Set game gamplay status
	out = false;
	canMove = true;

	// Make player input
	playerInput = new PlayerInput();

	// Particles
	stunEffect = new ParticleSystem(BFEngine::GetInstance()->GetSceneManager()->GetRenderer()->GetShaderManager(), glm::vec3(1.0f, 0.5f, 1.0f));
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(stunEffect);

	// Set player stats
	SetStats();
}

Player::~Player()
{
	// TODO: delete everything
}

void Player::AddProjecitleManager(ProjectileManager* pM)
{
	projectileManager = pM;
}

void Player::Update(const float deltaTime)
{

	if (specialMeter > 100) {
		specialMeter = 100;
	}

	if (shieldCD.checkOffCD() && healthIncrementCD.checkOffCD()) {
		if (shieldHealth < 100 && playerState != BLOCK) {
			shieldHealth++;
		}
		healthIncrementCD.startCD();
	}

	if (shieldHealth < 0) {
		shieldHealth = 0;
	}
	// Target Direction (only used for cpu players
	if (computerPlayer) {
		if (isTargeting) {
			if (!enemyTeam.at(currentTarget)->out) {
				targetedPlayer = enemyTeam.at(currentTarget)->worldPosition;
				targetAngle = -glm::atan((targetedPlayer.x - worldPosition.x) / (targetedPlayer.z - worldPosition.z));
			}
			else if (enemyTeam.at(currentTarget)->out) {
				SwitchTarget();
			}
		}
		else if (!isTargeting) {
			targetedPlayer = glm::vec3(worldPosition);
			targetedPlayer.z += dir;
			targetAngle = -glm::atan((targetedPlayer.x - worldPosition.x) / (targetedPlayer.z - worldPosition.z));
		}
	}
	

	// UPDATE TIMERS
	//
	// Update stun timer
	if (playerState == STUN) {
		stunTimer -= deltaTime;
		if (stunTimer <= 0) {
			stunTimer = 0;
			playerState = NORMAL;
		}
	}

	lightComboTimer -= deltaTime;
	if (lightComboTimer <= 0) {
		lightComboTimer = 0;
		lightComboPosition = 0;
	}

	mediumComboTimer -= deltaTime;
	if (mediumComboTimer <= 0) {
		mediumComboTimer = 0;
		mediumComboPosition = 0;
	}

	heavyComboTimer -= deltaTime;
	if (heavyComboTimer <= 0) {
		heavyComboTimer = 0;
		heavyComboPosition = 0;
	}

	specialComboTimer -= deltaTime;
	if (specialComboTimer <= 0) {
		specialComboTimer = 0;
		specialComboPosition = 0;
	}
	//

	//passive dialogue block

	switch (playerState)
	{
	case GAME::Player::NORMAL:
		dialogue.setDialogueState(dialogue.Idle);
		break;
	case GAME::Player::ATTACK:
		dialogue.setDialogueState(dialogue.Idle);
		break;
	case GAME::Player::BLOCK:
		dialogue.setDialogueState(dialogue.TakingDamage);
		break;
	case GAME::Player::STUN:
		dialogue.setDialogueState(dialogue.TakingDamage);
		break;
	case GAME::Player::JUMP:
		dialogue.setDialogueState(dialogue.Moving);
		break;
	case GAME::Player::DODGE:
		break;
	case GAME::Player::DEAD:
		break;
	default:
		break;
	}
	if (!out) {
		dialogue.playIdle(); //will play an idle sound if sound hasn't been played recently
	}
}

void Player::FixedUpdate(const float deltaTime) {

	if (!(Settings::getInstance()->networkedGame && Settings::getInstance()->spectatorMode && !Settings::getInstance()->isServer) && !computerPlayer && (!Settings::getInstance()->replaySystemEnabled || !Settings::getInstance()->playingReplay)) {
		playerInput->ParseNetworkInputs(playerInput->UpdateJoystickState());
	}
	else if (computerPlayer) {
		AIMovement();
		switch (playerTeam)
		{
		case GAME::Player::TEAM1:
			BoundsCheckT1();
			break;
		case GAME::Player::TEAM2:
			BoundsCheckT2();
			break;
		case GAME::Player::TEAM0:
			break;
		default:
			break;
		}

		AIShooting();
		//playerInput->DebugState();
	}



	ring->UpdateState();
	shield->UpdateState();
	marker->UpdateState();

	shield->SetWorldScale(shieldHealth / 100.0f, shieldHealth / 100.0f, 0.05f);

	// Update collision and physics
	physicsComponent->Update(deltaTime);
	SetWorldPosition(physicsComponent->GetPosition());
	collisionComponent->Update(GetWorldPosition());

	if (playerState != STUN) {
		physicsComponent->SetVelocity(glm::vec3(0.0f, physicsComponent->GetVelocity().y, 0.0f));
	}

	// Update player model
	UpdateModel(deltaTime);
	// If player can move update movement 
	if (canMove && !PlayingIntro) {
		if (playerInput->CheckForController() || playerInput->isNetworked() || computerPlayer || (Settings::getInstance()->replaySystemEnabled || Settings::getInstance()->playingReplay)) {//if they have a controller
			glm::vec2 mods = glm::vec2(playerInput->controllerState.at(1), playerInput->controllerState.at(2));
			glm::vec2 targetMods = glm::vec2(playerInput->controllerState.at(3), playerInput->controllerState.at(4));

			if (playerInput->controllerState.at(9) == 1 || playerInput->controllerState.at(10) == 1) { //if X or Y is pressed (player is blocking)
				if (!out && shieldHealth > 0) {
					Block();
				}
			}
			else {
				StopBlock();
				if (mods.x > 0.01f) {
					Movement(Player::PLAYERMOVEMENT::RIGHT, deltaTime);
				}
				if (mods.x < -0.01f) {
					Movement(Player::PLAYERMOVEMENT::LEFT, deltaTime);
				}
				if (mods.y > 0.01f) {
					Movement(Player::PLAYERMOVEMENT::BACKWARD, deltaTime);
				}
				if (mods.y < -0.01f) {
					Movement(Player::PLAYERMOVEMENT::FORWARD, deltaTime);
				}

			}

			if (playerInput->controllerState.at(7) == 1 || playerInput->controllerState.at(8) == 1) { //if the right trigger is pressed
				if (!out) {
					Jump();
				}
			}
			if (targetMods.x > 0.05f) {
				targetAngle = 0.5f;
			}

			if (targetMods.x < -0.05f) {
				targetAngle = -0.5f;
			}

			if (targetMods.x <= 0.05f && targetMods.x >= -0.05f) {
				targetAngle = 0.0f;
			}

			/*if (targetAngle > 0.5f) {
			targetAngle = 0.5f;
			}
			else if (targetAngle < -0.5f) {
			targetAngle = -0.5f;
			}*/
		}
	}

	// Update rotation
	if (playerTeam == PLAYERTEAM::TEAM1) {
		SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), -targetAngle);
	}
	else if (playerTeam == PLAYERTEAM::TEAM2) {
		SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), 3.14 - targetAngle);
	}
	if (!PlayingIntro) {
		HandleControllerEvents();
	}
	

	// Update function from child
	InheritedUpdate(deltaTime);
}

void Player::HandleEvents(SDL_Event events) ///THIS HAS BEEN BANNED IN ACCORDENCE WITH THE NEW CONTROLLER FUNCTIONALITY. USE HandleControllerEvents instead
{
	// Child events
	InheritedHandleEvents(events); ///we will also need to ban this function, but no one uses it anyways
}

void Player::HandleStates(const Uint8 *state)
{
	InheritedHandleStates(state);
}

void Player::Movement(PLAYERMOVEMENT movement, const float deltaTime)
{
	if (playerState == NORMAL) {

		float moveMod = 1;

		if (worldPosition.y > 0.1f) {
			moveMod = 0.5f;
		}
		if (movement == FORWARD) {
			if (!out) {
				physicsComponent->SetVelocity(glm::vec3(physicsComponent->GetVelocity().x, physicsComponent->GetVelocity().y, (-moveSpeed * moveMod) * deltaTime * 500 * dir));
			}
			base->SetWorldRotation(glm::vec3(1.0f, 0.0f, 0.0f), -0.2f);
		}
		if (movement == BACKWARD) {
			if (!out) {
				physicsComponent->SetVelocity(glm::vec3(physicsComponent->GetVelocity().x, physicsComponent->GetVelocity().y, (moveSpeed * moveMod) * deltaTime * 500 * dir));
			}
			base->SetWorldRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.2f);
		}
		if (movement == RIGHT) {
			if (!out) {
				physicsComponent->SetVelocity(glm::vec3((moveSpeed * moveMod) * deltaTime * 500 * dir, physicsComponent->GetVelocity().y, physicsComponent->GetVelocity().z));
			}
			base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), -0.2f);
		}
		if (movement == LEFT) {
			if (!out) {
				physicsComponent->SetVelocity(glm::vec3((-moveSpeed * moveMod) * deltaTime * 500 * dir, physicsComponent->GetVelocity().y, physicsComponent->GetVelocity().z));
			}
			base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), 0.2f);
		}
	}
}

void Player::UpdateModel(const float deltaTime)
{
	if (playerState != STUN) {
		// Rotate Ring
		ring->SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), ring->GetWorldRotationAngle() + 2.0f * deltaTime);

		// Move up and down
		if (localPosition.y >= 0.05f) {
			down = true;
		}
		else if (localPosition.y <= 0.0f) {
			down = false;
		}

		if (down) {
			SetLocalPosition(GetLocalPosition() - glm::vec3(0.0f, deltaTime / 20, 0.0f));
		}
		else {
			SetLocalPosition(GetLocalPosition() + glm::vec3(0.0f, deltaTime / 20, 0.0f));
		}

		ResetModel();
	}

	if (playerState == STUN) {
		ring->SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), ring->GetWorldRotationAngle() - 10.0f * deltaTime);
		base->SetWorldRotation(glm::vec3(0.0f, 1.0f, 1.0f), base->GetWorldRotationAngle() + 0.3f * deltaTime);
	}

	if (playerState == BLOCK) {
		ring->SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), ring->GetWorldRotationAngle() - 0.3f * deltaTime);
		shield->SetWorldRotation(glm::vec3(0.0f, 0.0f, 1.0f), shield->GetWorldRotationAngle() + 10.0f * deltaTime);
		shield->renderComponent->CanRender(true);
	}
	else {
		shield->renderComponent->CanRender(false);
	}

	marker->SetWorldPosition(targetedPlayer.x, targetedPlayer.y + 1.5f, targetedPlayer.z);
}

void Player::ResetModel() {
	// Set rotations back to normal
	base->SetLocalRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
	base->SetWorldRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
}

void Player::SetStats() {
	shieldHealth = maxHealth;
	moveSpeed = 2.0f;
	specialMeter = 0;
}

void Player::SetEnemyTeam(Player* player1, Player* player2) {
	enemyTeam = { player1, player2 };
	SetTarget(player1->GetWorldPosition());
}

void Player::SetTarget(glm::vec3 targetPlayer) {
	targetedPlayer = targetPlayer;
}

void Player::SetTarget(int target) {
	currentTarget = target;
}

void Player::SetTargetColour(glm::vec3 colour) {
	targetColour = colour;
	marker->renderComponent->SetColour(targetColour.x, targetColour.y, targetColour.z);
}

void Player::SetPlayerTeam(PLAYERTEAM pT) {
	playerTeam = pT;
	if (playerTeam == PLAYERTEAM::TEAM1) {
		dir = 1;
	}
	else if (playerTeam == PLAYERTEAM::TEAM2) {
		dir = -1;
	}
}

void Player::Hit(Projectile* projectile) {
	if (shieldHealth > 0 && playerState == BLOCK && projectile->GetStrength() != PROJECTILE_STRENGTH::SPECIAL) 
	{
		shieldHealth -= projectile->GetDamage() * 2;
		if (shieldHealth > 0) {
			shield->SetWorldScale(shieldHealth / 100.0f, shieldHealth / 100.0f, 0.05f);
		}
	}
	else
	{
		if (playerInput->CheckForController()) {
			playerInput->PlayFeedback(projectile->GetStunTime());
		}
		physicsComponent->AddForce(projectile->GetForce());
		Stun(projectile->GetStunTime());
		dialogue.playRandomFromOtherState(dialogue.TakingDamage, false);
	}

	stunEffect->Play();
	stunEffect->SetWorldPosition(worldPosition.x, worldPosition.y + 0.5f, worldPosition.z);
}

void Player::Stun() {
	ComboReset();
	stunTimer = stunTimerSet;
	playerState = STUN;
	ResetModel();
}

void Player::Stun(float stunTime) {
	stunTimer = stunTime;
	playerState = STUN;
	ResetModel();
}

void Player::Block() {
	if (playerState == PLAYERSTATES::NORMAL) {
		if (shieldHealth > 0) {
			playerState = BLOCK;
			physicsComponent->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		}
		else if (shieldHealth <= 0) {
			playerState = NORMAL;
		}
		ResetModel();
	}
}

void Player::StopBlock() {
	if (playerState == BLOCK) {
		playerState = NORMAL;
		ResetModel();
	}
}

void Player::ComboReset() {
	lightComboTimer = 0;
	lightComboPosition = 0;
	mediumComboTimer = 0;
	mediumComboPosition = 0;
	heavyComboTimer = 0;
	heavyComboPosition = 0;
	specialComboTimer = 0;
	specialComboPosition = 0;
}

void Player::Jump() {
	if (worldPosition.y < 0.1f && worldPosition.y > -0.1f && playerState == NORMAL) {
		physicsComponent->AddForce(glm::vec3(0.0f, 110.0f, 0.0f));
	}
}

void Player::SwitchTarget() {
	if (currentTarget == 0) {
		currentTarget = 1;
	}
	else if (currentTarget == 1) {
		currentTarget = 0;
	}
}

void Player::EnableTarget() {
	if (isTargeting) {
		isTargeting = false;
	}
	else if (!isTargeting) {
		isTargeting = true;
	}
}

void Player::Render(Shader* shader, const double _interpolation)
{
	// Account for interpolation
	glm::mat4 interpolatedMatrix;
	interpolatedMatrix = worldModelMatrix * (float)_interpolation + previousWorldModelMatrix * (1.0f - (float)_interpolation);

	// Account for interpolation
	glm::mat4 ringInterpolatedMatrix;
	ringInterpolatedMatrix = ring->GetWorldModelMatrix() * (float)_interpolation + ring->GetPreviousWorldMatrix() * (1.0f - (float)_interpolation);

	// Account for interpolation
	glm::mat4 shieldInterpolatedMatrix;
	shieldInterpolatedMatrix = shield->GetWorldModelMatrix() * (float)_interpolation + shield->GetPreviousWorldMatrix() * (1.0f - (float)_interpolation);

	// Account for interpolation
	glm::mat4 markerInterpolatedMatrix;
	markerInterpolatedMatrix = marker->GetWorldModelMatrix() * (float)_interpolation + marker->GetPreviousWorldMatrix() * (1.0f - (float)_interpolation);

	// Don't interpolate on the first render 
	if (firstRender == true) {
		interpolatedMatrix = worldModelMatrix;
		ringInterpolatedMatrix = ring->GetWorldModelMatrix();
		shieldInterpolatedMatrix = shield->GetWorldModelMatrix();
		markerInterpolatedMatrix = marker->GetWorldModelMatrix();
	}

	shader->setMat4("model", interpolatedMatrix * localModelMatrix * base->GetWorldModelMatrix() * base->GetLocalModelMatrix() * base->model->GetWorldModelMatrix() * base->model->GetLocalModelMatrix());
	base->Render(shader, _interpolation);

	if (worldPosition.y > -1.0f) {
		ring->SetWorldPosition(worldPosition.x, ring->GetWorldPosition().y, worldPosition.z);
		shader->setMat4("model", ringInterpolatedMatrix * ring->GetLocalModelMatrix());
		ring->Render(shader, _interpolation);
	}

	shader->setMat4("model", interpolatedMatrix * localModelMatrix * shieldInterpolatedMatrix * shield->GetLocalModelMatrix());
	shield->Render(shader, _interpolation);

	/*
	if (isTargeting) {
	shader->setMat4("model", markerInterpolatedMatrix * marker->GetLocalModelMatrix());
	marker->Render(shader, _interpolation);
	}
	*/
}

void Player::SetPlayerNumber(PLAYERNUMBER pN) {
	playerNumber = pN;
	int x = 0;
	switch (pN)
	{
	case GAME::Player::PLAYER1:
		x = 0;
		break;
	case GAME::Player::PLAYER2:
		x = 1;
		break;
	case GAME::Player::PLAYER3:
		x = 2;
		break;
	case GAME::Player::PLAYER4:
		x = 3;
		break;
	case GAME::Player::NONE:
		x = 0;
		break;
	default:
		break;
	}
	playerInput->setPlayerNum(x);
}


void GAME::Player::AIMovement()
{
	///learning block
	/// negative numbers on the z axis are forwrds for both teams. positive numbers are backwards
	/// in the x axis it is different for each team
	/// negative numbers are left and positive numbers are right for team 1
	/// positive numbers are left for team 2 and positive numbers are right

	if (CPUMovementCD.checkOffCD()) {
		playerInput->ResetMovementButtons();
		double dir = rand.box_muller(5, 5);
		double dir2 = rand.box_muller(5, 5);

		if (dir < 3) { //forward
			playerInput->controllerState.at(2) = -3;
		}
		else if (dir > 7) { //back
			playerInput->controllerState.at(2) = 3;
		}

		if (dir2 < 4) { //left
			playerInput->controllerState.at(1) = -3;
		}
		else if (dir2 > 6) { //right
			playerInput->controllerState.at(1) = 3;
		}

		int random = Clock::GetInstance()->generateRandomNumber();
		double newCD = 0.35 + (random * random / 100);
		CPUMovementCD.setNewDuration(newCD);
		CPUMovementCD.startCD();
	}
}

void GAME::Player::AIShooting()
{
	if (CPUShootingCD.checkOffCD()) {
		playerInput->ResetShootingButtons();
		if (specialMeter == 100) {
			playerInput->controllerState.at(5) = 1;
		}
		else {
			int dir = rand.box_muller(5, 5);
			if (dir <= 4) { //light
				playerInput->controllerState.at(6) = 1;
			}
			else if (dir <= 7) { //medium
				playerInput->controllerState.at(11) = 1;
			}
			else { //heavy
				playerInput->controllerState.at(12) = 1;
			}
		}

		int random = Clock::GetInstance()->generateRandomNumber();
		double newCD = random * random / 100;
		newCD = 0.6 + newCD;
		CPUShootingCD.setNewDuration(newCD);
		CPUShootingCD.startCD();
	}
}

///learning block
/// negative numbers on the z axis are forwrds for both teams. positive numbers are backwards
/// in the x axis it is different for each team
/// negative numbers are left and positive numbers are right for team 1
/// positive numbers are left for team 2 and positive numbers are right

void Player::BoundsCheckT2()
{
	//std::cout << worldPosition.x << " " << worldPosition.z << std::endl;
	if (worldPosition.z > -1) { //too close to the front
		playerInput->controllerState.at(2) = 3;
	}
	if (worldPosition.z < -4.2) { //too close to the back
		playerInput->controllerState.at(2) = -3;
	}
	if (worldPosition.x < -4) { //too close to the front
		playerInput->controllerState.at(1) = -3;
	}
	if (worldPosition.x > 4) { //too close to the front
		playerInput->controllerState.at(1) = 3;
	}
}

void GAME::Player::BoundsCheckT1()
{
	//std::cout << worldPosition.x << " " << worldPosition.z << std::endl;
	if (worldPosition.z < 1) { //too close to the front
		playerInput->controllerState.at(2) = 3;
	}
	if (worldPosition.z > 4.2) { //too close to the back
		playerInput->controllerState.at(2) = -3;
	}
	if (worldPosition.x < -4) { //too close to the front
		playerInput->controllerState.at(1) = 3;
	}
	if (worldPosition.x > 4) { //too close to the front
		playerInput->controllerState.at(1) = -3;
	}

}

void Player::HandleControllerEvents() {
	// Player can press buttons when not off the arena
	if (!out) {
		if (playerInput->controllerState.at(6) == 1) //right trigger
		{
			if (computerPlayer) {
				playerInput->controllerState.at(6) = 0;
			}
			std::vector<Projectile*> p = LightAttack();
			if (p.size() > 0) {
				for each (Projectile* subP in p) {

					if (playerTeam == PLAYERTEAM::TEAM1) {
						subP->SetTeam(PROJECTILE_TEAM::TEAM1);
					}
					else if (playerTeam == PLAYERTEAM::TEAM2) {
						subP->SetTeam(PROJECTILE_TEAM::TEAM2);
					}

					if (playerNumber == PLAYERNUMBER::PLAYER1) {
						subP->SetPlayer(PROJECTILE_PLAYER::PLAYER1);
					}
					else if (playerNumber == PLAYERNUMBER::PLAYER2) {
						subP->SetPlayer(PROJECTILE_PLAYER::PLAYER2);
					}
					else if (playerNumber == PLAYERNUMBER::PLAYER3) {
						subP->SetPlayer(PROJECTILE_PLAYER::PLAYER3);
					}
					else if (playerNumber == PLAYERNUMBER::PLAYER4) {
						subP->SetPlayer(PROJECTILE_PLAYER::PLAYER4);
					}

					subP->SetStrength(PROJECTILE_STRENGTH::LIGHT);
					projectileManager->AddProjectile(subP);

					shootEffect->Play();
					shootEffect->SetWorldPosition(subP->GetWorldPosition());
				}
			}
		}
		// Left Bumper
		if (playerInput->NewButtonPress(11))
		{
			std::vector<Projectile*> p = MediumAttack();
			if (p.size() > 0) {
				for each (Projectile* subP in p) {

					if (playerTeam == PLAYERTEAM::TEAM1) {
						subP->SetTeam(PROJECTILE_TEAM::TEAM1);
					}
					else if (playerTeam == PLAYERTEAM::TEAM2) {
						subP->SetTeam(PROJECTILE_TEAM::TEAM2);
					}

					if (playerNumber == PLAYERNUMBER::PLAYER1) {
						subP->SetPlayer(PROJECTILE_PLAYER::PLAYER1);
					}
					else if (playerNumber == PLAYERNUMBER::PLAYER2) {
						subP->SetPlayer(PROJECTILE_PLAYER::PLAYER2);
					}
					else if (playerNumber == PLAYERNUMBER::PLAYER3) {
						subP->SetPlayer(PROJECTILE_PLAYER::PLAYER3);
					}
					else if (playerNumber == PLAYERNUMBER::PLAYER4) {
						subP->SetPlayer(PROJECTILE_PLAYER::PLAYER4);
					}

					if (subP->GetElement() == PROJECTILE_ELEMENT::EARTH) {
						subP->SetStrength(PROJECTILE_STRENGTH::SPECIAL);
					}
					else {
						subP->SetStrength(PROJECTILE_STRENGTH::MEDIUM);
					}
					projectileManager->AddProjectile(subP);

					shootEffect->Play();
					shootEffect->SetWorldPosition(subP->GetWorldPosition());
				}
			}
		}
		// Right Bumper
		if (playerInput->NewButtonPress(12))
		{
			std::vector<Projectile*> p = HeavyAttack();
			if (p.size() > 0) {
				for each (Projectile* subP in p) {

					if (playerTeam == PLAYERTEAM::TEAM1) {
						subP->SetTeam(PROJECTILE_TEAM::TEAM1);
					}
					else if (playerTeam == PLAYERTEAM::TEAM2) {
						subP->SetTeam(PROJECTILE_TEAM::TEAM2);
					}

					if (playerNumber == PLAYERNUMBER::PLAYER1) {
						subP->SetPlayer(PROJECTILE_PLAYER::PLAYER1);
					}
					else if (playerNumber == PLAYERNUMBER::PLAYER2) {
						subP->SetPlayer(PROJECTILE_PLAYER::PLAYER2);
					}
					else if (playerNumber == PLAYERNUMBER::PLAYER3) {
						subP->SetPlayer(PROJECTILE_PLAYER::PLAYER3);
					}
					else if (playerNumber == PLAYERNUMBER::PLAYER4) {
						subP->SetPlayer(PROJECTILE_PLAYER::PLAYER4);
					}

					subP->SetStrength(PROJECTILE_STRENGTH::HEAVY);
					projectileManager->AddProjectile(subP);

					shootEffect->Play();
					shootEffect->SetWorldPosition(subP->GetWorldPosition());
				}
			}
		}
		// left trigger
		if (playerInput->NewButtonPress(5))
		{
			std::vector<Projectile*> p = SpecialAttack();
			if (p.size() > 0) {
				for each (Projectile* subP in p) {

					if (playerTeam == PLAYERTEAM::TEAM1) {
						subP->SetTeam(PROJECTILE_TEAM::TEAM1);
					}
					else if (playerTeam == PLAYERTEAM::TEAM2) {
						subP->SetTeam(PROJECTILE_TEAM::TEAM2);
					}

					if (playerNumber == PLAYERNUMBER::PLAYER1) {
						subP->SetPlayer(PROJECTILE_PLAYER::PLAYER1);
					}
					else if (playerNumber == PLAYERNUMBER::PLAYER2) {
						subP->SetPlayer(PROJECTILE_PLAYER::PLAYER2);
					}
					else if (playerNumber == PLAYERNUMBER::PLAYER3) {
						subP->SetPlayer(PROJECTILE_PLAYER::PLAYER3);
					}
					else if (playerNumber == PLAYERNUMBER::PLAYER4) {
						subP->SetPlayer(PROJECTILE_PLAYER::PLAYER4);
					}

					subP->SetStrength(PROJECTILE_STRENGTH::SPECIAL);
					projectileManager->AddProjectile(subP);

					shootEffect->Play();
					shootEffect->SetWorldPosition(subP->GetWorldPosition());
				}
			}
		}
		// left trigger
		if (playerInput->NewButtonPress(17))
		{
			dialogue.playRandomFromOtherState(dialogue.Rare, true);
		}
		//switching functionality removed
	}
}