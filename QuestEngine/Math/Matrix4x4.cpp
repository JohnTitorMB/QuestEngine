#include "Matrix4x4.h"
#include "Matrix3x3.h"
# define M_PI           3.14159265358979323846  /* pi */

Matrix4x4::Matrix4x4()
{
	m[0][0] = 0.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; ; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 0.0f; m[1][2] = 0.0f; ; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 0.0f; ; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; ; m[3][3] = 0.0f;
}

Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
	m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
	m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
	m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
}

Matrix4x4::Matrix4x4(const Vector3D& a, const Vector3D& b, const Vector3D& c)
{
	m[0][0] = a.m_x; m[0][1] = b.m_x; m[0][2] = c.m_x; m[0][3] = 0.0f;
	m[1][0] = a.m_y; m[1][1] = b.m_y; m[1][2] = c.m_y; m[1][3] = 0.0f;
	m[2][0] = a.m_z; m[2][1] = b.m_z; m[2][2] = c.m_z; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

Matrix4x4::Matrix4x4(const Vector4D& a, const Vector4D& b, const Vector4D& c, const Vector4D& d)
{
	m[0][0] = a.m_x; m[0][1] = b.m_x; m[0][2] = c.m_x; m[0][3] = d.m_x;
	m[1][0] = a.m_y; m[1][1] = b.m_y; m[1][2] = c.m_y; m[1][3] = d.m_y;
	m[2][0] = a.m_z; m[2][1] = b.m_z; m[2][2] = c.m_z; m[2][3] = d.m_z;
	m[3][0] = a.m_w; m[3][1] = b.m_w; m[3][2] = c.m_w; m[3][3] = d.m_w;
}

Matrix4x4::Matrix4x4(const Matrix4x4& value)
{
	m[0][0] = value.m[0][0];
	m[0][1] = value.m[0][1];
	m[0][2] = value.m[0][2];
	m[0][3] = value.m[0][3];

	m[1][0] = value.m[1][0];
	m[1][1] = value.m[1][1];
	m[1][2] = value.m[1][2];
	m[1][3] = value.m[1][3];

	m[2][0] = value.m[2][0];
	m[2][1] = value.m[2][1];
	m[2][2] = value.m[2][2];
	m[2][3] = value.m[2][3];

	m[3][0] = value.m[3][0];
	m[3][1] = value.m[3][1];
	m[3][2] = value.m[3][2];
	m[3][3] = value.m[3][3];
}


float& Matrix4x4::operator ()(int i, int j)
{
	return m[i][j];
}

const float& Matrix4x4::operator ()(int i, int j)const
{
	return m[i][j];
}

Matrix4x4& Matrix4x4::operator +=(const Matrix4x4& b)
{
	m[0][0] += b.m[0][0];
	m[0][1] += b.m[0][1];
	m[0][2] += b.m[0][2];
	m[0][3] += b.m[0][3];

	m[1][0] += b.m[1][0];
	m[1][1] += b.m[1][1];
	m[1][2] += b.m[1][2];
	m[1][3] += b.m[1][3];

	m[2][0] += b.m[2][0];
	m[2][1] += b.m[2][1];
	m[2][2] += b.m[2][2];
	m[2][3] += b.m[2][3];

	m[3][0] += b.m[3][0];
	m[3][1] += b.m[3][1];
	m[3][2] += b.m[3][2];
	m[3][3] += b.m[3][3];

	return (*this);
}

Matrix4x4& Matrix4x4::operator -=(const Matrix4x4& b)
{
	m[0][0] -= b.m[0][0];
	m[0][1] -= b.m[0][1];
	m[0][2] -= b.m[0][2];
	m[0][3] -= b.m[0][3];

	m[1][0] -= b.m[1][0];
	m[1][1] -= b.m[1][1];
	m[1][2] -= b.m[1][2];
	m[1][3] -= b.m[1][3];

	m[2][0] -= b.m[2][0];
	m[2][1] -= b.m[2][1];
	m[2][2] -= b.m[2][2];
	m[2][3] -= b.m[2][3];

	m[3][0] -= b.m[3][0];
	m[3][1] -= b.m[3][1];
	m[3][2] -= b.m[3][2];
	m[3][3] -= b.m[3][3];

	return (*this);
}

Matrix4x4& Matrix4x4::operator *=(const float& s)
{
	m[0][0] *= s;
	m[0][1] *= s;
	m[0][2] *= s;
	m[0][3] *= s;

	m[1][0] *= s;
	m[1][1] *= s;
	m[1][2] *= s;
	m[1][3] *= s;

	m[2][0] *= s;
	m[2][1] *= s;
	m[2][2] *= s;
	m[2][3] *= s;

	m[3][0] *= s;
	m[3][1] *= s;
	m[3][2] *= s;
	m[3][3] *= s;

	return (*this);
}

Matrix4x4& Matrix4x4::operator *=(const Matrix4x4& b)
{
	Matrix4x4 result;

	result.m[0][0] = m[0][0] * b.m[0][0] + m[0][1] * b.m[1][0] + m[0][2] * b.m[2][0] + m[0][3] * b.m[3][0];
	result.m[0][1] = m[0][0] * b.m[0][1] + m[0][1] * b.m[1][1] + m[0][2] * b.m[2][1] + m[0][3] * b.m[3][1];
	result.m[0][2] = m[0][0] * b.m[0][2] + m[0][1] * b.m[1][2] + m[0][2] * b.m[2][2] + m[0][3] * b.m[3][2];
	result.m[0][3] = m[0][0] * b.m[0][3] + m[0][1] * b.m[1][3] + m[0][2] * b.m[2][3] + m[0][3] * b.m[3][3];

	result.m[1][0] = m[1][0] * b.m[0][0] + m[1][1] * b.m[1][0] + m[1][2] * b.m[2][0] + m[1][3] * b.m[3][0];
	result.m[1][1] = m[1][0] * b.m[0][1] + m[1][1] * b.m[1][1] + m[1][2] * b.m[2][1] + m[1][3] * b.m[3][1];
	result.m[1][2] = m[1][0] * b.m[0][2] + m[1][1] * b.m[1][2] + m[1][2] * b.m[2][2] + m[1][3] * b.m[3][2];
	result.m[1][3] = m[1][0] * b.m[0][3] + m[1][1] * b.m[1][3] + m[1][2] * b.m[2][3] + m[1][3] * b.m[3][3];

	result.m[2][0] = m[2][0] * b.m[0][0] + m[2][1] * b.m[1][0] + m[2][2] * b.m[2][0] + m[2][3] * b.m[3][0];
	result.m[2][1] = m[2][0] * b.m[0][1] + m[2][1] * b.m[1][1] + m[2][2] * b.m[2][1] + m[2][3] * b.m[3][1];
	result.m[2][2] = m[2][0] * b.m[0][2] + m[2][1] * b.m[1][2] + m[2][2] * b.m[2][2] + m[2][3] * b.m[3][2];
	result.m[2][3] = m[2][0] * b.m[0][3] + m[2][1] * b.m[1][3] + m[2][2] * b.m[2][3] + m[2][3] * b.m[3][3];

	result.m[3][0] = m[3][0] * b.m[0][0] + m[3][1] * b.m[1][0] + m[3][2] * b.m[2][0] + m[3][3] * b.m[3][0];
	result.m[3][1] = m[3][0] * b.m[0][1] + m[3][1] * b.m[1][1] + m[3][2] * b.m[2][1] + m[3][3] * b.m[3][1];
	result.m[3][2] = m[3][0] * b.m[0][2] + m[3][1] * b.m[1][2] + m[3][2] * b.m[2][2] + m[3][3] * b.m[3][2];
	result.m[3][3] = m[3][0] * b.m[0][3] + m[3][1] * b.m[1][3] + m[3][2] * b.m[2][3] + m[3][3] * b.m[3][3];

	*this = result;
	return (*this);
}

Matrix4x4 Matrix4x4::Transpose() const
{
	return Matrix4x4(m[0][0], m[1][0], m[2][0], m[3][0],
					 m[0][1], m[1][1], m[2][1], m[3][1],
					 m[0][2], m[1][2], m[2][2], m[3][2],
					 m[0][3], m[1][3], m[2][3], m[3][3]);
}

Matrix4x4 Matrix4x4::Inverse()const
{
	float determinant = Determinant();
	float epsilon = 1e-9; // Change this to the level of precision you need

	if (fabs(determinant) < epsilon)
	{
		throw std::invalid_argument("Matrix is singular (determinant is 0) and cannot be inverted.");
	}
	Matrix3x3 subMatrix00 = Matrix3x3(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
	Matrix3x3 subMatrix01 = Matrix3x3(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
	Matrix3x3 subMatrix02 = Matrix3x3(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]);
	Matrix3x3 subMatrix03 = Matrix3x3(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);

	Matrix3x3 subMatrix10 = Matrix3x3(m[0][1], m[0][2], m[0][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
	Matrix3x3 subMatrix11 = Matrix3x3(m[0][0], m[0][2], m[0][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
	Matrix3x3 subMatrix12 = Matrix3x3(m[0][0], m[0][1], m[0][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]);
	Matrix3x3 subMatrix13 = Matrix3x3(m[0][0], m[0][1], m[0][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);

	Matrix3x3 subMatrix20 = Matrix3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[3][1], m[3][2], m[3][3]);
	Matrix3x3 subMatrix21 = Matrix3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[3][0], m[3][2], m[3][3]);
	Matrix3x3 subMatrix22 = Matrix3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[3][0], m[3][1], m[3][3]);
	Matrix3x3 subMatrix23 = Matrix3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[3][0], m[3][1], m[3][2]);

	Matrix3x3 subMatrix30 = Matrix3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3]);
	Matrix3x3 subMatrix31 = Matrix3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3]);
	Matrix3x3 subMatrix32 = Matrix3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3]);
	Matrix3x3 subMatrix33 = Matrix3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]);

	float C00 = powf(-1, 0 + 0) * subMatrix00.Determinant();
	float C01 = powf(-1, 0 + 1) * subMatrix01.Determinant();
	float C02 = powf(-1, 0 + 2) * subMatrix02.Determinant();
	float C03 = powf(-1, 0 + 3) * subMatrix03.Determinant();

	float C10 = powf(-1, 1 + 0) * subMatrix10.Determinant();
	float C11 = powf(-1, 1 + 1) * subMatrix11.Determinant();
	float C12 = powf(-1, 1 + 2) * subMatrix12.Determinant();
	float C13 = powf(-1, 1 + 3) * subMatrix13.Determinant();

	float C20 = powf(-1, 2 + 0) * subMatrix20.Determinant();
	float C21 = powf(-1, 2 + 1) * subMatrix21.Determinant();
	float C22 = powf(-1, 2 + 2) * subMatrix22.Determinant();
	float C23 = powf(-1, 2 + 3) * subMatrix23.Determinant();

	float C30 = powf(-1, 3 + 0) * subMatrix30.Determinant();
	float C31 = powf(-1, 3 + 1) * subMatrix31.Determinant();
	float C32 = powf(-1, 3 + 2) * subMatrix32.Determinant();
	float C33 = powf(-1, 3 + 3) * subMatrix33.Determinant();

	const Matrix4x4 CofactorMatrix = Matrix4x4(C00, C01, C02, C03,
		C10, C11, C12, C13,
		C20, C21, C22, C23,
		C30, C31, C32, C33);

	Matrix4x4 adjugateMatrix = CofactorMatrix.Transpose();

	return (1.0f / determinant) * adjugateMatrix;
}

float Matrix4x4::Determinant()const
{
	Matrix3x3 subMatrix00 = Matrix3x3(m[1][1], m[1][2], m[1][3],
									  m[2][1], m[2][2], m[2][3],
									  m[3][1], m[3][2], m[3][3]);

	Matrix3x3 subMatrix01 = Matrix3x3(m[1][0], m[1][2], m[1][3],
									  m[2][0], m[2][2], m[2][3],
									  m[3][0], m[3][2], m[3][3]);

	Matrix3x3 subMatrix02 = Matrix3x3(m[1][0], m[1][1], m[1][3],
									  m[2][0], m[2][1], m[2][3],
									  m[3][0], m[3][1], m[3][3]);

	Matrix3x3 subMatrix03 = Matrix3x3(m[1][0], m[1][1], m[1][2],
									  m[2][0], m[2][1], m[2][2],
									  m[3][0], m[3][1], m[3][2]);

	float determinant = m[0][0] * subMatrix00.Determinant() - m[0][1] * subMatrix01.Determinant()
		+ m[0][2] * subMatrix02.Determinant() - m[0][3] * subMatrix03.Determinant();

	return determinant;
}

Matrix4x4 operator +(const Matrix4x4& a, const Matrix4x4& b)
{
	return Matrix4x4(a.m[0][0] + b.m[0][0], a.m[0][1] + b.m[0][1], a.m[0][2] + b.m[0][2], a.m[0][3] + b.m[0][3],
					 a.m[1][0] + b.m[1][0], a.m[1][1] + b.m[1][1], a.m[1][2] + b.m[1][2], a.m[1][3] + b.m[1][3],
					 a.m[2][0] + b.m[2][0], a.m[2][1] + b.m[2][1], a.m[2][2] + b.m[2][2], a.m[2][3] + b.m[2][3],
					 a.m[3][0] + b.m[3][0], a.m[3][1] + b.m[3][1], a.m[3][2] + b.m[3][2], a.m[3][3] + b.m[3][3]);
}

Matrix4x4 operator -(const Matrix4x4& a, const Matrix4x4& b)
{
	return Matrix4x4(a.m[0][0] - b.m[0][0], a.m[0][1] - b.m[0][1], a.m[0][2] - b.m[0][2], a.m[0][3] - b.m[0][3],
		a.m[1][0] - b.m[1][0], a.m[1][1] - b.m[1][1], a.m[1][2] - b.m[1][2], a.m[1][3] - b.m[1][3],
		a.m[2][0] - b.m[2][0], a.m[2][1] - b.m[2][1], a.m[2][2] - b.m[2][2], a.m[2][3] - b.m[2][3],
		a.m[3][0] - b.m[3][0], a.m[3][1] - b.m[3][1], a.m[3][2] - b.m[3][2], a.m[3][3] - b.m[3][3]);
}

Matrix4x4 operator *(const Matrix4x4& a, const float& s)
{
	return Matrix4x4(a.m[0][0] * s, a.m[0][1] * s, a.m[0][2] * s, a.m[0][3] * s,
					 a.m[1][0] * s, a.m[1][1] * s, a.m[1][2] * s, a.m[1][3] * s,
					 a.m[2][0] * s, a.m[2][1] * s, a.m[2][2] * s, a.m[2][3] * s,
					 a.m[3][0] * s, a.m[3][1] * s, a.m[3][2] * s, a.m[3][3] * s);
}

Matrix4x4 operator *(const float& s, const Matrix4x4& a)
{
	return Matrix4x4(s * a.m[0][0], s * a.m[0][1], s * a.m[0][2], s * a.m[0][3],
					 s * a.m[1][0], s * a.m[1][1], s * a.m[1][2], s * a.m[1][3],
					 s * a.m[2][0], s * a.m[2][1], s * a.m[2][2], s * a.m[2][3],
					 s * a.m[3][0], s * a.m[3][1], s * a.m[3][2], s * a.m[3][3]);
}

Matrix4x4 operator *(const Matrix4x4& a, const Matrix4x4& b)
{
	return Matrix4x4(a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0] + a.m[0][2] * b.m[2][0] + a.m[0][3] * b.m[3][0],
					 a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1] + a.m[0][2] * b.m[2][1] + a.m[0][3] * b.m[3][1],
					 a.m[0][0] * b.m[0][2] + a.m[0][1] * b.m[1][2] + a.m[0][2] * b.m[2][2] + a.m[0][3] * b.m[3][2],
					 a.m[0][0] * b.m[0][3] + a.m[0][1] * b.m[1][3] + a.m[0][2] * b.m[2][3] + a.m[0][3] * b.m[3][3],

					 a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0] + a.m[1][2] * b.m[2][0] + a.m[1][3] * b.m[3][0],
					 a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1] + a.m[1][2] * b.m[2][1] + a.m[1][3] * b.m[3][1],
					 a.m[1][0] * b.m[0][2] + a.m[1][1] * b.m[1][2] + a.m[1][2] * b.m[2][2] + a.m[1][3] * b.m[3][2],
					 a.m[1][0] * b.m[0][3] + a.m[1][1] * b.m[1][3] + a.m[1][2] * b.m[2][3] + a.m[1][3] * b.m[3][3],

					 a.m[2][0] * b.m[0][0] + a.m[2][1] * b.m[1][0] + a.m[2][2] * b.m[2][0] + a.m[2][3] * b.m[3][0],
					 a.m[2][0] * b.m[0][1] + a.m[2][1] * b.m[1][1] + a.m[2][2] * b.m[2][1] + a.m[2][3] * b.m[3][1],
					 a.m[2][0] * b.m[0][2] + a.m[2][1] * b.m[1][2] + a.m[2][2] * b.m[2][2] + a.m[2][3] * b.m[3][2],
					 a.m[2][0] * b.m[0][3] + a.m[2][1] * b.m[1][3] + a.m[2][2] * b.m[2][3] + a.m[2][3] * b.m[3][3],

					 a.m[3][0] * b.m[0][0] + a.m[3][1] * b.m[1][0] + a.m[3][2] * b.m[2][0] + a.m[3][3] * b.m[3][0],
					 a.m[3][0] * b.m[0][1] + a.m[3][1] * b.m[1][1] + a.m[3][2] * b.m[2][1] + a.m[3][3] * b.m[3][1],
					 a.m[3][0] * b.m[0][2] + a.m[3][1] * b.m[1][2] + a.m[3][2] * b.m[2][2] + a.m[3][3] * b.m[3][2],
					 a.m[3][0] * b.m[0][3] + a.m[3][1] * b.m[1][3] + a.m[3][2] * b.m[2][3] + a.m[3][3] * b.m[3][3]);
}

bool Matrix4x4::operator==(const Matrix4x4& value) const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (abs(m[i][j] - value.m[i][j]) > FLT_EPSILON)
				return false;
		}
	}
	return true;
}

bool Matrix4x4::operator!=(const Matrix4x4& value) const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (abs(m[i][j] - value.m[i][j]) > FLT_EPSILON)
				return true;
		}
	}
	return false;
}

std::ostream& operator<<(std::ostream& os, const Matrix4x4& value)
{
	os << "(m00 : " << value.m[0][0] << ", m01 : " << value.m[0][1] << ", m02 : " << value.m[0][2] << ", m03 : " << value.m[0][3] << ")" << std::endl
		<< "(m10 : " << value.m[1][0] << ", m11 : " << value.m[1][1] << ", m12 : " << value.m[1][2] << ", m13 : " << value.m[1][3] << ")" << std::endl
		<< "(m20 : " << value.m[2][0] << ", m21 : " << value.m[2][1] << ", m22 : " << value.m[2][2] << ", m23 : " << value.m[2][3] << ")" << std::endl
		<< "(m30 : " << value.m[3][0] << ", m31 : " << value.m[3][1] << ", m32 : " << value.m[3][2] << ", m33 : " << value.m[3][3] << ")" << std::endl;
	return os;
}

Matrix4x4 Matrix4x4::Identity()
{
	return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::Translate(const Vector3D& translate)
{
	return Matrix4x4(1.0f, 0.0f,0.0f, translate.m_x,
		0.0f, 1.0f, 0.0f, translate.m_y,
		0.0f, 0.0f, 1.0f, translate.m_z,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::RotateX(const float& angle)
{
	float angleInRadian = angle * M_PI / 180.0f;
	return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosf(angleInRadian), -sinf(angleInRadian), 0.0f,
		0.0f, sinf(angleInRadian), cosf(angleInRadian), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::RotateY(const float& angle)
{
	float angleInRadian = angle * M_PI / 180.0f;
	return Matrix4x4(cosf(angleInRadian), 0.0f, sinf(angleInRadian), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sinf(angleInRadian), 0.0f, cosf(angleInRadian), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}
Matrix4x4 Matrix4x4::RotateZ(const float& angle)
{
	float angleInRadian = angle * M_PI / 180.0f;
	return Matrix4x4(cosf(angleInRadian), -sinf(angleInRadian), 0.0f,0.0f,
					 sinf(angleInRadian), cosf(angleInRadian), 0.0f,0.0f,
					 0.0f, 0.0f,1.0f, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::Rotate(const Vector3D& angles)
{
	return RotateZ(angles.m_z) * RotateY(angles.m_y) * RotateX(angles.m_x);
}

Matrix4x4 Matrix4x4::Rotate(const Quaternion& q)
{
	float m00 = 1.0f - 2.0f * q.m_y * q.m_y - 2.0f * q.m_z * q.m_z;
	float m01 = 2.0f * (q.m_x * q.m_y - q.m_w * q.m_z);
	float m02 = 2.0f * (q.m_x * q.m_z + q.m_w * q.m_y);

	float m10 = 2.0f * (q.m_x * q.m_y + q.m_w * q.m_z);
	float m11 = 1.0f - 2.0f * q.m_x * q.m_x - 2.0f * q.m_z * q.m_z;
	float m12 = 2.0f * (q.m_y * q.m_z - q.m_w * q.m_x);

	float m20 = 2.0f * (q.m_x * q.m_z - q.m_w * q.m_y);
	float m21 = 2.0f * (q.m_y * q.m_z + q.m_w * q.m_x);
	float m22 = 1.0f - 2.0f * q.m_x * q.m_x - 2.0f * q.m_y * q.m_y;

	return Matrix4x4(m00, m01, m02, 0,
		m10, m11, m12, 0,
		m20, m21, m22, 0,
		0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::ScaleXYZ(const Vector3D& scale)
{
	return Matrix4x4(scale.m_x, 0.0f, 0.0f,0.0f,
		0.0f, scale.m_y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.m_z,0.0f,
		0.0f, 0.0f, 0.0f,1.0f);
}

Matrix4x4 Matrix4x4::TS(const Vector3D& translate, const Vector3D& scale)
{
	return Translate(translate) * ScaleXYZ(scale);
}

Matrix4x4 Matrix4x4::RS(const Quaternion& rotation, const Vector3D& scale)
{
	return Rotate(rotation) * ScaleXYZ(scale);
}

Matrix4x4 Matrix4x4::TR(const Vector3D& translate, const Quaternion& rotation)
{
	return Translate(translate) * Rotate(rotation);
}

Matrix4x4 Matrix4x4::TRS(const Vector3D& translate, const Quaternion& rotation, const Vector3D& scale)
{
	return Translate(translate) * Rotate(rotation) * ScaleXYZ(scale);
}

Vector3D operator*(const Matrix4x4& a, const Vector3D& b)
{
	return Vector3D(a.m[0][0] * b.m_x + a.m[0][1] * b.m_y + a.m[0][2] * b.m_z + a.m[0][3] * 1.0f, 
					a.m[1][0] * b.m_x + a.m[1][1] * b.m_y + a.m[1][2] * b.m_z + a.m[1][3] * 1.0f,
					a.m[2][0] * b.m_x + a.m[2][1] * b.m_y + a.m[2][2] * b.m_z + a.m[2][3] * 1.0f);
}

Vector3D operator*(const Vector3D& b, const Matrix4x4& a)
{
	return Vector3D(a.m[0][0] * b.m_x + a.m[0][1] * b.m_y + a.m[0][2] * b.m_z + a.m[0][3] * 1.0f,
		a.m[1][0] * b.m_x + a.m[1][1] * b.m_y + a.m[1][2] * b.m_z + a.m[1][3] * 1.0f,
		a.m[2][0] * b.m_x + a.m[2][1] * b.m_y + a.m[2][2] * b.m_z + a.m[2][3] * 1.0f);
}

Vector4D operator*(const Vector4D& b, const Matrix4x4& a)
{
	return Vector4D(a.m[0][0] * b.m_x + a.m[0][1] * b.m_y + a.m[0][2] * b.m_z + a.m[0][3] * b.m_w,
					a.m[1][0] * b.m_x + a.m[1][1] * b.m_y + a.m[1][2] * b.m_z + a.m[1][3] * b.m_w,
					a.m[2][0] * b.m_x + a.m[2][1] * b.m_y + a.m[2][2] * b.m_z + a.m[2][3] * b.m_w,
					a.m[3][0] * b.m_x + a.m[3][1] * b.m_y + a.m[3][2] * b.m_z + a.m[3][3] * b.m_w);
}

Vector4D operator*(const Matrix4x4& a, const Vector4D& b)
{
	return Vector4D(a.m[0][0] * b.m_x + a.m[0][1] * b.m_y + a.m[0][2] * b.m_z + a.m[0][3] * b.m_w,
		a.m[1][0] * b.m_x + a.m[1][1] * b.m_y + a.m[1][2] * b.m_z + a.m[1][3] * b.m_w,
		a.m[2][0] * b.m_x + a.m[2][1] * b.m_y + a.m[2][2] * b.m_z + a.m[2][3] * b.m_w,
		a.m[3][0] * b.m_x + a.m[3][1] * b.m_y + a.m[3][2] * b.m_z + a.m[3][3] * b.m_w);
}


std::array<float, 16> Matrix4x4::GetAsArray() const
{
	return { m[0][0], m[0][1], m[0][2], m[0][3],
			 m[1][0], m[1][1], m[1][2], m[1][3],
			 m[2][0], m[2][1], m[2][2], m[2][3],
			 m[3][0], m[3][1], m[3][2], m[3][3],
	};
}

Matrix4x4 Matrix4x4::Orthographic(float left, float right, float bottom, float top, float near, float far)
{
	Matrix4x4 ortho = Matrix4x4(2.0f / (right - left), 0, 0, -(right + left) / (right - left),
								0, 2.0f / (top - bottom), 0, -(top + bottom) / (top - bottom),
								0, 0, -2.0f / (far - near), -(far + near) / (far - near),
								0,0,0,1);

	return ortho;
}
#include <iostream>

Matrix4x4 Matrix4x4::Perspective(float left, float right, float bottom, float top, float near, float far) //Work
{
	Matrix4x4 perspective = Matrix4x4(2.0f * near / (right - left), 0, (right + left) / (right - left), 0,
		0, 2.0f * near / (top - bottom), (top + bottom) / (top - bottom), 0,
		0, 0, -(far+near) / (far - near), (-2.0f * far * near) / (far - near),
		0, 0, -1, 0);

	return perspective;
}

Matrix4x4 Matrix4x4::Perspective(float fov, float aspect, float near, float far) //Work
{
	Matrix4x4 perspective = Matrix4x4(1.0f / (aspect * tanf(fov * 0.5f)), 0, 0, 0,
		0, 1.0f / (tanf(fov * 0.5f)), 0, 0,
		0, 0, -(far + near) / (far - near), (-2.0f * far * near) / (far - near),
		0, 0, -1, 0);



	return perspective;

}

Matrix4x4 Matrix4x4::InfinitePerspective(float left, float right, float bottom, float top, float near, float epsilon) //Work
{
	Matrix4x4 perspective = Matrix4x4(2.0f * near / (right - left), 0, (right + left) / (right - left), 0,
		0, 2.0f * near / (top - bottom), (top + bottom) / (top - bottom), 0,
		0, 0, -1 * (1 - epsilon), -2 * near * (1 - epsilon),
		0, 0, -1, 0);
	return perspective;
}

Matrix4x4 Matrix4x4::PerspectiveReverseZ(float left, float right, float bottom, float top, float near, float far) //work
{

	Matrix4x4 perspective = Matrix4x4(2.0f * near / (right - left), 0, (right + left) / (right - left), 0,
		0, 2.0f * near / (top - bottom), (top + bottom) / (top - bottom), 0,
		0, 0, -(near + far) / (near - far), (-2.0f * near * far) / (near - far),
		0, 0, -1, 0);

	return perspective;
}

Matrix4x4 Matrix4x4::InfinitePerspectiveReverseZ(float left, float right, float bottom, float top, float near, float epsilon) //work
{
	Matrix4x4 perspective = Matrix4x4(2.0f * near / (right - left), 0, (right + left) / (right - left), 0,
		0, 2.0f * near / (top - bottom), (top + bottom) / (top - bottom), 0,
		0, 0, 1 * (1 - epsilon), 2 * near * (1 - epsilon),
		0, 0, -1, 0);

	return perspective;
}


Matrix4x4 Matrix4x4::PerspectiveDepthZeroToOne(float left, float right, float bottom, float top, float near, float far) //Work
{
	Matrix4x4 perspective = Matrix4x4(2.0f * near / (right - left), 0, (right + left) / (right - left), 0,
		0, 2.0f * near / (top - bottom), (top + bottom) / (top - bottom), 0,
		0, 0, -far / (far - near), -near * far / (far - near),
		0, 0, -1, 0);

	return perspective;
}
	
Matrix4x4 Matrix4x4::InfinitePerspectiveDepthZeroToOne(float left, float right, float bottom, float top, float near, float epsilon) //Work
{
	Matrix4x4 perspective = Matrix4x4(2.0f * near / (right - left), 0, (right + left) / (right - left), 0,
		0, 2.0f * near / (top - bottom), (top + bottom) / (top - bottom), 0,
		0, 0, -1 * (1 - epsilon), -near * (1 - epsilon),
		0, 0, -1, 0);

	return perspective;
}

Matrix4x4 Matrix4x4::PerspectiveReverseZDepthZeroToOne(float left, float right, float bottom, float top, float near, float far) // doesn't work
{
	Matrix4x4 perspective = Matrix4x4(2.0f * near / (right - left), 0, (right + left) / (right - left), 0,
		0, 2.0f * near / (top - bottom), (top + bottom) / (top - bottom), 0,
		0, 0, -near/(near-far), -near*far/(near-far),
		0, 0, -1, 0);

	return perspective;
}

Matrix4x4 Matrix4x4::InfinitePerspectiveReverseZDepthZeroToOne(float left, float right, float bottom, float top, float near, float epsilon) // doesn't work
{
	Matrix4x4 perspective = Matrix4x4(2.0f * near / (right - left), 0, (right + left) / (right - left), 0,
		0, 2.0f * near / (top - bottom), (top + bottom) / (top - bottom), 0,
		0, 0, 0, near * (1 - epsilon),
		0, 0, -1, 0);

	return perspective;
}


Matrix4x4::operator Matrix3x3() const
{
	return Matrix3x3(
		m[0][0], m[0][1], m[0][2],
		m[1][0], m[1][1], m[1][2],
		m[2][0], m[2][1], m[2][2]
	);
}