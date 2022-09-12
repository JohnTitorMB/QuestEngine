#ifndef _VECTOR2_H_
#define _VECTOR2_H_

struct Vector2D
{
	float m_x;
	float m_y;

	Vector2D(float x, float y);

	Vector2D operator+(const Vector2D& a) const;
	Vector2D operator*(const float& a) const;

};

#endif
