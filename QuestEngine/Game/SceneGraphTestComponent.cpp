#include "SceneGraphTestComponent.h"

SceneGraphTestComponent::SceneGraphTestComponent()
{
	m_sceneComponent = nullptr;
	childRotation = Quaternion::Identity();
}

SceneGraphTestComponent::SceneGraphTestComponent(const SceneGraphTestComponent& other)
{
	m_isAttachToParent1 = other.m_isAttachToParent1;
	childRotation = other.childRotation;
}

void SceneGraphTestComponent::Start()
{
	m_sceneComponent->SetWorldRotation(Quaternion(0.906315f, 0, 0.422623f, 0));
	m_sceneComponent2->SetWorldRotation(Quaternion(0.906315f, 0.422623f, 0, 0));
}

void SceneGraphTestComponent::Update()
{
	if (m_sceneComponent)
	{
		bool assignParentTest = false;

		if (m_isAttachToParent1)
		{
			Quaternion currentRotation = m_sceneComponent->GetWorldRotation();
			m_sceneComponent->SetWorldRotation(currentRotation * Quaternion::AxisAngle(Vector3D(0, 1, 0).Normalized(), 1.0f));
		}

		if (!m_isAttachToParent1)
		{
			Quaternion currentRotation2 = m_sceneComponent2->GetWorldRotation();
			m_sceneComponent2->SetWorldRotation(currentRotation2 * Quaternion::AxisAngle(Vector3D(1, 0, 0).Normalized(), 1.0f));
		}

		childRotation = childRotation * Quaternion::AxisAngle(Vector3D(0, 1, 0).Normalized(), -1.0f);
		m_sceneComponentChild->SetWorldRotation(childRotation);

		Quaternion currentRelativeRotation2 = m_sceneComponentChild2->GetRelativeRotation();
		m_sceneComponentChild2->SetRelativeRotation(currentRelativeRotation2 * Quaternion::AxisAngle(Vector3D(1, 0, 0).Normalized(), -2.0f));

		time += 0.01f;
		time2 += 0.01f;
		
		if (!assignParentTest)
		{
			float scale = 1.0f + Mathf::PingPong(time, 1.0f);
			m_sceneComponent->SetWorldScale(Vector3D(scale, scale, scale));
		}

		if (assignParentTest && time2 > 5.0f)
		{
			time2 = 0.0f;

			if (m_isAttachToParent1)
			{
				m_sceneComponentChild->SetParent(m_sceneComponent2);
				m_isAttachToParent1 = false;
			}
			else
			{
				m_sceneComponentChild->SetParent(m_sceneComponent);
				m_isAttachToParent1 = true;
			}
		}
	}
}

void SceneGraphTestComponent::SetSceneComponent(SceneComponent* sceneComponent)
{
	m_sceneComponent = sceneComponent;
}

void SceneGraphTestComponent::SetSceneComponent2(SceneComponent* sceneComponent2)
{
	m_sceneComponent2 = sceneComponent2;
}

void SceneGraphTestComponent::SetSceneComponentChild(SceneComponent* sceneComponentChild)
{
	m_sceneComponentChild = sceneComponentChild;
}

void SceneGraphTestComponent::SetSceneComponentChild2(SceneComponent* sceneComponentChild2)
{
	m_sceneComponentChild2 = sceneComponentChild2;
}

Component* SceneGraphTestComponent::Clone()
{
	SceneGraphTestComponent* sceneGraphTestComponent = new SceneGraphTestComponent(*this);
	clonnedObject = sceneGraphTestComponent;
	clonnedObject->baseObject = this;
	return sceneGraphTestComponent;
}

void SceneGraphTestComponent::AssignPointerAndReference()
{
	Component::AssignPointerAndReference();

	SceneGraphTestComponent* baseSGTestComponent = (SceneGraphTestComponent*)baseObject;

	if (baseSGTestComponent->m_sceneComponent != nullptr)
		m_sceneComponent = (SceneComponent*)baseSGTestComponent->m_sceneComponent->clonnedObject;

	if (baseSGTestComponent->m_sceneComponent2 != nullptr)
		m_sceneComponent2 = (SceneComponent*)baseSGTestComponent->m_sceneComponent2->clonnedObject;

	if (baseSGTestComponent->m_sceneComponentChild != nullptr)
		m_sceneComponentChild = (SceneComponent*)baseSGTestComponent->m_sceneComponentChild->clonnedObject;

	if (baseSGTestComponent->m_sceneComponentChild2 != nullptr)
		m_sceneComponentChild2 = (SceneComponent*)baseSGTestComponent->m_sceneComponentChild2->clonnedObject;
}
