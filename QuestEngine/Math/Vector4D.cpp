#include "Vector4D.h"
#include <math.h>

Vector4D::Vector4D()
{
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
	m_w = 0.0f;
}

Vector4D::Vector4D(float x, float y, float z, float w)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_w = w;
}

Vector4D::Vector4D(const Vector4D& value)
{
	m_x = value.m_x;
	m_y = value.m_y;
	m_z = value.m_z;
	m_w = value.m_w;
}

Vector4D Vector4D::operator+(const Vector4D& value) const
{
	return Vector4D(m_x + value.m_x, m_y + value.m_y, m_z + value.m_z, m_w + value.m_w);
}

Vector4D Vector4D::operator*(const float& value) const
{
	return Vector4D(m_x * value, m_y * value, m_z * value, m_w * value);
}

Vector4D Vector4D::operator-(const Vector4D& value) const
{
	return Vector4D(m_x - value.m_x, m_y - value.m_y, m_z - value.m_z, m_w - value.m_w);
}

Vector4D Vector4D::operator/(const float& value) const
{
	float v = 1.0f / value;
	return Vector4D(m_x * v, m_y * v, m_z * v, m_w * v);
}

float Vector4D::Magnitude() const
{
	return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
}

void Vector4D::Normalize()
{
	float magnitude = Magnitude();
	m_x /= magnitude;
	m_y /= magnitude;
	m_z /= magnitude;
	m_w /= magnitude;
}

Vector4D& Vector4D::operator+=(const Vector4D& value)
{
	m_x += value.m_x;
	m_y += value.m_y;
	m_z += value.m_z;
	m_w += value.m_w;

	return (*this);
}

Vector4D& Vector4D::operator*=(float value)
{
	m_x *= value;
	m_y *= value;
	m_z *= value;
	m_w *= value;

	return (*this);
}

Vector4D& Vector4D::operator-=(const Vector4D& value)
{
	m_x -= value.m_x;
	m_y -= value.m_y;
	m_z -= value.m_z;
	m_w -= value.m_w;

	return (*this);
}

Vector4D& Vector4D::operator/=(float value)
{
	m_x /= value;
	m_y /= value;
	m_z /= value;
	m_w /= value;

	return (*this);
}

bool Vector4D::operator==(const Vector4D& value) const
{
	return (abs(m_x - value.m_x) < FLT_EPSILON && abs(m_y - value.m_y) < FLT_EPSILON && abs(m_z - value.m_z) < FLT_EPSILON && abs(m_w - value.m_w) < FLT_EPSILON);
}

bool Vector4D::operator!=(const Vector4D& value) const
{
	return (abs(m_x - value.m_x) > FLT_EPSILON || abs(m_y - value.m_y) > FLT_EPSILON || abs(m_z - value.m_z) > FLT_EPSILON || abs(m_w - value.m_w) > FLT_EPSILON);
}


Vector4D Vector4D::Normalized() const
{
	return (*this) / Magnitude();
}

float Vector4D::DotProduct(const Vector4D& a, const Vector4D& b)
{
	return a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z + a.m_w * b.m_w;
}


std::ostream& operator<<(std::ostream& os, const Vector4D& value)
{
	os << "(X : " << value.m_x << ", Y : " << value.m_y << ", Z : " << value.m_z << ", W : " << value.m_w << ")" << std::endl;
	return os;
}
