#include "Vector3D.h"
#include <math.h>

Vector3D::Vector3D()
{
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
}

Vector3D::Vector3D(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

Vector3D::Vector3D(const Vector3D& value)
{
	m_x = value.m_x;
	m_y = value.m_y;
	m_z = value.m_z;
}

Vector3D Vector3D::operator+(const Vector3D& value) const
{
	return Vector3D(m_x + value.m_x, m_y + value.m_y, m_z + value.m_z);
}

Vector3D Vector3D::operator*(const float& value) const
{
	return Vector3D(m_x * value, m_y * value, m_z * value);
}

Vector3D Vector3D::operator-(const Vector3D& value) const
{
	return Vector3D(m_x - value.m_x, m_y - value.m_y, m_z - value.m_z);
}

Vector3D Vector3D::operator/(const float& value) const
{
	float v = 1.0f / value;
	return Vector3D(m_x * v, m_y * v, m_z * v);
}

float Vector3D::Magnitude() const
{
	return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z);
}

void Vector3D::Normalize()
{
	float magnitude = Magnitude();
	m_x /= magnitude;
	m_y /= magnitude;
	m_z /= magnitude;
}

Vector3D& Vector3D::operator+=(const Vector3D& value)
{
	m_x += value.m_x;
	m_y += value.m_y;
	m_z += value.m_z;

	return (*this);
}

Vector3D& Vector3D::operator*=(float value)
{
	m_x *= value;
	m_y *= value;
	m_z *= value;

	return (*this);
}

Vector3D& Vector3D::operator-=(const Vector3D& value)
{
	m_x -= value.m_x;
	m_y -= value.m_y;
	m_z -= value.m_z;

	return (*this);
}

Vector3D& Vector3D::operator/=(float value)
{
	m_x /= value;
	m_y /= value;
	m_z /= value;

	return (*this);
}

bool Vector3D::operator==(const Vector3D& value) const
{
	const float epsilon = 1e-5; // ajustez selon vos besoins

	return (abs(m_x - value.m_x) < epsilon && abs(m_y - value.m_y) < epsilon && abs(m_z - value.m_z) < epsilon);
}

bool Vector3D::operator!=(const Vector3D& value) const
{
	const float epsilon = 1e-5; // ajustez selon vos besoins
	return (abs(m_x - value.m_x) > epsilon || abs(m_y - value.m_y) > epsilon || abs(m_z - value.m_z) > epsilon);
}


Vector3D Vector3D::Normalized() const
{
	return (*this) / Magnitude();
}

float Vector3D::DotProduct(const Vector3D& a, const Vector3D& b)
{
	return a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z;
}

Vector3D Vector3D::CrossProduct(const Vector3D& a, const Vector3D& b)
{
	return Vector3D(a.m_y * b.m_z - b.m_y * a.m_z, a.m_z * b.m_x - b.m_z * a.m_x, a.m_x * b.m_y - b.m_x * a.m_y);
}

std::ostream& operator<<(std::ostream& os, const Vector3D& value)
{
	os << "(X : " << value.m_x << ", Y : " << value.m_y << ", Z : " << value.m_z << ")" << std::endl;
	return os;
}
