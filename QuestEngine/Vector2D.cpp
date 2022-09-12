#include "Vector2D.h"
#include <math.h>
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

Vector2D Vector2D::operator-(const Vector2D& value) const
{
	return Vector2D(m_x - value.m_x, m_y - value.m_y);
}

Vector2D Vector2D::operator/(const float& value) const
{
	float v = 1.0f / value;
	return Vector2D(m_x * v, m_y * v);
}

float Vector2D::Magnitude()
{
	return sqrtf(m_x * m_x + m_y * m_y);
}

void Vector2D::Normalize()
{
	float magnitude = Magnitude();
	m_x /= magnitude;
	m_y /= magnitude;
}