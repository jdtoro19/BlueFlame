#include "ProjectileManager.h"
using namespace ENGINE;

namespace GAME {

	ProjectileManager::ProjectileManager() {
		projectileRenderer = new ProjectileRenderer();
	}

	void ProjectileManager::AddProjectile(Projectile* _projectile) {
		_projectile->CreateCollision(projectileRenderer->GetCubeMesh());
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

	void ProjectileManager::AddSpawnedProjectile(Projectile* _projectile) {
		spawnedProjectiles.push_back(_projectile);
	}

	void ProjectileManager::RemoveSpawnedProjectile(Projectile* _projectile) {
		spawnedProjectiles.erase(std::remove(spawnedProjectiles.begin(), spawnedProjectiles.end(), _projectile), spawnedProjectiles.end());
	}

	std::vector<Projectile*> ProjectileManager::GetProjectileList() {
		return projectileList;
	}

	void ProjectileManager::Update(float deltaTime) {
		projectileRenderer->SetProjectileList(projectileList);
		projectileRenderer->SetSpawnedProjectileList(spawnedProjectiles);

		for (unsigned int i = 0; i < projectileList.size(); ++i) {
			projectileList.at(i)->Update(deltaTime);
		}
		for (unsigned int i = 0; i < spawnedProjectiles.size(); ++i) {
			spawnedProjectiles.at(i)->Update(deltaTime);
		}

		int proSize = projectileList.size();
		int plaSize = playerList.size();

		// Collisions with player
		for (int i = 0; i < plaSize; i++) {
			for (int j = 0; j < proSize; j++) {
				if (playerList.at(i)->collisionComponent != NULL && projectileList.at(j)->collisionComponent != NULL) {
					if (PhysicsEngine::isColliding(playerList.at(i)->collisionComponent, projectileList.at(j)->collisionComponent)) {
						if (projectileList.at(j)->GetClipping() == PROJECTILE_CLIP::YES_WALL_PLAYER ||
							projectileList.at(j)->GetClipping() == PROJECTILE_CLIP::YES_PLAYER_PROJECTILE ||
							projectileList.at(j)->GetClipping() == PROJECTILE_CLIP::YES_PLAYER ||
							projectileList.at(j)->GetClipping() == PROJECTILE_CLIP::YES) {
							projectileList.at(j)->deleted = true;
							playerList.at(i)->Hit(projectileList.at(j));
						}
					}
				}
			}
		}

		proSize = projectileList.size();
		int envSize = environmentList.size();

		// Collisions with environment
		for (int i = 0; i < envSize; i++) {
			for (int j = 0; j < proSize; j++) {
				if (environmentList.at(i)->collisionComponent != NULL && projectileList.at(j)->collisionComponent != NULL) {
					if (PhysicsEngine::isColliding(environmentList.at(i)->collisionComponent, projectileList.at(j)->collisionComponent)) {
						if (projectileList.at(j)->GetClipping() == PROJECTILE_CLIP::YES_WALL_PLAYER || 
							projectileList.at(j)->GetClipping() == PROJECTILE_CLIP::YES_WALL_PROJECTILE ||
							projectileList.at(j)->GetClipping() == PROJECTILE_CLIP::YES_WALL ||
							projectileList.at(j)->GetClipping() == PROJECTILE_CLIP::YES) {
							projectileList.at(j)->deleted = true;
						}

						if (projectileList.at(j)->GetStrength() == PROJECTILE_STRENGTH::HEAVY && projectileList.at(j)->GetElement() == PROJECTILE_ELEMENT::EARTH) {
							Projectile *p = new Projectile(projectileList.at(j)->GetWorldPosition(), 0.0f, 0.0f);
							p->physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::STATIC);
							p->CreateCollision(projectileRenderer->GetCubeMesh());
							p->collisionComponent->SetLayer(0);
							p->SetClipping(PROJECTILE_CLIP::YES);
							p->physicsComponent->destructible = false;
							p->SetFirstDelay(1.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(2.0f, 2.0f, 0.75f), glm::vec3(2.0f, 2.0f, 0.75f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
							p->SetLifetime(3.0);

							AddSpawnedProjectile(p);							
						}
					}
				}
			}
		}

		proSize = projectileList.size();
		int spawnSize = spawnedProjectiles.size();
		
		// Collisions with spawned projectiles
		for (int i = 0; i < spawnSize; i++) {
			for (int j = 0; j < proSize; j++) {
					if (PhysicsEngine::isColliding(spawnedProjectiles.at(i)->collisionComponent, projectileList.at(j)->collisionComponent)) {
						projectileList.at(j)->deleted = true;
				}
			}
		}



		spawnSize = spawnedProjectiles.size();
		// Spawned projectile garbage collection
		for (int i = 0; i < spawnSize; i++) {
			if (spawnedProjectiles.at(i)->deleted == true) {
				RemoveSpawnedProjectile(spawnedProjectiles.at(i));
			}
		}
	}
}