#include "GameObject.h"

using namespace ENGINE;

GameObject::GameObject() : 
	localPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	worldPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	localRotation(glm::vec3(1.0f, 1.0f, 1.0f)),
	worldRotation(glm::vec3(1.0f, 1.0f, 1.0f)),
	localScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	worldScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	localRotationAngle(0.0f),
	worldRotationAngle(0.0f)
{
}

GameObject::~GameObject()
{
}

// local position
void GameObject::SetLocalPosition(const float &x, const float &y, const float &z)
{
	localPosition = glm::vec3(x, y, z);
}
void GameObject::SetLocalPosition(const glm::vec3 &p)
{
	localPosition = p;
}
glm::vec3 GameObject::GetLocalPosition() const
{
	return localPosition;
}

// local rotation
void GameObject::SetLocalRotation(const float &x, const float &y, const float &z, const float &a)
{
	localRotation = glm::vec3(x, y, z);
	localRotationAngle = a;
}
void GameObject::SetLocalRotation(const glm::vec3 &r, const float &a)
{
	localRotation = r;
	localRotationAngle = a;
}
glm::vec3 GameObject::GetLocalRotation() const
{
	return localRotation;
}

// local scale
void GameObject::SetLocalScale(const float &x, const float &y, const float &z)
{
	localScale = glm::vec3(x, y, z);
}
void GameObject::SetLocalScale(const glm::vec3 &s)
{
	localScale = s;
}
void GameObject::SetLocalScale(const float &s)
{
	localScale = glm::vec3(s, s, s);
}
glm::vec3 GameObject::GetLocalScale() const
{
	return localScale;
}

// local rotation angle
float GameObject::GetLocalRotationAngle() const
{
	return localRotationAngle;
}

// world position
void GameObject::SetWorldPosition(const float &x, const float &y, const float &z)
{
	worldPosition = glm::vec3(x, y, z);
}
void GameObject::SetWorldPosition(const glm::vec3 &p)
{
	worldPosition = p;
}
glm::vec3 GameObject::GetWorldPosition() const
{
	return worldPosition;
}

// world rotation
void GameObject::SetWorldRotation(const float &x, const float &y, const float &z, const float &a)
{
	worldRotation = glm::vec3(x, y, z);
	worldRotationAngle = a;
}
void GameObject::SetWorldRotation(const glm::vec3 &r, const float &a)
{
	worldRotation = r;
	worldRotationAngle = a;
}
glm::vec3 GameObject::GetWorldRotation() const
{
	return worldRotation;
}

// world scale
void GameObject::SetWorldScale(const float &x, const float &y, const float &z)
{
	worldScale = glm::vec3(x, y, z);
}
void GameObject::SetWorldScale(const glm::vec3 &s)
{
	worldScale = s;
}
void GameObject::SetWorldScale(const float &s)
{
	worldScale = glm::vec3(s, s, s);

}
glm::vec3 GameObject::GetWorldScale() const
{
	return worldScale;
}

// world rotation angle
float GameObject::GetWorldRotationAngle() const
{
	return worldRotationAngle;
}

void GameObject::SetIsModel(bool b)
{
	isModel = b;
}
bool GameObject::GetIsModel() {
	return isModel;
}

void GameObject::Rotate(float angle, glm::vec3 rot) {
	modelMatrix = glm::rotate(modelMatrix, angle, rot);
}

void GameObject::Update(const float deltaTime)
{
}

void GameObject::Render(Shader* shader)
{
}