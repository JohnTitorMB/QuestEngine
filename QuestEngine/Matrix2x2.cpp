#include "Matrix2x2.h"

Matrix2x2::Matrix2x2()
{
	m[0][0] = 0.0f; m[0][1] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 0.0f;
}

Matrix2x2::Matrix2x2(float m00, float m01,
	float m10, float m11)
{
	m[0][0] = m00; m[0][1] = m01;
	m[1][0] = m10; m[1][1] = m11;
}

Matrix2x2::Matrix2x2(const Vector2D& a, const Vector2D& b)
{
	m[0][0] = a.m_x; m[0][1] = a.m_y;
	m[1][0] = b.m_x; m[1][1] = b.m_y;
}

float& Matrix2x2::operator ()(int i, int j)
{
	return m[i][j];
}

const float& Matrix2x2::operator ()(int i, int j)const
{
	return m[i][j];
}

Matrix2x2& Matrix2x2::operator +=(const Matrix2x2& b)
{
	m[0][0] += b.m[0][0];
	m[0][1] += b.m[0][1];
	m[1][0] += b.m[1][0];
	m[1][1] += b.m[1][1];

	return (*this);
}

Matrix2x2& Matrix2x2::operator -=(const Matrix2x2& b)
{
	m[0][0] -= b.m[0][0];
	m[0][1] -= b.m[0][1];
	m[1][0] -= b.m[1][0];
	m[1][1] -= b.m[1][1];

	return (*this);
}

Matrix2x2& Matrix2x2::operator *=(const float& s)
{
	m[0][0] *= s;
	m[0][1] *= s;
	m[1][0] *= s;
	m[1][1] *= s;

	return (*this);
}

Matrix2x2& Matrix2x2::operator *=(const Matrix2x2& b)
{
	m[0][0] = m[0][0] * b.m[0][0] + m[0][1] * b.m[1][0];
	m[0][1] = m[0][0] * b.m[0][1] + m[0][1] * b.m[1][1];
	m[1][0] = m[1][0] * b.m[0][0] + m[1][1] * b.m[1][0];
	m[1][1] = m[1][0] * b.m[0][1] + m[1][1] * b.m[1][1];
}

Matrix2x2 Matrix2x2::Transpose()const
{
	return Matrix2x2(m[0][0], m[1][0],
					 m[0][1], m[1][1]);
}

Matrix2x2 Matrix2x2::Inverse()const
{
	float determinant = Determinant();
	float epsilon = 1e-5; // Change this to the level of precision you need

	if (fabs(determinant) < epsilon)
	{
		throw std::invalid_argument("Matrix is singular (determinant is 0) and cannot be inverted.");
	}
	Matrix2x2 adjugateMatrix = Matrix2x2(m[1][1], -m[0][1],
										-m[1][0], m[0][0]);
	
	return (1.0f / determinant) * adjugateMatrix;
}

float Matrix2x2::Determinant()const
{
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

Matrix2x2 operator +(const Matrix2x2& a, const Matrix2x2& b)
{
	return Matrix2x2(a.m[0][0] + b.m[0][0], a.m[0][1] + b.m[0][1],
		a.m[1][0] + b.m[1][0], a.m[1][1] + b.m[1][1]);
}

Matrix2x2 operator -(const Matrix2x2& a, const Matrix2x2& b)
{
	return Matrix2x2(a.m[0][0] - b.m[0][0], a.m[0][1] - b.m[0][1],
		a.m[1][0] - b.m[1][0], a.m[1][1] - b.m[1][1]);
}

Matrix2x2 operator *(const Matrix2x2& a, const float& s)
{
	return Matrix2x2(a.m[0][0] * s, a.m[0][1] * s,
		a.m[1][0] * s, a.m[1][1] * s);
}

Matrix2x2 operator *(const float& s, const Matrix2x2& a)
{
	return Matrix2x2(s * a.m[0][0], s * a.m[0][1],
		s * a.m[1][0], s * a.m[1][1]);
}

Matrix2x2 operator *(const Matrix2x2& a, const Matrix2x2& b)
{
	return Matrix2x2(a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0],         // Element 00
					 a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1],	        // Element 01
					 a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0],			// Element 10
					 a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1]);		// Element 11
}