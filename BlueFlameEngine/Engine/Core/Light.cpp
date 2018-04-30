#include "Light.h"

using namespace ENGINE;

Light::Light()
{
	lightComponent = new LightComponent();
	renderComponent = new RenderComponent();
	
}

Light::Light(LightComponent::Light_Type lt)
{
	lightComponent = new LightComponent(lt);
	renderComponent = new RenderComponent();
}

Light::~Light()
{

}

void Light::Render(Shader* shader, const double _interpolation)
{
	renderComponent->Render(shader);
}
