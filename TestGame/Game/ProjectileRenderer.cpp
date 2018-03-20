#include "ProjectileRenderer.h"

using namespace ENGINE;
using namespace GAME;

ProjectileRenderer::ProjectileRenderer() {
	cubeMesh = new RenderComponent();
	cubeMesh->SetRenderType(RenderComponent::Render_Type::CUBE);

	projectileShader = new Shader("Shaders/model.vs", "Shaders/model.fs");
	shader = BFEngine::GetInstance()->GetSceneManager()->GetRenderer()->GetShaderManager()->put(std::string("projectile"), projectileShader);
}

ProjectileRenderer::~ProjectileRenderer() {

}

void ProjectileRenderer::Update(const float deltaTime) {

}

void ProjectileRenderer::Render(Shader* shader, const double _interpolation) {

	// Account for interpolation
	glm::mat4 interpolatedMatrix;

	for (unsigned int i = 0; i < projectiles.size(); ++i) {
		interpolatedMatrix = (projectiles.at(i)->GetWorldModelMatrix() * (float)_interpolation) + (projectiles.at(i)->GetPreviousWorldMatrix() * (1.0f - (float)_interpolation));
		shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix());
		cubeMesh->Render(shader);
	}
	for (unsigned int i = 0; i < spawnedProjectiles.size(); ++i) {
		interpolatedMatrix = spawnedProjectiles.at(i)->GetWorldModelMatrix() * (float)_interpolation + spawnedProjectiles.at(i)->GetPreviousWorldMatrix() * (1.0f - (float)_interpolation);
		shader->setMat4("model", interpolatedMatrix * spawnedProjectiles.at(i)->GetLocalModelMatrix());
		cubeMesh->Render(shader);
	}
}
