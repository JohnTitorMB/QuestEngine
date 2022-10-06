#include "Camera.h"

Camera::Camera(Vector2D position, float size, bool isHorizontal)
{
	m_position = position;
	m_size = size;
	m_isHorizontal = isHorizontal;	
}

Vector2D Camera::GetVerticalAndHorizontalSize(float width, float height)
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

float Camera::GetSize()const
{
	return m_size;
}

bool Camera::GetSizeType()const
{
	return m_isHorizontal;
}
