#include "Matrix3x3.h"

Matrix3x3::Matrix3x3()
{
	m[0][0] = 0.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 0.0f; m[1][2] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 0.0f;
}

Matrix3x3::Matrix3x3(float m00, float m01, float m02,
	float m10, float m11, float m12,
	float m20, float m21, float m22)
{
	m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
	m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
	m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
}

float& Matrix3x3::operator ()(int i, int j)
{
	return m[i][j];
}

const float& Matrix3x3::operator ()(int i, int j)const
{
	return m[i][j];
}

Matrix3x3 Matrix3x3::operator +=(const Matrix3x3& b)
{
	m[0][0] += b.m[0][0];
	m[0][1] += b.m[0][1];
	m[0][2] += b.m[0][2];

	m[1][0] += b.m[1][0];
	m[1][1] += b.m[1][1];
	m[1][2] += b.m[1][2];

	m[2][0] += b.m[2][0];
	m[2][1] += b.m[2][1];
	m[2][2] += b.m[2][2];

	return (*this);
}

Matrix3x3 Matrix3x3::operator -=(const Matrix3x3& b)
{
	m[0][0] -= b.m[0][0];
	m[0][1] -= b.m[0][1];
	m[0][2] -= b.m[0][2];

	m[1][0] -= b.m[1][0];
	m[1][1] -= b.m[1][1];
	m[1][2] -= b.m[1][2];

	m[2][0] -= b.m[2][0];
	m[2][1] -= b.m[2][1];
	m[2][2] -= b.m[2][2];

	return (*this);
}

Matrix3x3 Matrix3x3::operator *=(const float& s)
{
	m[0][0] *= s;
	m[0][1] *= s;
	m[0][2] *= s;

	m[1][0] *= s;
	m[1][1] *= s;
	m[1][2] *= s;

	m[2][0] *= s;
	m[2][1] *= s;
	m[2][2] *= s;

	return (*this);
}

Matrix3x3 Matrix3x3::operator *=(const Matrix3x3& b)
{
	m[0][0] = m[0][0] * b.m[0][0] + m[0][1] * b.m[1][0] + m[0][2] * b.m[2][0];
	m[0][1] = m[0][0] * b.m[0][1] + m[0][1] * b.m[1][1] + m[0][2] * b.m[2][1];
	m[0][2] = m[0][0] * b.m[0][2] + m[0][1] * b.m[1][2] + m[0][2] * b.m[2][2];

	m[1][0] = m[1][0] * b.m[0][0] + m[1][1] * b.m[1][0] + m[1][2] * b.m[2][0];
	m[1][1] = m[1][0] * b.m[0][1] + m[1][1] * b.m[1][1] + m[1][2] * b.m[2][1];
	m[1][2] = m[1][0] * b.m[0][2] + m[1][1] * b.m[1][2] + m[1][2] * b.m[2][2];

	m[2][0] = m[2][0] * b.m[0][0] + m[2][1] * b.m[1][0] + m[2][2] * b.m[2][0];
	m[2][1] = m[2][0] * b.m[0][1] + m[2][1] * b.m[1][1] + m[2][2] * b.m[2][1];
	m[2][2] = m[2][0] * b.m[0][2] + m[2][1] * b.m[1][2] + m[2][2] * b.m[2][2];

	return (*this);
}

Matrix3x3 Matrix3x3::Transpose() const
{
	return Matrix3x3(m[0][0], m[1][0], m[2][0],
					 m[0][1], m[1][1], m[2][1],
					 m[0][2], m[1][2], m[2][2]);
}

Matrix3x3 Matrix3x3::Inverse()const
{
	float determinant = Determinant();
	float epsilon = 1e-5; // Change this to the level of precision you need

	if (fabs(determinant) < epsilon)
	{
		throw std::invalid_argument("Matrix is singular (determinant is 0) and cannot be inverted.");
	}
	
	float C00 = powf(-1,0 + 0)* (m[1][1] * m[2][2] - m[1][2] * m[2][1]);
	float C01 = powf(-1, 0 + 1) * (m[1][0] * m[2][2] - m[1][2] * m[2][0]);
	float C02 = powf(-1, 0 + 2) * (m[1][0] * m[2][1] - m[2][0] * m[1][1]);

	float C10 = powf(-1, 1 + 0) * (m[0][1] * m[2][2] - m[0][2] * m[2][1]);
	float C11 = powf(-1, 1 + 1) * (m[0][0] * m[2][2] - m[0][2] * m[2][0]);
	float C12 = powf(-1, 1 + 2) * (m[0][0] * m[2][1] - m[0][1] * m[2][0]);

	float C20 = powf(-1, 2 + 0) * (m[0][1] * m[1][2] - m[0][2] * m[1][1]);
	float C21 = powf(-1, 2 + 1) * (m[0][0] * m[1][2] - m[0][2] * m[1][0]);
	float C22 = powf(-1, 2 + 2) * (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
	
	const Matrix3x3 CofactorMatrix = Matrix3x3(C00, C01, C02,
										 C10, C11, C12,
										 C20, C21, C22);

	Matrix3x3 adjugateMatrix = CofactorMatrix.Transpose();

	return (1.0f / determinant) * adjugateMatrix;
}

float Matrix3x3::Determinant()const
{
	return m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] + m[0][2] * m[1][0] * m[2][1]
		- m[0][2] * m[1][1] * m[2][0] + m[0][1] * m[1][2] * m[2][0] - m[0][1] * m[1][0] * m[2][2];
}

Matrix3x3 operator +(const Matrix3x3& a, const Matrix3x3& b)
{
	return Matrix3x3(a.m[0][0] + b.m[0][0], a.m[0][1] + b.m[0][1], a.m[0][2] + b.m[0][2],
					 a.m[1][0] + b.m[1][0], a.m[1][1] + b.m[1][1], a.m[1][2] + b.m[1][2],
					 a.m[2][0] + b.m[2][0], a.m[2][1] + b.m[2][1], a.m[2][2] + b.m[2][2]);
}

Matrix3x3 operator -(const Matrix3x3& a, const Matrix3x3& b)
{
	return Matrix3x3(a.m[0][0] - b.m[0][0], a.m[0][1] - b.m[0][1], a.m[0][2] - b.m[0][2],
		a.m[1][0] - b.m[1][0], a.m[1][1] - b.m[1][1], a.m[1][2] - b.m[1][2],
		a.m[2][0] - b.m[2][0], a.m[2][1] - b.m[2][1], a.m[2][2] - b.m[2][2]);
}

Matrix3x3 operator *(const Matrix3x3& a, const float& s)
{
	return Matrix3x3(a.m[0][0] * s, a.m[0][1] * s, a.m[0][2] * s,
					 a.m[1][0] * s, a.m[1][1] * s, a.m[1][2] * s,
					 a.m[2][0] * s, a.m[2][1] * s, a.m[2][2] * s);
}

Matrix3x3 operator *(const float& s, const Matrix3x3& a)
{
	return Matrix3x3(s * a.m[0][0], s * a.m[0][1], s * a.m[0][2],
					 s * a.m[1][0], s * a.m[1][1], s * a.m[1][2],
					 s * a.m[2][0], s * a.m[2][1], s * a.m[2][2]);
}

Matrix3x3 operator *(const Matrix3x3& a, const Matrix3x3& b)
{
	return Matrix3x3(a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0] + a.m[0][2] * b.m[2][0],
					 a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1] + a.m[0][2] * b.m[2][1],
					 a.m[0][0] * b.m[0][2] + a.m[0][1] * b.m[1][2] + a.m[0][2] * b.m[2][2],

					 a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0] + a.m[1][2] * b.m[2][0],
					 a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1] + a.m[1][2] * b.m[2][1],
					 a.m[1][0] * b.m[0][2] + a.m[1][1] * b.m[1][2] + a.m[1][2] * b.m[2][2],

					 a.m[2][0] * b.m[0][0] + a.m[2][1] * b.m[1][0] + a.m[2][2] * b.m[2][0],
					 a.m[2][0] * b.m[0][1] + a.m[2][1] * b.m[1][1] + a.m[2][2] * b.m[2][1],
					 a.m[2][0] * b.m[0][2] + a.m[2][1] * b.m[1][2] + a.m[2][2] * b.m[2][2]);
}