#include "ProjectileRenderer.h"

using namespace ENGINE;
using namespace GAME;

ProjectileRenderer::ProjectileRenderer() {
	cubeMesh = new RenderComponent();
	cubeMesh->SetRenderType(RenderComponent::Render_Type::CUBE);

	meteor = new Model("Resources/Models/Projectiles/Fire/Projectile_Meteor_0001.obj");
	meteor->SetWorldScale(0.025f);

	projectileShader = new Shader("Shaders/model.vs", "Shaders/model.fs");
	shader = BFEngine::GetInstance()->GetSceneManager()->GetRenderer()->GetShaderManager()->put(std::string("projectile"), projectileShader);
}

ProjectileRenderer::~ProjectileRenderer() {

}

void ProjectileRenderer::Update(const float deltaTime) {

}

void ProjectileRenderer::Render(Shader* shader, const double _interpolation) {

	glm::mat4 interpolatedMatrix;

	for (unsigned int i = 0; i < projectiles.size(); ++i) {

		// Account for interpolation
		interpolatedMatrix = (projectiles.at(i)->GetWorldModelMatrix() * (float)_interpolation) + (projectiles.at(i)->GetPreviousWorldMatrix() * (1.0f - (float)_interpolation));

		// Don't interpolate on the first render 
		if (projectiles.at(i)->firstRender == true) {
			interpolatedMatrix = projectiles.at(i)->GetWorldModelMatrix();
		}
		
		if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::CUBE)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix());
			cubeMesh->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::METEOR)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() * 
							meteor->GetWorldModelMatrix() * meteor->GetLocalModelMatrix());
			meteor->model->Render(shader);
		}
	}
	for (unsigned int i = 0; i < spawnedProjectiles.size(); ++i) {

		// Account for interpolation
		interpolatedMatrix = spawnedProjectiles.at(i)->GetWorldModelMatrix() * (float)_interpolation + spawnedProjectiles.at(i)->GetPreviousWorldMatrix() * (1.0f - (float)_interpolation);

		// Don't interpolate on the first render 
		if (spawnedProjectiles.at(i)->firstRender == true) {
			interpolatedMatrix = spawnedProjectiles.at(i)->GetWorldModelMatrix();
		}
		shader->setMat4("model", interpolatedMatrix * spawnedProjectiles.at(i)->GetLocalModelMatrix());
		cubeMesh->Render(shader);
	}
}
