#include "ProjectileManager.h"
using namespace ENGINE;

namespace GAME {

	ProjectileManager::ProjectileManager() {

	}

	void ProjectileManager::AddProjectile(Projectile* _projectile) {
		projectileList.push_back(_projectile);
	}

	void ProjectileManager::AddPlayer(Cube* _player) {
		playerList.push_back(_player);
	}

	void ProjectileManager::RemoveProjectile(Projectile* _projectile) {
		projectileList.erase(std::remove(projectileList.begin(), projectileList.end(), _projectile), projectileList.end());
	}

	void ProjectileManager::RemovePlayer(Cube* _player) {
		playerList.erase(std::remove(playerList.begin(), playerList.end(), _player), playerList.end());
	}

	std::vector<Projectile*> ProjectileManager::GetProjectileList() {
		return projectileList;
	}

	void ProjectileManager::Update(float deltaTime) {
		int proSize = projectileList.size();
		int plaSize = playerList.size();

		for (int i = 0; i < plaSize; i++) {
			for (int j = 0; j < proSize; j++) {
				if (PhysicsEngine::isColliding(playerList.at(i)->collisionComponent, projectileList.at(j)->collisionComponent)) {
					//Add forces here
					std::cout << "Projectile Collision" << std::endl;
					projectileList.at(j)->deleted = true;
					playerList.at(i)->physicsComponent->AddForce(projectileList.at(j)->GetForce());
				}
			}
		}
	}
}