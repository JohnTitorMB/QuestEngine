#include "Vector2.h"

Vector2D::Vector2D(float x, float y)
{
	m_x = x;
	m_y = y;
}

Vector2D Vector2D::operator+(const Vector2D& value) const
{
	return Vector2D(m_x + value.m_x, m_y + value.m_y);
}

Vector2D Vector2D::operator*(const float& value) const
{
	return Vector2D(m_x * value, m_y * value);
}