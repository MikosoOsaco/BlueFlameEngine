#ifndef FIREPLAYER_H
#define FIREPLAYER_H

#include "Player.h"

using namespace ENGINE;

namespace GAME
{
	class FirePlayer : public Player
	{
	public:
		FirePlayer();
		~FirePlayer();

		// Attack Functions
		std::vector<Projectile*> LightAttack();
		std::vector<Projectile*> MediumAttack();
		std::vector<Projectile*> HeavyAttack();
		std::vector<Projectile*> SpecialAttack();

		void InheritedUpdate(const float deltaTime);
		void InheritedHandleEvents(SDL_Event events);
		void InheritedHandleStates(const Uint8 *state);

	private:
		std::vector<Projectile*> tempSpecialProjs;

	};
}

#endif // !FIREPLAYER_H
