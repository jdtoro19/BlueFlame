#include "Model.h"

using namespace ENGINE;

Model::Model()
{
}

Model::Model(std::string path)
{
	model = new ModelComponent(path);
	isModel = true;
}

Model::~Model()
{
}

void Model::Update(const float deltaTime) {

}

void Model::Render(Shader* shader) {
	model->Render(shader);
}