#include "Light.h"

Light::Light()
{
	m_ambiantColor = Color(0.1f, 0.1f, 0.1f, 1.0f);
	m_diffuseColor = Color(1, 1, 1, 1);
	m_specularColor = Color(1, 1, 1, 1);
	m_intensity = 1.0f;
	m_lightType = LightType::Directional;
}