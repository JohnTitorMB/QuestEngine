#ifndef _MATRIX2X2_H_
#define _MATRIX2X2_H_
#include "Vector2D.h"

struct Matrix2x2
{
public:
	float m[2][2];

	Matrix2x2();
	Matrix2x2(float m00, float m01,
			  float m10, float m11);

	Matrix2x2(const Vector2D& a, const Vector2D& b);
	float& operator ()(int i, int j);
	const float& operator ()(int i, int j)const;

	Matrix2x2& operator +=(const Matrix2x2& b);
	Matrix2x2& operator -=(const Matrix2x2& b);
	Matrix2x2& operator *=(const float& s);
	Matrix2x2& operator *=(const Matrix2x2& b);

	Matrix2x2 Transpose()const;
	Matrix2x2 Inverse()const;
	float Determinant()const;
};
Matrix2x2 operator +(const Matrix2x2& a, const Matrix2x2& b);
Matrix2x2 operator -(const Matrix2x2& a, const Matrix2x2& b);
Matrix2x2 operator *(const Matrix2x2& a, const float& s);
Matrix2x2 operator *(const float& s, const Matrix2x2& a);
Matrix2x2 operator *(const Matrix2x2& a, const Matrix2x2& b);

#endif
