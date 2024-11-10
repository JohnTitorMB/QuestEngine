#include "PointLight.h"

PointLightComponent::PointLightComponent() :LightComponent()
{
	m_lightType = LightType::Point;
	m_linearValue = 1.0f;
	m_constantValue = 1.0f;
	m_quadraticValue = 1.0f;
}

PointLightComponent::PointLightComponent(const PointLightComponent& other) : LightComponent(other)
{
	m_linearValue = other.m_linearValue;
	m_constantValue = other.m_constantValue;
	m_quadraticValue = other.m_quadraticValue;
}

Component* PointLightComponent::Clone()
{
	PointLightComponent* pointLightComponent = new PointLightComponent(*this);
	clonnedObject = pointLightComponent;
	clonnedObject->baseObject = this;
	return pointLightComponent;
}

void PointLightComponent::AssignPointerAndReference()
{
	LightComponent::AssignPointerAndReference();
}
