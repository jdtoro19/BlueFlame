#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../../Core/GameObject.h"
#include "../../Core/RenderComponent.h"
#include "../../Math/CollisionComponent.h"
#include "../../Math/PhysicsComponent.h"
#include "../../Math/PhysicsEngine.h"
#include "../../Graphics/Shader.h"

namespace ENGINE {

	class Projectile : public GameObject {
	private:

		// Direction where the player is facing
		// Affects all other forces and their direction
		int dir;

		// Force acting on the projectile during motion
		glm::vec3 actingForce;

		// Force given to player when projectile makes contact
		glm::vec3 knockbackForce;

		float stunTime;

		int damage;

	public:

		// Default Constructor requires the start position, launch force, and direction
		Projectile(glm::vec3 p, glm::vec3 _force, int _dir);
		~Projectile();

		// Sets the acting force on the projectile during motion
		void SetActingForce(glm::vec3 _force);

		// Sets the knockback force applied to the player when hit
		void SetKnockbackForce(glm::vec3 _force);

		void SetStunTime(float time);

		void SetDamage(int d);

		// Gets the knockback force applied to the player when hit
		// Required in the projectile manager
		glm::vec3 GetForce();

		float GetStunTime();
		int GetDamage();

		void Update(const float deltaTime);
		void FixedUpdate(const float deltaTime);
		void Render(Shader* shader);

		int rip = NULL;
		RenderComponent* renderComponent;

		// Enable this bool to create zig zag effects
		// Acting Force will flip in the x axis every flip interval 
		bool canFlipX;
		bool canFlipY;

		// The amount of time between every flip during zig cag effects
		// Measured in seconds
		float flipIntervalX;
		float flipIntervalY;
		float flipTimeX;
		float flipTimeY;

	};

}
#endif
