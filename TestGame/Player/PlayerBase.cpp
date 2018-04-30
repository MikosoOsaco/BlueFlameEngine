#include "PlayerBase.h"

using namespace GAME;

PlayerBase::PlayerBase()
{
	renderComponent = new RenderComponent();
	renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
}

PlayerBase::PlayerBase(std::string path) 
{
	model = new Model(path, true);
	model->SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), 1.5708f);

	renderComponent = new RenderComponent();
	renderComponent->SetRenderType(RenderComponent::Render_Type::CUBE);
	renderComponent->CanRender(false);
}

PlayerBase::~PlayerBase()
{
}

void PlayerBase::Update(const float deltaTime)
{
}

void PlayerBase::Render(Shader* shader, const double _interpolation)
{
	if (renderComponent != NULL) {
		renderComponent->Render(shader);
	}

	if (model != NULL) {
		model->Render(shader, _interpolation);
	}
}