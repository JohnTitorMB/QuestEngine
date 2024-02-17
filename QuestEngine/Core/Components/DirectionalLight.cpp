#include "DirectionalLight.h"

DirectionalLightComponent::DirectionalLightComponent() : LightComponent()
{
	m_lightType = LightType::Directional;
	m_direction = Vector3D::Forward;
	
}

DirectionalLightComponent::~DirectionalLightComponent()
{

}