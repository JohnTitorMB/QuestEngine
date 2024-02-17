#include "DirectionalLightEntity.h"
#include "../Math/Quaternion.h"
#include <iostream>

DirectionalLightEntity::DirectionalLightEntity() : Entity()
{
	m_directionalLightComponent = nullptr;
}

void DirectionalLightEntity::Start()
{
	Entity::Start();
	//m_directionalLightComponent = GetComponent<DirectionalLightComponent>();
}

void DirectionalLightEntity::Update()
{
	Entity::Update();
	if (m_directionalLightComponent != nullptr)
		m_directionalLightComponent->m_direction = Quaternion::AxisAngle(Vector3D(0, 1, 0).Normalized(), 1.0f) * m_directionalLightComponent->m_direction;
}

void DirectionalLightEntity::SetDirectionalLightComponent(DirectionalLightComponent* directionalLightComponent)
{
	m_directionalLightComponent = directionalLightComponent;
}

