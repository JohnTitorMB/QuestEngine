#include "SceneComponent.h"
#include <iostream>
#include "../Entity.h"
#include "../World.h"
SceneComponent::SceneComponent()
{
	m_parent = nullptr;
	m_relativePosition = Vector3D(0, 0, 0);
	m_relativeRotation = Quaternion::Identity();
	m_relativeScale = Vector3D(1, 1, 1);
}

SceneComponent::SceneComponent(const SceneComponent& sceneComponent) : Component(sceneComponent)
{
	m_relativePosition = sceneComponent.m_relativePosition;
	m_relativeRotation = sceneComponent.m_relativeRotation;
	m_relativeScale = sceneComponent.m_relativeScale;
	m_transform = sceneComponent.m_transform;
	m_parent = sceneComponent.m_parent;

	for (SceneComponent* child : m_childs)
		m_childs.insert(child);
}

void SceneComponent::SetParent(SceneComponent* sceneComponent)
{
	if (m_parent == sceneComponent)
		return;

	if (m_parent == this)
		return;

	if (IsChildExist(sceneComponent))
		return;

	if (GetOwnEntity()->GetRootComponent() == this && GetOwnEntity() == sceneComponent->GetOwnEntity())
		return;

	if (m_parent != nullptr)
		m_parent->m_childs.erase(this);

	m_parent = sceneComponent;

	if (m_parent != nullptr)
	{
		m_relativePosition = m_parent->GetTransform().InverseTransformPosition(m_transform.m_position);
		m_relativeRotation = m_parent->GetTransform().GetRotation().Inverse() * m_transform.m_rotation;
		m_relativeScale = m_parent->GetTransform().GetScale().GetSafeInvertedVector() * m_transform.m_scale;
		m_parent->m_childs.insert(this);
	}
	else
	{
		m_relativePosition = m_transform.m_position;
		m_relativeRotation = m_transform.m_rotation;
		m_relativeScale = m_transform.m_scale;
	}


}
bool SceneComponent::IsChildExist(SceneComponent* childSceneComponent)
{
	if (m_childs.find(childSceneComponent) != m_childs.end())
		return true;

	bool findChild = false;

	for (auto it = m_childs.begin(); it != m_childs.end(); ++it)
	{
		SceneComponent* component = *it;
		if (component->IsChildExist(childSceneComponent))
			return true;
	}

	return false;
}


const Transform& SceneComponent::GetTransform()const
{
	return m_transform;
}

void SceneComponent::SetRelativePosition(const Vector3D& relativePosition)
{
	m_relativePosition = relativePosition;

	Vector3D worldPosition = relativePosition;
	Vector3D currentWorldPosition = m_transform.m_position;
	
	if (m_parent)
		worldPosition = m_parent->GetTransform().TransformPosition(m_relativePosition);

	m_transform.SetPosition(worldPosition);

	MoveChildWorldPosition(worldPosition - currentWorldPosition);
}

void SceneComponent::MoveChildWorldPosition(Vector3D delta)
{
	for (SceneComponent* sC : m_childs)
	{
		sC->m_transform.SetPosition(sC->m_transform.m_position + delta);
		sC->MoveChildWorldPosition(delta);
	}
}

void SceneComponent::RefreshChildWorldPosition()
{
	for (SceneComponent* sC : m_childs)
	{
		sC->m_transform.SetPosition(GetTransform().TransformPosition(sC->m_relativePosition));
		sC->RefreshChildWorldPosition();
	}
}

void SceneComponent::RefreshChildWorldRotation()
{
	for (SceneComponent* sC : m_childs)
	{
		sC->m_transform.SetRotation(GetTransform().m_rotation * sC->m_relativeRotation);
		sC->RefreshChildWorldRotation();
	}
}

void SceneComponent::RefreshChildWorldScale()
{
	for (SceneComponent* sC : m_childs)
	{
		sC->m_transform.SetScale(GetTransform().m_scale * sC->m_relativeScale);
		sC->RefreshChildWorldScale();
	}
}

void SceneComponent::SetRelativeRotation(const Quaternion& relativeRotation)
{
	m_relativeRotation = relativeRotation;

	Quaternion worldRotation = relativeRotation;

	if (m_parent)
		worldRotation = m_parent->GetTransform().m_rotation * relativeRotation;

	m_transform.SetRotation(worldRotation);

	RefreshChildWorldPosition();
	RefreshChildWorldRotation();
}

void SceneComponent::SetRelativeScale(const Vector3D& relativeScale)
{
	m_relativeScale = relativeScale;

	Vector3D worldScale = relativeScale;
	if (m_parent)
		worldScale = m_parent->GetTransform().m_scale * relativeScale;

	m_transform.SetScale(worldScale);


	RefreshChildWorldPosition();
	RefreshChildWorldScale();
}

void SceneComponent::SetWorldPosition(const Vector3D& worldPosition)
{
	Vector3D currentWorldPosition = m_transform.m_position;
	m_transform.SetPosition(worldPosition);

	if (m_parent != nullptr)
	{
		const Transform& parentTR = m_parent->GetTransform();
		m_relativePosition = parentTR.InverseTransformPosition(worldPosition);
	}
	else
		m_relativePosition = worldPosition;

	MoveChildWorldPosition(worldPosition - currentWorldPosition);
}

void SceneComponent::SetWorldRotation(const Quaternion& worldRotation)
{
	m_transform.SetRotation(worldRotation);

	if (m_parent != nullptr)
	{
		const Transform& parentTR = m_parent->GetTransform();
		m_relativeRotation = parentTR.GetRotation().Inverse() * worldRotation;
	}
	else
		m_relativeRotation = worldRotation;

	RefreshChildWorldPosition();
	RefreshChildWorldRotation();
}

void SceneComponent::SetWorldScale(const Vector3D& worldScale)
{
	m_transform.SetScale(worldScale);

	if (m_parent != nullptr)
	{
		const Transform& parentTR = m_parent->GetTransform();
		m_relativeScale = parentTR.GetScale().GetSafeInvertedVector() * worldScale;
	}
	else
		m_relativeScale = worldScale;

	RefreshChildWorldPosition();
	RefreshChildWorldScale();
}

Component* SceneComponent::Clone()
{
	SceneComponent* sceneComponent = new SceneComponent(*this);	
	clonnedObject = sceneComponent;
	clonnedObject->baseObject = this;
	return sceneComponent;
}

void SceneComponent::AssignPointerAndReference()
{
	Component::AssignPointerAndReference();

	SceneComponent* sceneComponent = (SceneComponent*)baseObject;
	if (sceneComponent->m_parent != nullptr)
		m_parent = (SceneComponent*)sceneComponent->m_parent->clonnedObject;	

	m_childs.clear();
	for (auto it = sceneComponent->m_childs.begin(); it != sceneComponent->m_childs.end(); ++it)
	{
		SceneComponent* child = *it;
		m_childs.insert((SceneComponent*)child->clonnedObject);
	}
}

Vector3D SceneComponent::GetWorldPosition() const
{
	return GetTransform().GetPosition();
}

Quaternion SceneComponent::GetWorldRotation() const
{
	return GetTransform().GetRotation();
}

Vector3D SceneComponent::GetWorldScale() const
{
	return GetTransform().GetScale();
}

Vector3D SceneComponent::GetRelativePosition() const
{
	return m_relativePosition;
}

Quaternion SceneComponent::GetRelativeRotation() const
{
	return m_relativeRotation;
}

Vector3D SceneComponent::GetRelativeScale() const
{
	return m_relativeScale;
}

Vector3D SceneComponent::GetForwardVector() const
{
	return GetTransform().TransformVectorWithoutScale(Vector3D::Forward);
}

Vector3D SceneComponent::GetRightVector() const
{
	return GetTransform().TransformVectorWithoutScale(Vector3D::Right);
}

Vector3D SceneComponent::GetUpVector() const
{
	return GetTransform().TransformVectorWithoutScale(Vector3D::Up);
}
