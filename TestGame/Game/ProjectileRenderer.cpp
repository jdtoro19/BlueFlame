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

void ProjectileRenderer::Render(Shader* shader) {
	for (unsigned int i = 0; i < projectiles.size(); ++i) {
		shader->setMat4("model", projectiles.at(i)->GetWorldModelMatrix() * projectiles.at(i)->GetLocalModelMatrix());
		cubeMesh->Render(shader);
	}
	for (unsigned int i = 0; i < spawnedProjectiles.size(); ++i) {
		shader->setMat4("model", spawnedProjectiles.at(i)->GetWorldModelMatrix() * spawnedProjectiles.at(i)->GetLocalModelMatrix());
		cubeMesh->Render(shader);
	}
}
