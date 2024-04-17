#ifndef _VECTOR4D_H_
#define _VECTOR4D_H_
#include <ostream>

struct Vector4D
{
	float m_x;
	float m_y;
	float m_z;
	float m_w;

	Vector4D();
	Vector4D(float x, float y, float z, float w);
	Vector4D(const Vector4D& value);

	Vector4D operator+(const Vector4D& value) const;
	Vector4D operator*(const float& value) const;
	Vector4D operator*(const Vector4D& value) const;
	Vector4D operator-(const Vector4D& value) const;
	Vector4D operator/(const float& value) const;

	float Magnitude() const;
	void Normalize();
	Vector4D Normalized() const;
	Vector4D GetSafeInvertedVector()const;

	Vector4D& operator+=(const Vector4D& value);
	Vector4D& operator*=(const float& value);
	Vector4D& operator*=(const Vector4D& value);
	Vector4D& operator-=(const Vector4D& value);
	Vector4D& operator/=(const float& value);
	bool operator==(const Vector4D& value)const;
	bool operator!=(const Vector4D& value)const;

	static float DotProduct(const Vector4D& a, const Vector4D& b);
};
std::ostream& operator<<(std::ostream& os, const Vector4D& value);

#endif