#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

#include <vector>
#include <BlueFlameEngine\Engine\Math\PhysicsEngine.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Projectile.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Cube.h>
#include "../Player/Player.h"
#include <BlueFlameEngine\Engine\Core\Scene.h>
#include <BlueFlameEngine\Engine\BFEngine.h>
#include <BlueFlameEngine\Engine\Graphics\Shader.h>
#include "ProjectileRenderer.h"

using namespace ENGINE;

namespace GAME {
	// Forward delcare player class to avoid circular dependency
	class Player;

	class ProjectileManager {
	private:
		// Lists of projectiles and players/environments they can come into contact with
		std::vector<Projectile*> projectileList;
		std::vector<Player*> playerList;
		std::vector<GameObject*> environmentList;

		// Projectile Renderer 
		ProjectileRenderer* projectileRenderer;

		// Int that checks if you'd like friendly fire / no friendly fire / phase through
		int projectileInteraction;

	public:
		ProjectileManager();

		// Functions to add and remove the corresponding objects from the list
		void AddProjectile(Projectile* _projectile);
		void AddPlayer(Player* _player);
		void AddEnvironment(GameObject* _environment);
		void RemoveProjectile(Projectile* _projectile);
		void RemovePlayer(Player* _player);
		void RemoveEnvironment(GameObject* _environment);

		// Sets the projectile interaction type
		void SetFriendlyFire(bool _fireType);
		void SetPhaseThrough(bool _phaseType);

		// Deletes the projectile that is weaker
		void ProjectileCollision(Projectile &_proj1, Projectile &_proj2);

		// Checking if projectile should collide with players
		bool IsSameTeam(Player &_player, Projectile &_projectile);
		bool IsSameTeam(Projectile &_proj1, Projectile &_proj2);
		bool IsSamePlayer(Player &_player, Projectile &_projectile);
		bool IsSamePlayer(Projectile &_proj1, Projectile &_proj2);

	public:

		std::vector<Projectile*> GetProjectileList();

		ProjectileRenderer* GetProjectileRenderer() { return projectileRenderer; };

		// Game and graphics loops
		void Update(float deltaTime);
		void FixedUpdate(const float deltaTime);

	};
}
#endif
