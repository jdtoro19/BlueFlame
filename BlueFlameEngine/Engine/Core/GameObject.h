#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Graphics/Shader.h"

namespace ENGINE {

	class GameObject
	{
	protected:
		// Local variables
		glm::vec3 localPosition;
		glm::vec3 localRotation;
		glm::vec3 localScale;
		float localRotationAngle;

		// World variables
		glm::vec3 worldPosition;
		glm::vec3 worldRotation;
		glm::vec3 worldScale;
		float worldRotationAngle;

		bool isModel = false;

	public:
		GameObject();
		~GameObject();

		// Local getters and setters
		// position
		virtual void SetLocalPosition(const float &x, const float &y, const float &z);
		virtual void SetLocalPosition(const glm::vec3 &p);
		virtual glm::vec3 GetLocalPosition() const;
		// rotation
		virtual void SetLocalRotation(const float &x, const float &y, const float &z, const float &a);
		virtual void SetLocalRotation(const glm::vec3 &r, const float &a);
		virtual glm::vec3 GetLocalRotation() const;
		// scale
		virtual void SetLocalScale(const float &x, const float &y, const float &z);
		virtual void SetLocalScale(const glm::vec3 &s);
		virtual void SetLocalScale(const float &s);
		virtual glm::vec3 GetLocalScale() const;
		// angle
		virtual float GetLocalRotationAngle() const;

		// World getters and setters
		// position
		virtual void SetWorldPosition(const float &x, const float &y, const float &z);
		virtual void SetWorldPosition(const glm::vec3 &p);
		virtual glm::vec3 GetWorldPosition() const;
		// rotation
		virtual void SetWorldRotation(const float &x, const float &y, const float &z, const float &a);
		virtual void SetWorldRotation(const glm::vec3 &r, const float &a);
		virtual glm::vec3 GetWorldRotation() const;
		// scale
		virtual void SetWorldScale(const float &x, const float &y, const float &z);
		virtual void SetWorldScale(const glm::vec3 &s);
		virtual void SetWorldScale(const float &s);
		virtual glm::vec3 GetWorldScale() const;
		// angle
		virtual float GetWorldRotationAngle() const;

		void SetIsModel(bool b);
		bool GetIsModel();

		void Rotate(float angle, glm::vec3 rot);

		glm::mat4 modelMatrix = glm::mat4(); 
		virtual void Update(const float deltaTime);
		virtual void Render(Shader* shader);
	};

}
#endif
