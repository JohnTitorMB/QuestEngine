#include "DirectionalLightEntity.h"
#include "../Math/Quaternion.h"
#include <iostream>

DirectionalLightEntity::DirectionalLightEntity() : Entity()
{
	m_directionalLightComponent = nullptr;
}

DirectionalLightEntity::DirectionalLightEntity(const DirectionalLightEntity& other) : Entity(other)
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
	{
		Quaternion currentRotation = m_directionalLightComponent->GetWorldRotation();
		m_directionalLightComponent->SetWorldRotation(currentRotation * Quaternion::AxisAngle(Vector3D(0, 1, 0).Normalized(), 1.0f));
	}
}

void DirectionalLightEntity::SetDirectionalLightComponent(DirectionalLightComponent* directionalLightComponent)
{
	m_directionalLightComponent = directionalLightComponent;
}

Entity* DirectionalLightEntity::Clone(bool isAddedOnScene)
{
	DirectionalLightEntity* directionalLightEntity = new DirectionalLightEntity(*this);
	clonnedObject = directionalLightEntity;
	clonnedObject->baseObject = this;
	return directionalLightEntity;
}

void DirectionalLightEntity::AssignPointerAndReference()
{
	Entity::AssignPointerAndReference();
	DirectionalLightEntity* directionalLightentityBase = (DirectionalLightEntity*)baseObject;

	if (directionalLightentityBase->m_directionalLightComponent != nullptr)
		m_directionalLightComponent = (DirectionalLightComponent*)directionalLightentityBase->m_directionalLightComponent->clonnedObject;

	Entity::AssignPointerAndReference();
}

