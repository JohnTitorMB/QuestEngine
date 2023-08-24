#include "Camera.h"
#include <iostream>
# define M_PI           3.14159265358979323846  /* pi */

Camera::Camera(Vector3D position, Vector3D angles, float size, bool isPerspective, bool isHorizontal)
{
	m_position = position;
	m_angles = angles;
	m_size = size;
	m_isHorizontal = isHorizontal;	
	m_isPerspective = isPerspective;
}

Vector2D Camera::GetVerticalAndHorizontalSize(float width, float height)const
{
	if (m_isHorizontal)
		return Vector2D(m_size, m_size * height / width);
	else
		return Vector2D(m_size * width / height, m_size);
}

void Camera::SetPosition(const Vector3D& position)
{
	m_position = position;
}

void Camera::SetAngle(const Vector3D& angles)
{
	m_angles = angles;
}


void Camera::SetSizeType(bool isHorizontal)
{
	m_isHorizontal = isHorizontal;
}

void Camera::SetSize(float size)
{
	m_size = size;
}
void Camera::SetNear(float near)
{
	m_near = near;
}

void Camera::SetFar(float far)
{
	m_far = far;
}

void Camera::SetFov(float fov)
{
	float fovInRadian = fov * M_PI / 180.0f;

	m_size = 2.0f * m_near * tan(fovInRadian / 2.0f);
}

Vector3D Camera::GetPosition()const
{
	return m_position;
}

Vector3D  Camera::GetAngles()const
{
	return m_angles;
}

float Camera::GetSize()const
{
	return m_size;
}

bool Camera::GetSizeType()const
{
	return m_isHorizontal;
}

float Camera::GetNear()const
{
	return m_near;
}


float Camera::GetFar()const
{
	return m_far;
}

Matrix4x4 Camera::ViewMatrix()const
{
	return Matrix4x4::ScaleXYZ(Vector3D(1.0f, 1.0f,-1.0f)) * Matrix4x4::Rotate(m_angles).Inverse() * Matrix4x4::Translate(m_position).Inverse();
}

Matrix4x4 Camera::ProjectionMatrix(float windowWidth, float windowHeight)const
{
	Vector2D size = GetVerticalAndHorizontalSize(windowWidth, windowHeight);

	if (m_isPerspective)
		return Matrix4x4::Perspective(-size.m_x / 2.0f, size.m_x / 2.0f, -size.m_y / 2.0f, size.m_y / 2.0f, m_near, m_far);
	else
		return Matrix4x4::Orthographic(-size.m_x / 2.0f, size.m_x / 2.0f, -size.m_y / 2.0f, size.m_y / 2.0f, m_near, m_far);
}