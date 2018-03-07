#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../../Core/GameObject.h"
#include "../../Core/RenderComponent.h"
#include "../../Math/CollisionComponent.h"
#include "../../Math/PhysicsComponent.h"
#include "../../Math/PhysicsEngine.h"
#include "../../Graphics/Shader.h"
#include "../../Timers/Cooldown.h"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/compatibility.hpp>

namespace ENGINE {

	class Projectile : public GameObject {
	private:

		// Force used on the projectile at the beginning of motion
		glm::vec3 impulseForce;

		// Force acting on the projectile during motion
		glm::vec3 actingForce;

		// Force given to player when projectile makes contact
		glm::vec3 knockbackForce;

		// Angle to the target
		float angle;

		// Direction the player is facing
		int dir;

		// Delay before the projectile begins motion
		float delay;
		float delayTimer;

		// Stun time given to player when projectile makes contact
		float stunTime;

		// Damage given to player when projectile makes contact
		int damage;

		// Scale difference if desired when using delay, if delay is not used the method will do nothing
		glm::vec3 beginScale;
		glm::vec3 endScale;

		// Position difference if desired when using delay, if delay is not used the method will do nothing
		glm::vec3 endPosition;

	public:

		// Default Constructor requires the start position, launch force, target angle, and direction
		Projectile(glm::vec3 p, glm::vec3 _force, float _angle, int _dir);
		Projectile(glm::vec3 p, glm::vec3 _force, float _angle, int _dir, float _delay);
		~Projectile();

		// Sets the acting force on the projectile during motion
		void SetActingForce(glm::vec3 _force);

		// Sets the knockback force applied to the player when hit
		void SetKnockbackForce(glm::vec3 _force);

		// Sets the delay before the projectile is set into motion
		void SetDelayTime(float _time);

		// Sets the stun time applied to the player when hit
		void SetStunTime(float time);

		// Sets the damage applied to the player when hit
		void SetDamage(int d);

		// Sets the scale difference of the projectile when delayed
		void SetScaleChange(glm::vec3 _beginScale, glm::vec3 _endScale);

		// Sets the scale difference of the projectile when delayed
		void SetPositionChange(glm::vec3 _endPosition);

		// Gets the knockback force applied to the player when hit
		// Required in the projectile manager
		glm::vec3 GetForce();

		float GetStunTime();
		int GetDamage();

		void addMaxDistance(float distance);

		void setLifetime(double seconds);

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

		//use the addMaxDistance function to adjust these. if you do, object will not go beyond a certain distance
		bool maximumDistance = false;
		float maxD;
		glm::vec3 originalPos;

		//used for lifetime over the old rip

		Cooldown lifetime;
		Cooldown delayCD;

		bool hasFired;
	};

}
#endif
