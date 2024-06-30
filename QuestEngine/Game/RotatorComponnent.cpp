#include "RotatorComponnent.h"
#include "../Math/Quaternion.h"
#include "../Core/Entity.h"
RotatorComponnent::RotatorComponnent() : Component()
{

}

RotatorComponnent::RotatorComponnent(const RotatorComponnent& rotatorComponnent) : Component(rotatorComponnent)
{
}

void RotatorComponnent::Start()
{
	Component::Start();
}


void RotatorComponnent::Update()
{
	Component::Update();
	Quaternion rotation = GetOwnEntity()->GetRootComponent()->GetWorldRotation();
	rotation = rotation * Quaternion::FromEulerAngle(Vector3D(0, 1.0f, 0));
	GetOwnEntity()->GetRootComponent()->SetWorldRotation(rotation);
}

RotatorComponnent::~RotatorComponnent()
{
}

Component* RotatorComponnent::Clone()
{
	RotatorComponnent* rotatorComponnent = new RotatorComponnent(*this);
	clonnedObject = rotatorComponnent;
	clonnedObject->baseObject = this;
	return rotatorComponnent;
}

void RotatorComponnent::AssignPointerAndReference()
{
	Component::AssignPointerAndReference();
}
