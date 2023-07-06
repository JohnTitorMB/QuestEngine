#include "Camera.h"

Camera::Camera(Vector2D position, float angle, float size, bool isHorizontal)
{
	m_position = position;
	m_angle = angle;
	m_size = size;
	m_isHorizontal = isHorizontal;	
}

Vector2D Camera::GetVerticalAndHorizontalSize(float width, float height)const
{
	if (m_isHorizontal)
		return Vector2D(m_size, m_size * height / width);
	else
		return Vector2D(m_size * width / height, m_size);
}

void Camera::SetPosition(const Vector2D& position)
{
	m_position = position;
}

void Camera::SetAngle(const float& angle)
{
	m_angle = angle;
}


void Camera::SetSizeType(bool isHorizontal)
{
	m_isHorizontal = isHorizontal;
}

void Camera::SetSize(float size)
{
	m_size = size;
}

Vector2D Camera::GetPosition()const
{
	return m_position;
}

float Camera::GetAngle()const
{
	return m_angle;
}

float Camera::GetSize()const
{
	return m_size;
}

bool Camera::GetSizeType()const
{
	return m_isHorizontal;
}

Matrix3x3 Camera::ViewMatrix()const
{
	return Matrix3x3::RotateZ(m_angle).Inverse() * Matrix3x3::Translate(m_position).Inverse();
}

Matrix3x3 Camera::ToNDCSpaceMatrix(float windowWidth, float windowHeight)const
{
	Vector2D size = GetVerticalAndHorizontalSize(windowWidth, windowHeight);

	return Matrix3x3::ScaleXY(Vector2D(2.0f / size.m_x, 2.0f / size.m_y));
}