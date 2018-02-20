#include "PlayerBase.h"

using namespace GAME;

PlayerBase::PlayerBase()
{
	renderComponent = new RenderComponent();
	renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
}

PlayerBase::~PlayerBase()
{
}

void PlayerBase::Update(const float deltaTime)
{
}

void PlayerBase::Render(Shader* shader)
{
	renderComponent->Render(shader);
}