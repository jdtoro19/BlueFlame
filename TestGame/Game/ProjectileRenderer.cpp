#include "ProjectileRenderer.h"

using namespace ENGINE;
using namespace GAME;

ProjectileRenderer::ProjectileRenderer() {
	cubeMesh = new RenderComponent();
	cubeMesh->SetRenderType(RenderComponent::Render_Type::CUBE);

	/*
	FIX THE FOLLOWING
	--------------------------
	normfire		looking right
	stalagmites		looking up
	normEarth		looking right
	normIce			looking right
	iceUlt			looking up
	normElec		looking right
	normWind		looking right
	*/

	normFire = new Model("Resources/Models/Projectiles/Fire/Projectile_F_0001.obj");
	normFire->SetWorldScale(0.2f);
	fireMeteor = new Model("Resources/Models/Projectiles/Fire/Projectile_Meteor_0001.obj");
	fireMeteor->SetWorldScale(0.025f);

	normEarth = new Model("Resources/Models/Projectiles/Earth/Projectile_S_0001.obj");
	normEarth->SetWorldScale(0.2f);
	earthStalagmites = new Model("Resources/Models/Projectiles/Earth/Projectile_EarthSpike_0001.obj");
	earthStalagmites->SetWorldScale(0.005f);
	earthUlt = new Model("Resources/Models/Projectiles/Earth/Projectile_Wall_Ice.obj");
	earthUlt->SetWorldScale(0.010f);

	//STILL NEEDS TO BE TEXTURED
	normIce = new Model("Resources/Models/Projectiles/Ice/Projectile_I_0001.obj");
	normIce->SetWorldScale(0.2f);
	iceRamp = new Model("Resources/Models/Projectiles/Ice/Projectile_Ramp_Ice.obj");
	iceRamp->SetWorldScale(0.010f);
	iceWall = new Model("Resources/Models/Projectiles/Ice/Projectile_Wall_Ice.obj");
	iceWall->SetWorldScale(0.010f);
	iceUlt = new Model("Resources/Models/Projectiles/Ice/Projectile_Icicle.obj");
	iceUlt->SetWorldScale(0.005f);

	normElec = new Model("Resources/Models/Projectiles/Elec/Projectile_E_0001.obj");
	normElec->SetWorldScale(0.2f);
	elecDisc = new Model("Resources/Models/Projectiles/Elec/Projectile_Shurniken_0001.obj");
	elecDisc->SetWorldScale(0.010f);

	normWind = new Model("Resources/Models/Projectiles/Wind/Projectile_W_0001.obj");
	normWind->SetWorldScale(0.2f);
	windDisc = new Model("Resources/Models/Projectiles/Wind/Projectile_Shurniken_0001.obj");
	windDisc->SetWorldScale(0.010f);
	windWall = new Model("Resources/Models/Projectiles/Wind/Projectile_Wall_Ice.obj");
	windWall->SetWorldScale(0.010f);

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
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::NORM_FIRE)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				normFire->GetWorldModelMatrix() * normFire->GetLocalModelMatrix());
			normFire->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::FIRE_METEOR)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				fireMeteor->GetWorldModelMatrix() * fireMeteor->GetLocalModelMatrix());
			fireMeteor->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::NORM_EARTH)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				normEarth->GetWorldModelMatrix() * normEarth->GetLocalModelMatrix());
			normEarth->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::EARTH_STALAGMITES)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				earthStalagmites->GetWorldModelMatrix() * earthStalagmites->GetLocalModelMatrix());
			earthStalagmites->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::EARTH_ULT)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				earthUlt->GetWorldModelMatrix() * earthUlt->GetLocalModelMatrix());
			earthUlt->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::NORM_ICE)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				normIce->GetWorldModelMatrix() * normIce->GetLocalModelMatrix());
			normIce->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::ICE_RAMP)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				iceRamp->GetWorldModelMatrix() * iceRamp->GetLocalModelMatrix());
			iceRamp->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::ICE_WALL)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				iceWall->GetWorldModelMatrix() * iceWall->GetLocalModelMatrix());
			iceWall->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::ICE_ULT)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				iceUlt->GetWorldModelMatrix() * iceUlt->GetLocalModelMatrix());
			iceUlt->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::NORM_ELEC)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				normElec->GetWorldModelMatrix() * normElec->GetLocalModelMatrix());
			normElec->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::ELEC_DISC)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				elecDisc->GetWorldModelMatrix() * elecDisc->GetLocalModelMatrix());
			elecDisc->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::NORM_WIND)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				normWind->GetWorldModelMatrix() * normWind->GetLocalModelMatrix());
			normWind->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::WIND_DISC)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				windDisc->GetWorldModelMatrix() * windDisc->GetLocalModelMatrix());
			windDisc->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::WIND_WALL)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				windWall->GetWorldModelMatrix() * windWall->GetLocalModelMatrix());
			windWall->model->Render(shader);
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
