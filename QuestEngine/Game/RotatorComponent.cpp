#include "RotatorComponent.h"
#include "../Math/Quaternion.h"
#include "../Core/Entity.h"
#include "../Core/TimeManager.h"
RotatorComponent::RotatorComponent() : Component()
{

}



void RotatorComponent::Start()
{
	Component::Start();
}


void RotatorComponent::Update()
{
	Component::Update();
	Quaternion rotation = GetOwnEntity()->GetRootComponent()->GetWorldRotation();
	rotation = rotation * Quaternion::FromEulerAngle(Vector3D(0, m_speed* TimeManager::Instance()->GetDeltaTime(), 0));
	GetOwnEntity()->GetRootComponent()->SetWorldRotation(rotation);
}

RotatorComponent::~RotatorComponent()
{
}

Component* RotatorComponent::Clone()
{
	RotatorComponent* rotatorComponent = new RotatorComponent(*this);
	clonnedObject = rotatorComponent;
	clonnedObject->baseObject = this;
	return rotatorComponent;
}

void RotatorComponent::AssignPointerAndReference()
{
	Component::AssignPointerAndReference();
}