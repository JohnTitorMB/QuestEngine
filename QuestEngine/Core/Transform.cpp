#include "Transform.h"

Transform::Transform()
{
	m_position = Vector3D(0.0f,0.0f,0.0f);
	m_scale = Vector3D(0.0f, 0.0f, 0.0f);
	m_angles = Vector3D(0.0f, 0.0f, 0.0f);
}

Transform::Transform(Vector3D position, Vector3D scale, Vector3D angles)
{
	m_position = position;
	m_scale = scale;
	m_angles = angles;
}

Matrix4x4 Transform::TransformMatrix()const
{
	return Matrix4x4::TRS(m_position, m_angles, m_scale);
}