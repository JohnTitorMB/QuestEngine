#include "DirectionalLightController.h"
#include "../Math/Quaternion.h"
#include "../Core/Entity.h"
#include <iostream>
DirectionalLightControllerComponent::DirectionalLightControllerComponent() : Component()
{
	m_directionalLightComponent = nullptr;

}

DirectionalLightControllerComponent::DirectionalLightControllerComponent(const DirectionalLightControllerComponent& other)
{
	m_directionalLightComponent = nullptr;
}

void DirectionalLightControllerComponent::Start()
{
	Component::Start();
}

void DirectionalLightControllerComponent::Update()
{
	Component::Update();
	if (m_directionalLightComponent != nullptr)
	{
		Quaternion currentRotation = m_directionalLightComponent->GetWorldRotation();
		m_directionalLightComponent->SetWorldRotation(currentRotation* Quaternion::AxisAngle(Vector3D(0, 1, 0).Normalized(), 1.0f));
	}
}

void DirectionalLightControllerComponent::SetDirectionalLightComponent(DirectionalLightComponent* directionalLightComponent)
{
	m_directionalLightComponent = directionalLightComponent;
}

Component* DirectionalLightControllerComponent::Clone()
{
	DirectionalLightControllerComponent* controller = new DirectionalLightControllerComponent(*this);
	clonnedObject = controller;
	clonnedObject->baseObject = this;
	return controller;
}

void DirectionalLightControllerComponent::AssignPointerAndReference()
{
	Component::AssignPointerAndReference();
	DirectionalLightControllerComponent* baseController = (DirectionalLightControllerComponent*)baseObject;

	if (baseController->m_directionalLightComponent != nullptr)
		m_directionalLightComponent = (DirectionalLightComponent*)baseController->m_directionalLightComponent->clonnedObject;
}

