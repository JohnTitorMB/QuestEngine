#include "Light.h"

LightComponent::LightComponent()
{
	m_ambiantColor = ColorRGB(0.1f, 0.1f, 0.1f, 1.0f);
	m_diffuseColor = ColorRGB(1, 1, 1, 1);
	m_specularColor = ColorRGB(1, 1, 1, 1);
	m_intensity = 1.0f;
	m_lightType = LightType::Directional;
}

LightComponent::LightComponent(const LightComponent& other) : SceneComponent(other)
{
	m_ambiantColor = other.m_ambiantColor;
	m_diffuseColor = other.m_diffuseColor;
	m_specularColor = other.m_specularColor;
	m_intensity = other.m_intensity;
	m_lightType = other.m_lightType;
}

Component* LightComponent::Clone()
{
	LightComponent* lightComponent = new LightComponent(*this);
	clonnedObject = lightComponent;
	clonnedObject->baseObject = this;
	return lightComponent;
}

void LightComponent::AssignPointerAndReference()
{
	SceneComponent::AssignPointerAndReference();
}
