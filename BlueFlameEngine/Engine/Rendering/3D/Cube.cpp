#include "Cube.h"

using namespace ENGINE;

Cube::Cube()
{
	renderComponent = new RenderComponent();
	renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
}

Cube::~Cube() {

}

void Cube::Update(const float deltaTime) {

}

void Cube::Render(Shader* shader) 
{
	renderComponent->Render(shader);
}