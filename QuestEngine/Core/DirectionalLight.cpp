#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	m_lightType = LightType::Directional;
	m_direction = Vector3D::Forward;
	
}