#include "Cube.h"

using namespace ENGINE;

Cube::Cube()
{
	renderComponent = new RenderComponent();
	renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
}

Cube::~Cube() {

}

void Cube::Update() {

}

void Cube::Render(Shader* shader) 
{
	renderComponent->Render(shader);
}