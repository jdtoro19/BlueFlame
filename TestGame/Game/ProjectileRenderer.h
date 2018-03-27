#pragma once
#ifndef PROJECTILERENDERER_H
#define PROJECTILERENDERER_H

#include <BlueFlameEngine\Engine\Core\RenderComponent.h>
#include <BlueFlameEngine\Engine\Core\GameObject.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Projectile.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Model.h>
#include <BlueFlameEngine\Engine\BFEngine.h>

using namespace ENGINE;

namespace GAME {

	// This class renders all projectiles in the scene
	class ProjectileRenderer : public GameObject {

	public:

		// Constructors 
		ProjectileRenderer();
		~ProjectileRenderer();

		void Update(const float deltaTime);
		void Render(Shader* shader, const double _interpolation);

		RenderComponent* GetCubeMesh() { return cubeMesh; };

		void SetProjectileList(std::vector<Projectile*> projectileList) { projectiles = projectileList; };
		void SetSpawnedProjectileList(std::vector<Projectile*> projectileList) { spawnedProjectiles = projectileList; };
	
	private:

		// The mesh to be used for rendering 
		RenderComponent* cubeMesh;
		Model* meteor;
		// List of projectiles in the scene
		std::vector<Projectile*> projectiles;
		std::vector<Projectile*> spawnedProjectiles;
		// Projectiles shader
		Shader* projectileShader;

	};
}

#endif 
