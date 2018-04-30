#pragma once 
#ifndef ICEPLAYER_H
#define ICEPLAYER_H

#include "Player.h"

using namespace ENGINE;

namespace GAME {

	class IcePlayer : public Player
	{
	public:
		IcePlayer();
		~IcePlayer();

		// Attack Functions
		std::vector<Projectile*> LightAttack();
		std::vector<Projectile*> MediumAttack();
		std::vector<Projectile*> HeavyAttack();
		std::vector<Projectile*> SpecialAttack();

		void InheritedUpdate(const float deltaTime);
		void InheritedHandleEvents(SDL_Event events);
		void InheritedHandleStates(const Uint8 *state);
	};

}

#endif

