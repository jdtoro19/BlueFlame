#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../../Core/GameObject.h"
#include "../../Core/RenderComponent.h"
#include "../../Math/CollisionComponent.h"
#include "../../Math/PhysicsComponent.h"
#include "../../Math/PhysicsEngine.h"
#include "../../Graphics/Shader.h"
#include <glm/gtx/vector_angle.hpp>

namespace ENGINE {

	class Projectile : public GameObject {
	private:

		// Force acting on the projectile during motion
		glm::vec3 actingForce;

		// Force given to player when projectile makes contact
		glm::vec3 knockbackForce;

		// Angle to the target
		float angle;

		// Direction the player is facing
		int dir;

		// Stun time given to player when projectile makes contact
		float stunTime;

		// Damage given to player when projectile makes contact
		int damage;

	public:

		// Default Constructor requires the start position, launch force, target angle, and direction
		Projectile(glm::vec3 p, glm::vec3 _force, float _angle, int _dir);
		~Projectile();

		// Sets the acting force on the projectile during motion
		void SetActingForce(glm::vec3 _force);

		// Sets the knockback force applied to the player when hit
		void SetKnockbackForce(glm::vec3 _force);

		// Sets the stun time applied to the player when hit
		void SetStunTime(float time);

		// Sets the damage applied to the player when hit
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
