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
	class ProjectileManager {
	private:
		// Lists of projectiles and players/environments they can come into contact with
		std::vector<Projectile*> projectileList;
		std::vector<Player*> playerList;
		std::vector<GameObject*> environmentList;

		// List of projectiles that were spawned in the game scene from other projectiles
		std::vector<Projectile*> spawnedProjectiles;

		// Projectile Renderer 
		ProjectileRenderer* projectileRenderer;

	public:
		ProjectileManager();

		// Functions to add and remove the corresponding objects from the list
		void AddProjectile(Projectile* _projectile);
		void AddPlayer(Player* _player);
		void AddEnvironment(GameObject* _environment);
		void RemoveProjectile(Projectile* _projectile);
		void RemovePlayer(Player* _player);
		void RemoveEnvironment(GameObject* _environment);

	private:
		void AddSpawnedProjectile(Projectile* _projectile);
		void RemoveSpawnedProjectile(Projectile* _projectile);

	public:

		std::vector<Projectile*> GetProjectileList();

		ProjectileRenderer* GetProjectileRenderer() { return projectileRenderer; };

		void Update(float deltaTime);

	};
}
#endif
