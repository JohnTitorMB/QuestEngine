#include "Vector2D.h"
#include <math.h>
#include "Mathf.h"

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

Vector2D::Vector2D(const Vector2D& value)
{
	m_x = value.m_x;
	m_y = value.m_y;
}

Vector2D Vector2D::operator+(const Vector2D& value) const
{
	return Vector2D(m_x + value.m_x, m_y + value.m_y);
}

Vector2D Vector2D::operator*(const float& value) const
{
	return Vector2D(m_x * value, m_y * value);
}

Vector2D Vector2D::operator*(const Vector2D& value) const
{
	return Vector2D(m_x * value.m_x, m_y * value.m_y);
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

Vector2D Vector2D::GetSafeInvertedVector()const
{
	Vector2D safeInvertedVector = Vector2D(0.0f, 0.0f);
	const float epsilon = Mathf::Epsilon8;

	if (abs(m_x) > epsilon)
		safeInvertedVector.m_x = 1.0f / m_x;

	if (abs(m_y) > epsilon)
		safeInvertedVector.m_y = 1.0f / m_y;

	return safeInvertedVector;
}

Vector2D& Vector2D::operator+=(const Vector2D& value)
{
	m_x += value.m_x;
	m_y += value.m_y;

	return (*this);
}

Vector2D& Vector2D::operator*=(const float& value)
{
	m_x *= value;
	m_y *= value;

	return (*this);
}

Vector2D& Vector2D::operator*=(const Vector2D& value)
{
	m_x *= value.m_x;
	m_y *= value.m_y;

	return (*this);
}

Vector2D& Vector2D::operator-=(const Vector2D& value)
{
	m_x -= value.m_x;
	m_y -= value.m_y;

	return (*this);
}

Vector2D& Vector2D::operator/=(const float& value)
{
	m_x /= value;
	m_y /= value;

	return (*this);
}

bool Vector2D::operator==(const Vector2D& value) const
{
	return (abs(m_x - value.m_x) < FLT_EPSILON && abs(m_y - value.m_y) < FLT_EPSILON);
}

bool Vector2D::operator!=(const Vector2D& value) const
{
	return (abs(m_x - value.m_x) > FLT_EPSILON || abs(m_y - value.m_y) > FLT_EPSILON);
}

Vector2D Vector2D::Normalized() const
{
	return (*this) / Magnitude();
}

float Vector2D::DotProduct(const Vector2D& a, const Vector2D& b)
{
	return a.m_x * b.m_x + a.m_y * b.m_y;
}

float Vector2D::GetAngle(const Vector2D& a, const Vector2D& b, const bool clockWise)
{
	// Calculate the dot product
	float dotProduct = Vector2D::DotProduct(a,b);

	// Calculate the magnitudes of the vectors
	float mag1 = a.Magnitude();
	float mag2 = b.Magnitude();

	// Calculate the cosine of the angle between the vectors
	float cosAngle = dotProduct / (mag1 * mag2);

	// Clamp the cosine value to [-1, 1] to avoid numerical errors
	cosAngle = std::max(-1.0f, std::min(1.0f, cosAngle));

	// Calculate the angle in radians
	float angle = std::acos(cosAngle);

	// Convert angle to degrees
	angle = angle * 180.0f / Mathf::PI;

	// Determine the sign of the angle based on the specified clockwise parameter
	if (clockWise) {
		angle = 360.0f - angle;
	}

	return angle;
}

std::ostream& operator<<(std::ostream& os, const Vector2D& value)
{
	os << "(X : " << value.m_x << ", Y : " << value.m_y << ")" << std::endl;
	return os;
}
