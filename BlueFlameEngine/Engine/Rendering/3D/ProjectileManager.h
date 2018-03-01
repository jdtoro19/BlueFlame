#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

#include <vector>
#include <BlueFlameEngine\Engine\Math\PhysicsEngine.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Projectile.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Cube.h>

using namespace ENGINE;

namespace GAME {
	class ProjectileManager {
	private:
		std::vector<Projectile*> projectileList;
		std::vector<Cube*> playerList;
	public:
		ProjectileManager();

		void AddProjectile(Projectile* _projectile);
		void AddPlayer(Cube* _player);
		void RemoveProjectile(Projectile* _projectile);
		void RemovePlayer(Cube* _player);

		std::vector<Projectile*> GetProjectileList();

		void Update(float deltaTime);

	};
}
#endif
