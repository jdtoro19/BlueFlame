#include "RenderComponent.h"

using namespace ENGINE;

RenderComponent::RenderComponent() : mesh(nullptr)
{
	colour = glm::vec3(0.5f, 0.5f, 0.5f);
	renderType = NONE;
}
RenderComponent::~RenderComponent()
{

}
void RenderComponent::SetRenderType(Render_Type rt) {
	if (rt == NONE) {
		renderType = NONE;
		vertexList.clear();
	}
	else if (rt == CUBE) {
		renderType = CUBE;
		vertexList.clear();
		v.color = colour;		
		// SIDE 1
		v.normal = glm::vec3(0.0f, 0.0f, -1.0f);
		v.position = glm::vec3(-0.5f, -0.5f, -0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, -0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, -0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, -0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, -0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, -0.5f, -0.5f);
		vertexList.push_back(v);
		// SIDE 2
		v.normal = glm::vec3(0.0f, 0.0f, 1.0f);
		v.position = glm::vec3(-0.5f, -0.5f, 0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, 0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, 0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, 0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, 0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, -0.5f, 0.5f);
		vertexList.push_back(v);
		// SIDE 3
		v.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
		v.position = glm::vec3(-0.5f, 0.5f, 0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, -0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, -0.5f, -0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, -0.5f, -0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, -0.5f, 0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, 0.5f);
		vertexList.push_back(v);
		// SIDE 4
		v.normal = glm::vec3(1.0f, 0.0f, 0.0f);
		v.position = glm::vec3(0.5f, 0.5f, 0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, -0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, -0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, -0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, 0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, 0.5f);
		vertexList.push_back(v);
		// SIDE 5
		v.normal = glm::vec3(0.0f, -1.0f, 0.0f);
		v.position = glm::vec3(-0.5f, -0.5f, -0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, -0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, 0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, 0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, -0.5f, 0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, -0.5f, -0.5f);
		vertexList.push_back(v);
		// SIDE 6
		v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
		v.position = glm::vec3(-0.5f, 0.5f, -0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, -0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, 0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, 0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, 0.5f);
		vertexList.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, -0.5f);
		vertexList.push_back(v);
		
		mesh = new Mesh(&vertexList);
	}
	else {

	}
}
void RenderComponent::SetColour(float r, float g, float b)
{
	colour = glm::vec3(r, g, b);
	SetRenderType(renderType);
}
void RenderComponent::Update() {

}
void RenderComponent::Render(Shader* shader) {
	if (mesh != nullptr) {
		mesh->Render();
	}
}