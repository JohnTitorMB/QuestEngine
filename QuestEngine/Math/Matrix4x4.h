#ifndef _MATRIX4X4_H_
#define _MATRIX4X4_H_
#include <ostream>
#include <math.h>
#include "Vector3D.h"
#include "Vector4D.h"
#include <array>
struct Matrix4x4
{
public:
	float m[4][4];

	Matrix4x4();
	Matrix4x4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	Matrix4x4(const Vector3D& a, const Vector3D& b, const Vector3D& c);
	Matrix4x4(const Vector4D& a, const Vector4D& b, const Vector4D& c, const Vector4D& d);

	Matrix4x4(const Matrix4x4& value);

	float& operator ()(int i, int j);
	const float& operator ()(int i, int j)const;

	Matrix4x4& operator +=(const Matrix4x4& b);
	Matrix4x4& operator -=(const Matrix4x4& b);
	Matrix4x4& operator *=(const float& s);
	Matrix4x4& operator *=(const Matrix4x4& b);
	bool operator==(const Matrix4x4& value)const;
	bool operator!=(const Matrix4x4& value)const;

	Matrix4x4 Transpose()const;
	Matrix4x4 Inverse()const;
	float Determinant()const;

	static Matrix4x4 Identity();
	static Matrix4x4 Translate(const Vector3D& translate);
	static Matrix4x4 RotateX(const float& angle);
	static Matrix4x4 RotateY(const float& angle);
	static Matrix4x4 RotateZ(const float& angle);
	static Matrix4x4 Rotate(const Vector3D& angles);
	static Matrix4x4 ScaleXYZ(const Vector3D& scale);

	static Matrix4x4 TS(const Vector3D& translate, const Vector3D& scale);
	static Matrix4x4 RS(const Vector3D& angles, const Vector3D& scale);
	static Matrix4x4 TR(const Vector3D& translate, const Vector3D& angles);
	static Matrix4x4 TRS(const Vector3D& translate, const Vector3D& angles, const Vector3D& scale);
	std::array<float, 16> GetAsArray() const;
};
Matrix4x4 operator +(const Matrix4x4& a, const Matrix4x4& b);
Matrix4x4 operator -(const Matrix4x4& a, const Matrix4x4& b);
Matrix4x4 operator *(const Matrix4x4& a, const float& s);
Matrix4x4 operator *(const float& s, const Matrix4x4& a);
Matrix4x4 operator *(const Matrix4x4& a, const Matrix4x4& b);
std::ostream& operator<<(std::ostream& os, const Matrix4x4& value);

Vector3D operator*(const Matrix4x4& a, const Vector3D& b);
Vector3D operator*(const Vector3D& b, const Matrix4x4& a);
Vector4D operator*(const Vector4D& b, const Matrix4x4& a);
Vector4D operator*(const Matrix4x4& a, const Vector4D& b);

#endif