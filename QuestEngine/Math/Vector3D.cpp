#include "Vector3D.h"
#include <math.h>
#include "Mathf.h"
#include "Quaternion.h"

const Vector3D Vector3D::Right = Vector3D(1,0,0);
const Vector3D Vector3D::Up = Vector3D(0, 1, 0);
const Vector3D Vector3D::Forward = Vector3D(0, 0, 1);
const Vector3D Vector3D::Zero = Vector3D(0, 0, 0);

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

Vector3D Vector3D::operator*(const Vector3D& value) const
{
	return Vector3D(m_x * value.m_x, m_y * value.m_y, m_z * value.m_z);
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

float Vector3D::MagnitudeSquared() const
{
	return m_x * m_x + m_y * m_y + m_z * m_z;
}

void Vector3D::Normalize()
{
	float magnitude = Magnitude();
	m_x /= magnitude;
	m_y /= magnitude;
	m_z /= magnitude;
}

Vector3D Vector3D::Orthogonal(const Vector3D& v)
{
	Vector3D orthogonal;

	if (abs(v.m_x) < abs(v.m_y) && abs(v.m_x) < abs(v.m_z))
	{
		orthogonal = Vector3D(0, -v.m_z, v.m_y);
	}
	else if (abs(v.m_y) < abs(v.m_z))
	{
		orthogonal = Vector3D(-v.m_z, 0, v.m_x);
	}
	else
	{
		orthogonal = Vector3D(-v.m_y, v.m_x, 0);
	}

	// Normaliser le vecteur orthogonal
	orthogonal = orthogonal.Normalized();
	return orthogonal;
}

Vector3D& Vector3D::operator+=(const Vector3D& value)
{
	m_x += value.m_x;
	m_y += value.m_y;
	m_z += value.m_z;

	return (*this);
}

Vector3D& Vector3D::operator*=(const float& value)
{
	m_x *= value;
	m_y *= value;
	m_z *= value;

	return (*this);
}

Vector3D& Vector3D::operator*=(const Vector3D& value)
{
	m_x *= value.m_x;
	m_y *= value.m_y;
	m_z *= value.m_z;

	return (*this);
}

Vector3D& Vector3D::operator-=(const Vector3D& value)
{
	m_x -= value.m_x;
	m_y -= value.m_y;
	m_z -= value.m_z;

	return (*this);
}

Vector3D& Vector3D::operator/=(const float& value)
{
	m_x /= value;
	m_y /= value;
	m_z /= value;

	return (*this);
}

bool Vector3D::operator==(const Vector3D& value) const
{
	const float epsilon = Mathf::Epsilon8;

	return (abs(m_x - value.m_x) < epsilon && abs(m_y - value.m_y) < epsilon && abs(m_z - value.m_z) < epsilon);
}

bool Vector3D::operator!=(const Vector3D& value) const
{
	const float epsilon = Mathf::Epsilon8;
	return (abs(m_x - value.m_x) > epsilon || abs(m_y - value.m_y) > epsilon || abs(m_z - value.m_z) > epsilon);
}

bool Vector3D::IsNaN()
{
	return std::isnan(m_x) || std::isnan(m_y) || std::isnan(m_z);
}


Vector3D Vector3D::Normalized() const
{
	return (*this) / Magnitude();
}

Vector3D Vector3D::GetSafeInvertedVector()const
{
	Vector3D safeInvertedVector = Vector3D(0.0f,0.0f,0.0f);
	const float epsilon = Mathf::Epsilon8;

	if (abs(m_x) > epsilon)
		safeInvertedVector.m_x = 1.0f / m_x;

	if (abs(m_y) > epsilon)
		safeInvertedVector.m_y = 1.0f / m_y;

	if (abs(m_z) > epsilon)
		safeInvertedVector.m_z = 1.0f / m_z;

	return safeInvertedVector;
}

float Vector3D::DotProduct(const Vector3D& a, const Vector3D& b)
{
	return a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z;
}

Vector3D Vector3D::CrossProduct(const Vector3D& a, const Vector3D& b)
{
	return Vector3D(a.m_y * b.m_z - b.m_y * a.m_z, a.m_z * b.m_x - b.m_z * a.m_x, a.m_x * b.m_y - b.m_x * a.m_y);
}

Vector3D Vector3D::Lerp(const Vector3D& v, const Vector3D& v2, float t)
{
	return v + (v2 - v) * t;
}

Vector3D Vector3D::Slerp(const Vector3D& v, const Vector3D& v2, float t, bool longPath)
{
	float vMagnitudeSquared = v.MagnitudeSquared();
	float v2MagnitudeSquared = v2.MagnitudeSquared();
	float magnitudeSquared = vMagnitudeSquared + (v2MagnitudeSquared - vMagnitudeSquared) * t;

	Vector3D vNorm = v.Normalized();
	Quaternion q = Quaternion::SetGoToRotation(vNorm, v2.Normalized());
	Vector3D Axis = q.GetAxis();
	float angle = q.GetAngle();
	angle = longPath ? -360 + angle : angle;

	Vector3D slerpDirection = vNorm * sqrt(magnitudeSquared);
	Quaternion rotateQuat = Quaternion::AxisAngle(Axis, angle * t);
	
	return rotateQuat * slerpDirection;
}

std::ostream& operator<<(std::ostream& os, const Vector3D& value)
{
	os << "(X : " << value.m_x << ", Y : " << value.m_y << ", Z : " << value.m_z << ")" << std::endl;
	return os;
}
