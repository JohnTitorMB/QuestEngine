#include "Vector2D.h"
#include <math.h>

Vector2D::Vector2D()
{
	m_x = 0.0f;
	m_y = 0.0f;
}

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

float Vector2D::Magnitude() const
{
	return sqrtf(m_x * m_x + m_y * m_y);
}

void Vector2D::Normalize()
{
	float magnitude = Magnitude();
	m_x /= magnitude;
	m_y /= magnitude;
}

Vector2D& Vector2D::operator+=(const Vector2D& value)
{
	m_x += value.m_x;
	m_y += value.m_y;

	return (*this);
}

Vector2D& Vector2D::operator*=(float value)
{
	m_x *= value;
	m_y *= value;

	return (*this);
}

Vector2D& Vector2D::operator-=(const Vector2D& value)
{
	m_x -= value.m_x;
	m_y -= value.m_y;

	return (*this);
}

Vector2D& Vector2D::operator/=(float value)
{
	m_x /= value;
	m_y /= value;

	return (*this);
}

bool Vector2D::operator==(const Vector2D& value) const
{
	return (m_x == value.m_x && m_y == value.m_y);
}

bool Vector2D::operator!=(const Vector2D& value) const
{
	return (m_x != value.m_x || m_y != value.m_y);
}

Vector2D::Vector2D(const Vector2D& value)
{
	m_x = value.m_x;
	m_y = value.m_y;
}

Vector2D Vector2D::Normalized() const
{
	return (*this) / Magnitude();
}
std::ostream& operator<<(std::ostream& os, const Vector2D& value)
{
	os << "(X : " << value.m_x << ", Y : " << value.m_y << ")" << std::endl;
	return os;
}
