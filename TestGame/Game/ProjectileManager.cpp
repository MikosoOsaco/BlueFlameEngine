#include "ProjectileManager.h"
using namespace ENGINE;

namespace GAME {

	ProjectileManager::ProjectileManager() {
		projectileRenderer = new ProjectileRenderer();

		// Default to no friendly fire and no phase through
		SetFriendlyFire(false);
	}

	void ProjectileManager::AddProjectile(Projectile* _projectile) {
		_projectile->CreateCollision(projectileRenderer->GetCubeMesh(), _projectile->GetWorldPosition());
		projectileList.push_back(_projectile);
	}

	void ProjectileManager::AddPlayer(Player* _player) {
		playerList.push_back(_player);
	}

	void ProjectileManager::AddEnvironment(GameObject* _environment) {
		environmentList.push_back(_environment);
	}

	void ProjectileManager::RemoveProjectile(Projectile* _projectile) {
		projectileList.erase(std::remove(projectileList.begin(), projectileList.end(), _projectile), projectileList.end());
	}

	void ProjectileManager::RemovePlayer(Player* _player) {
		playerList.erase(std::remove(playerList.begin(), playerList.end(), _player), playerList.end());
	}

	void ProjectileManager::RemoveEnvironment(GameObject* _environment) {
		environmentList.erase(std::remove(environmentList.begin(), environmentList.end(), _environment), environmentList.end());
	}

	void ProjectileManager::SetFriendlyFire(bool _fireType) {
		if (_fireType)
			projectileInteraction = 1;
		else if (!_fireType && (projectileInteraction == 2 || projectileInteraction == 3))
			projectileInteraction = 2;
		else if (!_fireType)
			projectileInteraction = 0;
	}

	void ProjectileManager::SetPhaseThrough(bool _phaseType) {
		if (_phaseType && projectileInteraction == 1)
			projectileInteraction = 3;
		else if (_phaseType && projectileInteraction == 0)
			projectileInteraction = 2;
		else if (!_phaseType && projectileInteraction == 3)
			projectileInteraction = 1;
		else if (!_phaseType && projectileInteraction == 2)
			projectileInteraction = 0;
		else if (!_phaseType) {
			projectileInteraction = 0;
		}
		else if (_phaseType) {
			projectileInteraction = 2;
		}
	}

	void ProjectileManager::ProjectileCollision(Projectile &_proj1, Projectile &_proj2) {
		if (_proj1.GetClipping() == PROJECTILE_CLIP::YES_PLAYER_PROJECTILE ||
			_proj1.GetClipping() == PROJECTILE_CLIP::YES_WALL_PROJECTILE ||
			_proj1.GetClipping() == PROJECTILE_CLIP::YES_PROJECTILE ||
			_proj1.GetClipping() == PROJECTILE_CLIP::YES ||
			_proj2.GetClipping() == PROJECTILE_CLIP::YES_PLAYER_PROJECTILE ||
			_proj2.GetClipping() == PROJECTILE_CLIP::YES_WALL_PROJECTILE ||
			_proj2.GetClipping() == PROJECTILE_CLIP::YES_PROJECTILE ||
			_proj2.GetClipping() == PROJECTILE_CLIP::YES) {

			if (_proj1.GetStrength() == _proj2.GetStrength()) {
				_proj1.deleted = true;
				_proj2.deleted = true;
			}
			else if (_proj1.GetStrength() > _proj2.GetStrength()) {
				_proj2.deleted = true;
			}
			else if (_proj1.GetStrength() < _proj2.GetStrength()) {
				_proj1.deleted = true;
			}
			else {

			}
		}
	}
	bool ProjectileManager::IsSameTeam(Player &_player, Projectile &_projectile) {
		if ((_player.GetPlayerTeam() == Player::PLAYERTEAM::TEAM1 && _projectile.GetTeam() == PROJECTILE_TEAM::TEAM1) ||
			(_player.GetPlayerTeam() == Player::PLAYERTEAM::TEAM2 && _projectile.GetTeam() == PROJECTILE_TEAM::TEAM2)) {
			return true;
		}
		else {
			return false;
		}
	}

	bool ProjectileManager::IsSameTeam(Projectile &_proj1, Projectile &_proj2) {
		if (_proj1.GetTeam() == _proj2.GetTeam()) {
			return true;
		}
		else {
			return false;
		}
	}

	bool ProjectileManager::IsSamePlayer(Player &_player, Projectile &_projectile) {
		if ((_player.GetPlayerNumber() == Player::PLAYERNUMBER::PLAYER1 && _projectile.GetPlayer() == PROJECTILE_PLAYER::PLAYER1) ||
			(_player.GetPlayerNumber() == Player::PLAYERNUMBER::PLAYER2 && _projectile.GetPlayer() == PROJECTILE_PLAYER::PLAYER2) ||
			(_player.GetPlayerNumber() == Player::PLAYERNUMBER::PLAYER3 && _projectile.GetPlayer() == PROJECTILE_PLAYER::PLAYER3) ||
			(_player.GetPlayerNumber() == Player::PLAYERNUMBER::PLAYER4 && _projectile.GetPlayer() == PROJECTILE_PLAYER::PLAYER4)) {
			return true;
		}
		else {
			return false;
		}
	}

	bool ProjectileManager::IsSamePlayer(Projectile &_proj1, Projectile &_proj2) {
		if (_proj1.GetPlayer() == _proj2.GetPlayer()) {
			return true;
		}
		else {
			return false;
		}
	}

	std::vector<Projectile*> ProjectileManager::GetProjectileList() {
		return projectileList;
	}

	void ProjectileManager::Update(float deltaTime) {

		for (unsigned int i = 0; i < projectileList.size(); ++i) {
			projectileList.at(i)->Update(deltaTime);
		}
	}

	void ProjectileManager::FixedUpdate(const float deltaTime) {

		projectileRenderer->SetProjectileList(projectileList);

		for (unsigned int i = 0; i < projectileList.size(); ++i) {
			projectileList.at(i)->UpdateState();
			projectileList.at(i)->FixedUpdate(deltaTime);
		}

		// Collisions with player
		int proSize = projectileList.size();
		int plaSize = playerList.size();
		for (int i = 0; i < plaSize; i++) {
			for (int j = 0; j < proSize; j++) {
				// First Check
				// Don't bother checking collision if the projectile was sent by that player 
				if (!IsSamePlayer(*playerList.at(i), *projectileList.at(j))) {
					if (playerList.at(i)->collisionComponent != NULL && projectileList.at(j)->collisionComponent != NULL && projectileList.at(j)->collisionComponent->GetBoundingBox().c != glm::vec3(0.0f)) {
						if (PhysicsEngine::isColliding(playerList.at(i)->collisionComponent, projectileList.at(j)->collisionComponent)) {
							if (projectileList.at(j)->GetClipping() == PROJECTILE_CLIP::YES_WALL_PLAYER ||
								projectileList.at(j)->GetClipping() == PROJECTILE_CLIP::YES_PLAYER_PROJECTILE ||
								projectileList.at(j)->GetClipping() == PROJECTILE_CLIP::YES_PLAYER ||
								projectileList.at(j)->GetClipping() == PROJECTILE_CLIP::YES) {

								// If friendly fire is off, delete projectiles when they hit friend
								if (projectileInteraction == 0) {
									projectileList.at(j)->deleted = true;
									if (!IsSameTeam(*playerList.at(i), *projectileList.at(j))) {
										if (projectileList.at(j)->GetPlayer() == PROJECTILE_PLAYER::PLAYER1) {
											if (playerList.at(i)->GetPlayerState() != Player::PLAYERSTATES::BLOCK)
												playerList.at(0)->AddSpecialMeter(projectileList.at(j)->GetDamage());
										}
										else if (projectileList.at(j)->GetPlayer() == PROJECTILE_PLAYER::PLAYER2) {
											if (playerList.at(i)->GetPlayerState() != Player::PLAYERSTATES::BLOCK)
												playerList.at(1)->AddSpecialMeter(projectileList.at(j)->GetDamage());
										}
										else if (projectileList.at(j)->GetPlayer() == PROJECTILE_PLAYER::PLAYER3) {
											if (playerList.at(i)->GetPlayerState() != Player::PLAYERSTATES::BLOCK)
												playerList.at(2)->AddSpecialMeter(projectileList.at(j)->GetDamage());
										}
										else if (projectileList.at(j)->GetPlayer() == PROJECTILE_PLAYER::PLAYER4) {
											if (playerList.at(i)->GetPlayerState() != Player::PLAYERSTATES::BLOCK)
												playerList.at(3)->AddSpecialMeter(projectileList.at(j)->GetDamage());
										}
										playerList.at(i)->Hit(projectileList.at(j));
									}
								}
								// If friendly fire is on, act like the hit player is an enemy
								else if (projectileInteraction == 1) {
									projectileList.at(j)->deleted = true;
									if (projectileList.at(j)->GetPlayer() == PROJECTILE_PLAYER::PLAYER1) {
										if (playerList.at(i)->GetPlayerState() != Player::PLAYERSTATES::BLOCK)
											playerList.at(0)->AddSpecialMeter(projectileList.at(j)->GetDamage());
									}
									else if (projectileList.at(j)->GetPlayer() == PROJECTILE_PLAYER::PLAYER2) {
										if (playerList.at(i)->GetPlayerState() != Player::PLAYERSTATES::BLOCK)
											playerList.at(1)->AddSpecialMeter(projectileList.at(j)->GetDamage());
									}
									else if (projectileList.at(j)->GetPlayer() == PROJECTILE_PLAYER::PLAYER3) {
										if (playerList.at(i)->GetPlayerState() != Player::PLAYERSTATES::BLOCK)
											playerList.at(2)->AddSpecialMeter(projectileList.at(j)->GetDamage());
									}
									else if (projectileList.at(j)->GetPlayer() == PROJECTILE_PLAYER::PLAYER4) {
										if (playerList.at(i)->GetPlayerState() != Player::PLAYERSTATES::BLOCK)
											playerList.at(3)->AddSpecialMeter(projectileList.at(j)->GetDamage());
									}
									playerList.at(i)->Hit(projectileList.at(j));
								}
								// If the projectiles are set to phase through, projectiles only detect collision with enemy team
								else if (projectileInteraction == 2 || projectileInteraction == 3) {
									if (!IsSameTeam(*playerList.at(i), *projectileList.at(j))) {
										if (projectileList.at(j)->GetPlayer() == PROJECTILE_PLAYER::PLAYER1) {
											if (playerList.at(i)->GetPlayerState() != Player::PLAYERSTATES::BLOCK)
												playerList.at(0)->AddSpecialMeter(projectileList.at(j)->GetDamage());
										}
										else if (projectileList.at(j)->GetPlayer() == PROJECTILE_PLAYER::PLAYER2) {
											if (playerList.at(i)->GetPlayerState() != Player::PLAYERSTATES::BLOCK)
												playerList.at(1)->AddSpecialMeter(projectileList.at(j)->GetDamage());
										}
										else if (projectileList.at(j)->GetPlayer() == PROJECTILE_PLAYER::PLAYER3) {
											if (playerList.at(i)->GetPlayerState() != Player::PLAYERSTATES::BLOCK)
												playerList.at(2)->AddSpecialMeter(projectileList.at(j)->GetDamage());
										}
										else if (projectileList.at(j)->GetPlayer() == PROJECTILE_PLAYER::PLAYER4) {
											if (playerList.at(i)->GetPlayerState() != Player::PLAYERSTATES::BLOCK)
												playerList.at(3)->AddSpecialMeter(projectileList.at(j)->GetDamage());
										}
										playerList.at(i)->Hit(projectileList.at(j));
									}
								}
								// If all fails, just act like every player is an enemy
								else {
									projectileList.at(j)->deleted = true;
									if (projectileList.at(j)->GetPlayer() == PROJECTILE_PLAYER::PLAYER1) {
										if (playerList.at(i)->GetPlayerState() != Player::PLAYERSTATES::BLOCK)
											playerList.at(0)->AddSpecialMeter(projectileList.at(j)->GetDamage());
									}
									else if (projectileList.at(j)->GetPlayer() == PROJECTILE_PLAYER::PLAYER2) {
										if (playerList.at(i)->GetPlayerState() != Player::PLAYERSTATES::BLOCK)
											playerList.at(1)->AddSpecialMeter(projectileList.at(j)->GetDamage());
									}
									else if (projectileList.at(j)->GetPlayer() == PROJECTILE_PLAYER::PLAYER3) {
										if (playerList.at(i)->GetPlayerState() != Player::PLAYERSTATES::BLOCK)
											playerList.at(2)->AddSpecialMeter(projectileList.at(j)->GetDamage());
									}
									else if (projectileList.at(j)->GetPlayer() == PROJECTILE_PLAYER::PLAYER4) {
										if (playerList.at(i)->GetPlayerState() != Player::PLAYERSTATES::BLOCK)
											playerList.at(3)->AddSpecialMeter(projectileList.at(j)->GetDamage());
									}
									playerList.at(i)->Hit(projectileList.at(j));
								}
							}
						}
					}
				}
			}
		}

		// Collisions with environment
		proSize = projectileList.size();
		int envSize = environmentList.size();
		for (int i = 0; i < envSize; i++) {
			for (int j = 0; j < proSize; j++) {
				if (environmentList.at(i)->collisionComponent != NULL && projectileList.at(j)->collisionComponent != NULL && projectileList.at(j)->collisionComponent->GetBoundingBox().c != glm::vec3(0.0f)) {
					if (PhysicsEngine::isColliding(environmentList.at(i)->collisionComponent, projectileList.at(j)->collisionComponent)) {
						if (projectileList.at(j)->GetClipping() == PROJECTILE_CLIP::YES_WALL_PLAYER ||
							projectileList.at(j)->GetClipping() == PROJECTILE_CLIP::YES_WALL_PROJECTILE ||
							projectileList.at(j)->GetClipping() == PROJECTILE_CLIP::YES_WALL ||
							projectileList.at(j)->GetClipping() == PROJECTILE_CLIP::YES) {
							projectileList.at(j)->deleted = true;
						}
					}
				}
			}
		}


		// Collisions with other players' projectiles
		proSize = projectileList.size();
		for (int i = 1; i < proSize; ++i) {
			if (projectileList.at(i - 1) != projectileList.at(i) && !IsSameTeam(*projectileList.at(i - 1), *projectileList.at(i))) {
				if (projectileList.at(i - 1)->collisionComponent != NULL && projectileList.at(i)->collisionComponent != NULL &&
					projectileList.at(i - 1)->collisionComponent->GetBoundingBox().c != glm::vec3(0.0f) &&
					projectileList.at(i)->collisionComponent->GetBoundingBox().c != glm::vec3(0.0f)) {
					if (PhysicsEngine::isColliding(projectileList.at(i - 1)->collisionComponent, projectileList.at(i)->collisionComponent)) {
						ProjectileCollision(*projectileList.at(i - 1), *projectileList.at(i));
					}
				}
			}
		}
	}
}

