#include "Transform.h"

TransformComponent::TransformComponent()
{
	m_position = Vector3D(0.0f,0.0f,0.0f);
	m_scale = Vector3D(1.0f, 1.0f, 1.0f);
	m_rotation = Quaternion::Identity();
}

TransformComponent::TransformComponent(Vector3D position, Vector3D scale, Quaternion rotation)
{
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;
}


Matrix4x4 TransformComponent::TransformMatrix()const
{
	return Matrix4x4::TRS(m_position, m_rotation, m_scale);
}

void TransformComponent::SetPosition(const Vector3D& position)
{
	m_position = position;
}

void TransformComponent::SetScale(const Vector3D& scale)
{
	m_scale = scale;
}

void TransformComponent::SetEulerAngles(const Vector3D& eulerAngles)
{
	m_rotation = Quaternion::FromEulerAngle(eulerAngles);
}

void TransformComponent::SetRotation(const Quaternion& rotation)
{
	m_rotation = rotation;
}

Vector3D TransformComponent::GetPosition()
{
	return m_position;
}

Vector3D TransformComponent::GetScale()
{
	return m_scale;
}

Vector3D TransformComponent::GetEulerAngles()
{
	return m_rotation.ToEulerAngle();
}

Quaternion TransformComponent::GetRotation()
{
	return m_rotation;
}