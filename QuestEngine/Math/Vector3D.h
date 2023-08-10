#ifndef _VECTOR3D_H_
#define _VECTOR3D_H_
#include <ostream>

struct Vector3D
{
	float m_x;
	float m_y;
	float m_z;

	Vector3D();
	Vector3D(float x, float y, float z);
	Vector3D(const Vector3D& value);

	Vector3D operator+(const Vector3D& value) const;
	Vector3D operator*(const float& value) const;
	Vector3D operator-(const Vector3D& value) const;
	Vector3D operator/(const float& value) const;

	float Magnitude() const;
	void Normalize();
	Vector3D Normalized() const;

	Vector3D& operator+=(const Vector3D& value);
	Vector3D& operator*=(float value);
	Vector3D& operator-=(const Vector3D& value);
	Vector3D& operator/=(float value);
	bool operator==(const Vector3D& value)const;
	bool operator!=(const Vector3D& value)const;

	static Vector3D CrossProduct(const Vector3D& a, const Vector3D& b);
	static float DotProduct(const Vector3D& a, const Vector3D& b);

};
std::ostream& operator<<(std::ostream& os, const Vector3D& value);

#endif