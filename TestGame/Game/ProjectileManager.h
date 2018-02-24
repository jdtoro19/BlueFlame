#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

#include <vector>
#include <BlueFlameEngine\Engine\Math\PhysicsEngine.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Projectile.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Cube.h>
#include "../Player/Player.h"

using namespace ENGINE;

namespace GAME {
	class ProjectileManager {
	private:
		std::vector<Projectile*> projectileList;
		std::vector<Player*> playerList;
	public:
		ProjectileManager();

		void AddProjectile(Projectile* _projectile);
		void AddPlayer(Player* _player);
		void RemoveProjectile(Projectile* _projectile);
		void RemovePlayer(Player* _player);

		std::vector<Projectile*> GetProjectileList();

		void Update(float deltaTime);

	};
}
#endif
