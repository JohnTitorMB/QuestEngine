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

Vector3D Transform::GetPosition()const
{
	return m_position;
}

Vector3D Transform::GetScale()const
{
	return m_scale;
}

Vector3D Transform::GetEulerAngles()const
{
	return m_rotation.ToEulerAngle();
}

Quaternion Transform::GetRotation()const
{
	return m_rotation;
}

Vector3D Transform::TransformPosition(const Vector3D& v)const
{
	return m_rotation* (Vector3D(v.m_x * m_scale.m_x, v.m_y * m_scale.m_y, v.m_z * m_scale.m_z)) + m_position;
}

Vector3D Transform::InverseTransformPosition(const Vector3D& v)const
{
	return m_scale.GetSafeInvertedVector() * (m_rotation.Inverse() * (v - m_position));
}

Vector3D Transform::TransformVector(const Vector3D& v)const
{
	return m_rotation * m_scale * v;
}

Vector3D Transform::InverseTransformVector(const Vector3D& v)const
{
	return m_scale.GetSafeInvertedVector() * (m_rotation.Inverse() * v);
}

Vector3D Transform::TransformVectorWithoutScale(const Vector3D& v) const
{
	return m_rotation * v;
}

Vector3D Transform::InverseTransformVectorWithoutScale(const Vector3D& v) const
{
	return m_rotation.Inverse() * v;
}
