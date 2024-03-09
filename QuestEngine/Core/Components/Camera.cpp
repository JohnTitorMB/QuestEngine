#include "Camera.h"
#include <iostream>
# define M_PI           3.14159265358979323846  /* pi */

CameraComponent::CameraComponent(Vector3D position, Quaternion rotation, float size, bool isPerspective, bool isHorizontal)
{
	m_position = position;
	m_rotation = rotation;
	m_size = size;
	m_isHorizontal = isHorizontal;	
	m_isPerspective = isPerspective;
}


Vector2D CameraComponent::GetVerticalAndHorizontalSize(float width, float height)const
{
	if (m_isHorizontal)
		return Vector2D(m_size, m_size * height / width);
	else
		return Vector2D(m_size * width / height, m_size);
}

void CameraComponent::SetPosition(const Vector3D& position)
{
	m_position = position;
}

void CameraComponent::SetRotation(const Quaternion& rotation)
{
	m_rotation = rotation;
}

void CameraComponent::SetProjectionMode(EProjectionMode _projectionMode)
{
	if (_projectionMode == EProjectionMode::PERSPECTIVE)
		m_isPerspective = true;
	else
		m_isPerspective = false;
}


void CameraComponent::SetSizeType(bool isHorizontal)
{
	m_isHorizontal = isHorizontal;
}

void CameraComponent::SetSize(float size)
{
	m_size = size;
}
void CameraComponent::SetNear(float near)
{
	m_near = near;
}

void CameraComponent::SetFar(float far)
{
	m_far = far;
}

void CameraComponent::SetFov(float fov)
{
	float fovInRadian = fov * M_PI / 180.0f;

	m_size = 2.0f * m_near * tan(fovInRadian / 2.0f);
}

Vector3D CameraComponent::GetPosition()const
{
	return m_position;
}

Quaternion CameraComponent::GetRotation()const
{
	return m_rotation;
}

float CameraComponent::GetSize()const
{
	return m_size;
}

bool CameraComponent::GetSizeType()const
{
	return m_isHorizontal;
}

float CameraComponent::GetNear()const
{
	return m_near;
}


float CameraComponent::GetFar()const
{
	return m_far;
}

Matrix4x4 CameraComponent::ViewMatrix()const
{
	return Matrix4x4::ScaleXYZ(Vector3D(1.0f, 1.0f,-1.0f)) * Matrix4x4::Rotate(m_rotation).Inverse() * Matrix4x4::Translate(m_position).Inverse();
}

Matrix4x4 CameraComponent::ProjectionMatrix(float windowWidth, float windowHeight)const
{
	Vector2D size = GetVerticalAndHorizontalSize(windowWidth, windowHeight);

	if (m_isPerspective)
		return Matrix4x4::Perspective(-size.m_x / 2.0f, size.m_x / 2.0f, -size.m_y / 2.0f, size.m_y / 2.0f, m_near, m_far);
	else
		return Matrix4x4::Orthographic(-size.m_x / 2.0f, size.m_x / 2.0f, -size.m_y / 2.0f, size.m_y / 2.0f, m_near, m_far);
}