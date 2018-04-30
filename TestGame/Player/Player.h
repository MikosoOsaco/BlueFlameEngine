#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <SDL/SDL.h>
#include <BlueFlameEngine\Engine\BFEngine.h>
#include <BlueFlameEngine\Engine\Audio\PlayerDialogue.h>
#include <BlueFlameEngine\Engine\Core\GameObject.h>
#include <BlueFlameEngine\Engine\Core\RenderComponent.h>
#include <BlueFlameEngine\Engine\Core\ResourceManager.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Projectile.h>
#include <BlueFlameEngine\Engine\Graphics\ParticleSystem.h>
#include "../Game/ProjectileManager.h"
#include "PlayerBase.h"
#include "PlayerInput.h"
#include <BlueFlameEngine\Engine\Math\Randomizer.h>

using namespace ENGINE;

namespace GAME {

	// Forward declare Projectile Manager to avoid circular dependency 
	class ProjectileManager;

	// Base Player Class
	class Player : public GameObject
	{
	public:
		// Constructors
		Player();
		virtual ~Player();

		// Enum for Player number
		enum PLAYERNUMBER { PLAYER1, PLAYER2, PLAYER3, PLAYER4, NONE };
		// Enum for Player Team
		enum PLAYERTEAM { TEAM1, TEAM2, TEAM0 };
		// Enum for player movement
		enum PLAYERMOVEMENT { FORWARD, BACKWARD, RIGHT, LEFT };
		// Enum for player states
		enum PLAYERSTATES { NORMAL, ATTACK, BLOCK, STUN, JUMP, DODGE, DEAD };

		// Loop functions
		// DO NOT OVERRIDE THIS FUNCTION
		void Update(const float deltaTime);
		void FixedUpdate(const float deltaTime);
		//
		// Children use this method
		virtual void InheritedUpdate(const float deltaTime) {};
		// DO NOT OVERRIDE THIS FUNCTION
		void HandleEvents(SDL_Event events);
		//
		// Children use this method
		virtual void InheritedHandleEvents(SDL_Event events) {};
		// DO NOT OVERRIDE THIS FUNCTION
		void HandleStates(const Uint8 *state);
		//
		// Children use this method
		virtual void InheritedHandleStates(const Uint8 *state) {};
		// Render
		void Render(Shader* shader, const double _interpolation);

		// Player movement
		// Give a direction and deltatime to move player
		void Movement(PLAYERMOVEMENT movement, const float deltaTime);

		// Player Model Functions
		// Used to move the parts of the player
		void UpdateModel(const float deltaTime);
		void ResetModel();

		// STATUS FUNCTIONS
		//
		// Use this to set up initial player stats such as health
		virtual void SetStats();
		// Called when the player gets hit
		void Hit(Projectile* projectile);
		// Call this to stun the player for 1 second
		void Stun();
		// Call this to stun player for a certain amount of time
		void Stun(float stunTime);
		// Call this to set the player to block and render shield
		void Block();
		// Call this to set the player to stop blocking
		void StopBlock();
		// Resets combo 
		void ComboReset();
		// Jump
		void Jump();
		// Switches target
		void SwitchTarget();
		// Enables / Disables targetting
		void EnableTarget();
		//

		// Attack Functions
		// Returns a list of projectile
		virtual std::vector<Projectile*> LightAttack() = 0;
		virtual std::vector<Projectile*> MediumAttack() = 0;
		virtual std::vector<Projectile*> HeavyAttack() = 0;
		virtual std::vector<Projectile*> SpecialAttack() = 0;
		void AddSpecialMeter(int value) { if(specialMeter < 100 ) {specialMeter += value; } if (specialMeter > 100) { specialMeter = 100; } };

		// Sets the projectile manager for the player to use
		void AddProjecitleManager(ProjectileManager* pM);

		// Target
		// Used for setting the player's target
		void SetEnemyTeam(Player* player1, Player* player2);
		void SetTarget(glm::vec3 targetPlayer);
		void SetTarget(int target);
		void SetTargetColour(glm::vec3 colour);

		// Player number and team setter
		void SetPlayerNumber(PLAYERNUMBER pN);
		void SetPlayerTeam(PLAYERTEAM pT);

		// Set player gameplay status
		void SetIsOut(bool isOut) { out = isOut; };
		void SetCanMove(bool canM) { canMove = canM; };

		// Getters
		int GetHealth() { return shieldHealth; };
		int GetMaxHealth() { return maxHealth; };
		float GetMoveSpeed() { return moveSpeed; };
		int GetSpecialMeter() { return specialMeter; };
		PLAYERSTATES GetPlayerState() { return playerState; };
		PLAYERNUMBER GetPlayerNumber() { return playerNumber; };
		PLAYERTEAM GetPlayerTeam() { return playerTeam; };
		float GetStunTimer() { return stunTimer; };
		bool GetIsTargeting() { return isTargeting; };
		std::vector<Player*> GetEnemyTeam() { return enemyTeam; };
		PlayerInput* GetPlayerInput() { return playerInput; };
		bool IsOut() { return out; };
		bool CanMove() { return canMove; };

		//CPU Player
		void SetAsCPU() { computerPlayer = true; }

		//Player Specific Audio
		//MUST HAVE A CONSTRUCTOR FOR THIS IN EXPANDED CLASS
		PlayerDialogue dialogue; //USED TO STORE PLAYER DIALOGUE. LoadPlayerDialogue(
		SoundEffectSelector pSoundEffects;

		bool PlayingIntro;

	protected:
		// Player model parts
		PlayerBase* base;
		PlayerBase* ring;
		PlayerBase* shield;
		PlayerBase* marker;
		bool down = false;

		// Player Target
		bool isTargeting;
		int currentTarget;
		std::vector<Player*> enemyTeam;
		glm::vec3 targetedPlayer;
		glm::vec3 targetColour;
		float targetAngle;
		int dir;

		// Combo and movement cooldowns
		Cooldown HeavyCD;
		Cooldown LightCD;
		Cooldown MediumCD;
		Cooldown movementCD;
		Cooldown shieldCD;
		Cooldown healthIncrementCD;

		// Projectile manager reference 
		ProjectileManager* projectileManager;

		// Player Input 
		PlayerInput* playerInput;

		// Player Stats
		int shieldHealth = 0;
		int maxHealth = 100;
		int specialMeter = 0;
		int maxSpecialMeter = 100;
		int staminaMeter = 0;
		int maxStaminaMeter = 100;
		float moveSpeed = 1.5f;
		PLAYERNUMBER playerNumber = NONE;
		PLAYERTEAM playerTeam = TEAM0;

		// Player State
		PLAYERSTATES playerState = NORMAL;
		float stunTimer = 0;
		float stunTimerSet = 1.0f;
		bool moveWhileShooting = false;

		// Player Combo stats
		float lightComboTimer = 0;
		float mediumComboTimer = 0;
		float heavyComboTimer = 0;
		float specialComboTimer = 0;

		int lightComboPosition = 0;
		int mediumComboPosition = 0;
		int heavyComboPosition = 0;
		int specialComboPosition = 0;

		// Projectile shader
		Shader* projectileShader;
		ResourceHandle<Shader> pShader;

		// Check to see if off arena
		bool out;

		// Check to see if player can move
		bool canMove;

		// Particles
		ParticleSystem* stunEffect;
		ParticleSystem* shootEffect;

		//handle events but for networked players
		void HandleControllerEvents();

		//AI???
		bool computerPlayer = false;

		void AIMovement();
		void AIShooting();

		void BoundsCheckT2();
		void BoundsCheckT1();

		Cooldown CPUMovementCD;
		Cooldown CPUShootingCD;

		Randomizer rand;
		//end AI
		
	};

}

#endif
