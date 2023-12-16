#include "Transform.h"

Transform::Transform()
{
	m_position = Vector3D(0.0f,0.0f,0.0f);
	m_scale = Vector3D(1.0f, 1.0f, 1.0f);
	m_rotation = Quaternion::Identity();
}

Transform::Transform(Vector3D position, Vector3D scale, Quaternion rotation)
{
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;
}

Matrix4x4 Transform::TransformMatrix()const
{
	return Matrix4x4::TRS(m_position, m_rotation, m_scale);
}

void Transform::SetPosition(const Vector3D& position)
{
	m_position = position;
}

void Transform::SetScale(const Vector3D& scale)
{
	m_scale = scale;
}

void Transform::SetEulerAngles(const Vector3D& eulerAngles)
{
	m_rotation = Quaternion::FromEulerAngle(eulerAngles);
}

void Transform::SetRotation(const Quaternion& rotation)
{
	m_rotation = rotation;
}

Vector3D Transform::GetPosition()
{
	return m_position;
}

Vector3D Transform::GetScale()
{
	return m_scale;
}

Vector3D Transform::GetEulerAngles()
{
	return m_rotation.ToEulerAngle();
}

Quaternion Transform::GetRotation()
{
	return m_rotation;
}