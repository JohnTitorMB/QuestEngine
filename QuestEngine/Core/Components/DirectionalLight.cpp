#include "DirectionalLight.h"

DirectionalLightComponent::DirectionalLightComponent() : LightComponent()
{
	m_lightType = LightType::Directional;	
}

DirectionalLightComponent::DirectionalLightComponent(const DirectionalLightComponent& other) : LightComponent(other)
{

}

DirectionalLightComponent::~DirectionalLightComponent()
{

}

Component* DirectionalLightComponent::Clone()
{
	DirectionalLightComponent* directionalLightComponent = new DirectionalLightComponent(*this);
	clonnedObject = directionalLightComponent;
	clonnedObject->baseObject = this;
	return directionalLightComponent;
}

void DirectionalLightComponent::AssignPointerAndReference()
{
	LightComponent::AssignPointerAndReference();
}
