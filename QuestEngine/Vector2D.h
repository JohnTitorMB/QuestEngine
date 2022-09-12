#ifndef _VECTOR2D_H_
#define _VECTOR2D_H_

struct Vector2D
{
	float m_x;
	float m_y;

	Vector2D(float x, float y);

	Vector2D operator+(const Vector2D& value) const;
	Vector2D operator*(const float& value) const;
	Vector2D operator-(const Vector2D& value) const;
	Vector2D operator/(const float& value) const;
	float Magnitude();
	void Normalize();
};

#endif
