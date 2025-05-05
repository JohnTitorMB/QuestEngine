#ifndef _MATRIX3X3_H_
#define _MATRIX3X3_H_
#include <ostream>
#include <math.h>
#include "Vector2D.h"
#include <array>
struct Matrix3x3
{
public:
	float m[3][3];

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

	static Matrix3x3 Identity();
	static Matrix3x3 Translate(const Vector2D& translate);
	static Matrix3x3 RotateZ(const float& angle);
	static Matrix3x3 ScaleXY(const Vector2D& scale);

	static Matrix3x3 TS(const Vector2D& translate, const Vector2D& scale);
	static Matrix3x3 RS(const float& angle, const Vector2D& scale);
	static Matrix3x3 TR(const Vector2D& translate, const float& angle);
	static Matrix3x3 TRS(const Vector2D& translate, const float& angle, const Vector2D& scale);
	std::array<float, 9> GetAsArray() const;
};
Matrix3x3 operator +(const Matrix3x3& a, const Matrix3x3& b);
Matrix3x3 operator -(const Matrix3x3& a, const Matrix3x3& b);
Matrix3x3 operator *(const Matrix3x3& a, const float& s);
Matrix3x3 operator *(const float& s, const Matrix3x3& a);
Matrix3x3 operator *(const Matrix3x3& a, const Matrix3x3& b);
std::ostream& operator<<(std::ostream& os, const Matrix3x3& value);

Vector2D operator*(const Matrix3x3& a, const Vector2D& b);
Vector2D operator*(const Vector2D& b, const Matrix3x3& a);

Vector3D operator*(const Matrix3x3& a, const Vector3D& b);
Vector3D operator*(const Vector3D& b, const Matrix3x3& a);

#endif
