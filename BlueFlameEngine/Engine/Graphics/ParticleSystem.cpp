#include "ParticleSystem.h"

using namespace ENGINE;

ParticleSystem::ParticleSystem(ResourceManager<Shader>* rm) : mesh(nullptr)
{
	colour = glm::vec3(0.8f, 0.7f, 0.0f);
	lifeTime = 5.0f;
	loop = true;
	size = 2;
	alpha = 1.0f;
	glow = true;
	canRender = true;
	useImage = false;
	time = 0;
	particleShader = new Shader("Shaders/particle.vs", "Shaders/particle.fs");
	shader = rm->put(std::string("particle"), particleShader);
	SetUpMesh();
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::SetUpMesh() 
{
	Randomizer* rand = new Randomizer();

	for (size_t i = 0; i < 100; ++i) {
		v.position = glm::vec3(0.0f, 0.0f, 0.0f);
		// repurpose normal for velocity
		v.normal = glm::vec3(rand->box_muller(0, 1), rand->box_muller(15, 5), rand->box_muller(0, 1));
		v.color = glm::vec3(rand->box_muller(2, 0.5), 0, rand->box_muller(2, 0.5));
		vertexList.push_back(v);
	}
	mesh = new Mesh(&vertexList);
}

void ParticleSystem::Update(const float deltaTime) 
{
	time += deltaTime;
	if (loop) {
		if (time > lifeTime) {
			time = 0.0f;
		}
	}
	else 
	{
		if (time > lifeTime) {
			deleted = true;
		}
	}
}

void ParticleSystem::Render(Shader* shader, const double _interpolation)
{
	if (canRender) {
		if (mesh != nullptr) {
			glPointSize(size);
			shader->Use();
			shader->setFloat("Time", time);
			shader->setVec3("inColour", colour);
			shader->setFloat("Alpha", alpha);
			shader->setInt("Glow", glow);
			if (useImage) {
				mesh->Render(shader);
			}
			else 
			{
				mesh->RenderPoints(shader);
			}
		}
	}
}