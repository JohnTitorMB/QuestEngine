#include "PointLight.h"

PointLightComponent::PointLightComponent() :LightComponent()
{
	m_lightType = LightType::Point;
	m_linearValue = 1.0f;
	m_constantValue = 1.0f;
	m_quadraticValue = 1.0f;
}