#pragma once
#ifndef EARTHPLAYER_H
#define EARTHPLAYER_H

#include "Player.h"
#include <BlueFlameEngine\Engine\Timers\Cooldown.h>

using namespace ENGINE;

namespace GAME {

	class EarthPlayer : public Player
	{
	public:
		EarthPlayer();
		~EarthPlayer();

		// Attack Functions
		std::vector<Projectile*> LightAttack();
		std::vector<Projectile*> MediumAttack();
		std::vector<Projectile*> HeavyAttack();
		std::vector<Projectile*> SpecialAttack();

		void InheritedUpdate(const float deltaTime);
		void InheritedHandleEvents(SDL_Event events);
		void InheritedHandleStates(const Uint8 *state);

	private:
		Projectile* smallRock(float offset, float xxx); //used to create light attack
		Projectile* SpecialRock(float offset); //used to create Special attack
		Projectile* stalagmite(float offset, float delay); //used to creat really cool attack
		bool specialActivated = false;
	};

}

#endif
