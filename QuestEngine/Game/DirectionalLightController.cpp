#include "DirectionalLightController.h"
#include "../Math/Quaternion.h"
#include "../Core/Entity.h"
#include <iostream>
DirectionalLightControllerComponent::DirectionalLightControllerComponent() : Component()
{
	m_directionalLightComponent = nullptr;

}

void DirectionalLightControllerComponent::Start()
{
	Component::Start();

//	Entity* entity = GetOwnEntity();
//	m_directionalLightComponent = entity->GetComponent<DirectionalLightComponent>();
}

void DirectionalLightControllerComponent::Update()
{
	Component::Update();
	if(m_directionalLightComponent != nullptr)
		m_directionalLightComponent->m_direction = Quaternion::AxisAngle(Vector3D(0, 1, 0).Normalized(), 1.0f) * m_directionalLightComponent->m_direction;

}

void DirectionalLightControllerComponent::SetDirectionalLightComponent(DirectionalLightComponent* directionalLightComponent)
{
	m_directionalLightComponent = directionalLightComponent;
}

