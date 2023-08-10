#ifndef _VECTOR2D_H_
#define _VECTOR2D_H_
#include <ostream>

struct Vector2D
{
	float m_x;
	float m_y;

	Vector2D();
	Vector2D(float x, float y);
	Vector2D(const Vector2D& value);

	Vector2D operator+(const Vector2D& value) const;
	Vector2D operator*(const float& value) const;
	Vector2D operator-(const Vector2D& value) const;
	Vector2D operator/(const float& value) const;
	
	float Magnitude() const;
	void Normalize();
	Vector2D Normalized() const;
	static float DotProduct(const Vector2D& a, const Vector2D& b);

	Vector2D& operator+=(const Vector2D& value);
	Vector2D& operator*=(float value);
	Vector2D& operator-=(const Vector2D& value);
	Vector2D& operator/=(float value);
	bool operator==(const Vector2D& value)const;
	bool operator!=(const Vector2D& value)const;

private:
	float m_z = 0;
};
std::ostream& operator<<(std::ostream& os, const Vector2D& value);

#endif
