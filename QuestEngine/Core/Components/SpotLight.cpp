#include "SpotLight.h"

SpotLightComponent::SpotLightComponent() :LightComponent()
{
	m_lightType = LightType::Spot;
	m_linearValue = 1.0f;
	m_constantValue = 1.0f;
	m_quadraticValue = 1.0f;

	m_spotAngle = 45;
	m_spotSmoothValue = 0.1f;
}

SpotLightComponent::SpotLightComponent(const SpotLightComponent& other) : LightComponent(other)
{
	m_spotAngle = other.m_spotAngle;
	m_spotSmoothValue = other.m_spotSmoothValue;
	m_constantValue = other.m_constantValue;
	m_linearValue = other.m_linearValue;
	m_quadraticValue = other.m_quadraticValue;
}

Component* SpotLightComponent::Clone()
{
	SpotLightComponent* spotLightComponent = new SpotLightComponent(*this);
	clonnedObject = spotLightComponent;
	clonnedObject->baseObject = this;
	return spotLightComponent;
}

void SpotLightComponent::AssignPointerAndReference()
{
	LightComponent::AssignPointerAndReference();
}
