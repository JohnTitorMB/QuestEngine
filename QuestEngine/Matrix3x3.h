#ifndef _MATRIX3X3_H_
#define _MATRIX3X3_H_
#include <ostream>
#include <math.h>

struct Matrix3x3
{
public:
	Matrix3x3();
	Matrix3x3(float m00, float m01, float m02,
			  float m10, float m11, float m12,
			  float m20, float m21, float m22);
	
	Matrix3x3(const Matrix3x3& value);

	float& operator ()(int i, int j);
	const float& operator ()(int i, int j)const;

	Matrix3x3& operator +=(const Matrix3x3& b);
	Matrix3x3& operator -=(const Matrix3x3& b);
	Matrix3x3& operator *=(const float& s);
	Matrix3x3& operator *=(const Matrix3x3& b);
	bool operator==(const Matrix3x3& value)const;
	bool operator!=(const Matrix3x3& value)const;

	Matrix3x3 Transpose()const;
	Matrix3x3 Inverse()const;
	float Determinant()const;

	float m[3][3];
};
Matrix3x3 operator +(const Matrix3x3& a, const Matrix3x3& b);
Matrix3x3 operator -(const Matrix3x3& a, const Matrix3x3& b);
Matrix3x3 operator *(const Matrix3x3& a, const float& s);
Matrix3x3 operator *(const float& s, const Matrix3x3& a);
Matrix3x3 operator *(const Matrix3x3& a, const Matrix3x3& b);
std::ostream& operator<<(std::ostream& os, const Matrix3x3& value);

#endif
