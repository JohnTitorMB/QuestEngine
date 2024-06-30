#ifndef _VECTOR2D_H_
#define _VECTOR2D_H_
#include <ostream>
#include "Vector3D.h"
struct Vector2D
{
	float m_x;
	float m_y;

	Vector2D();
	Vector2D(float x, float y);
	Vector2D(const Vector2D& value);
	Vector2D operator+(const Vector2D& value) const;
	Vector2D operator*(const float& value) const;
	Vector2D operator*(const Vector2D& value) const;
	Vector2D operator-(const Vector2D& value) const;
	Vector2D operator/(const float& value) const;
	
	float Magnitude() const;
	void Normalize();
	Vector2D Normalized() const;
	Vector2D GetSafeInvertedVector()const;

	static float DotProduct(const Vector2D& a, const Vector2D& b);
	static float GetAngle(const Vector2D& a, const Vector2D& b, const bool clockWise = false);
	Vector2D& operator+=(const Vector2D& value);
	Vector2D& operator*=(const float& value);
	Vector2D& operator*=(const Vector2D& value);
	Vector2D& operator-=(const Vector2D& value);
	Vector2D& operator/=(const float& value);
	bool operator==(const Vector2D& value)const;
	bool operator!=(const Vector2D& value)const;
};
std::ostream& operator<<(std::ostream& os, const Vector2D& value);

#endif
