#include "SpotLight.h"

SpotLight::SpotLight() :Light()
{
	m_lightType = LightType::Spot;
	m_linearValue = 1.0f;
	m_constantValue = 1.0f;
	m_quadraticValue = 1.0f;

	m_spotAngle = 45;
	m_spotSmoothValue = 0.1f;
}
