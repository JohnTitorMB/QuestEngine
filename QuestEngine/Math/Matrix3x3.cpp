#include "Matrix3x3.h"
# define M_PI           3.14159265358979323846  /* pi */

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

Matrix3x3::Matrix3x3(const Matrix3x3& value)
{
	m[0][0] = value.m[0][0];
	m[0][1] = value.m[0][1];
	m[0][2] = value.m[0][2];

	m[1][0] = value.m[1][0];
	m[1][1] = value.m[1][1];
	m[1][2] = value.m[1][2];

	m[2][0] = value.m[2][0];
	m[2][1] = value.m[2][1];
	m[2][2] = value.m[2][2];
}


float& Matrix3x3::operator ()(int i, int j)
{
	return m[i][j];
}

const float& Matrix3x3::operator ()(int i, int j)const
{
	return m[i][j];
}

Matrix3x3& Matrix3x3::operator +=(const Matrix3x3& b)
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

Matrix3x3& Matrix3x3::operator -=(const Matrix3x3& b)
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

Matrix3x3& Matrix3x3::operator *=(const float& s)
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

Matrix3x3& Matrix3x3::operator *=(const Matrix3x3& b)
{
	Matrix3x3 result;

	result.m[0][0] = m[0][0] * b.m[0][0] + m[0][1] * b.m[1][0] + m[0][2] * b.m[2][0];
	result.m[0][1] = m[0][0] * b.m[0][1] + m[0][1] * b.m[1][1] + m[0][2] * b.m[2][1];
	result.m[0][2] = m[0][0] * b.m[0][2] + m[0][1] * b.m[1][2] + m[0][2] * b.m[2][2];

	result.m[1][0] = m[1][0] * b.m[0][0] + m[1][1] * b.m[1][0] + m[1][2] * b.m[2][0];
	result.m[1][1] = m[1][0] * b.m[0][1] + m[1][1] * b.m[1][1] + m[1][2] * b.m[2][1];
	result.m[1][2] = m[1][0] * b.m[0][2] + m[1][1] * b.m[1][2] + m[1][2] * b.m[2][2];

	result.m[2][0] = m[2][0] * b.m[0][0] + m[2][1] * b.m[1][0] + m[2][2] * b.m[2][0];
	result.m[2][1] = m[2][0] * b.m[0][1] + m[2][1] * b.m[1][1] + m[2][2] * b.m[2][1];
	result.m[2][2] = m[2][0] * b.m[0][2] + m[2][1] * b.m[1][2] + m[2][2] * b.m[2][2];

	*this = result;
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

bool Matrix3x3::operator==(const Matrix3x3& value) const
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (m[i][j] != value.m[i][j])
				return false;
		}
	}
	return true;
}

bool Matrix3x3::operator!=(const Matrix3x3& value) const
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (m[i][j] != value.m[i][j])
				return true;
		}
	}
	return false;
}

std::ostream& operator<<(std::ostream& os, const Matrix3x3& value)
{
	os << "(m00 : " << value.m[0][0] << ", m01 : " << value.m[0][1] << ", m02 : " << value.m[0][2] << ")" << std::endl
		<< "(m10 : " << value.m[1][0] << ", m11 : " << value.m[1][1] << ", m12 : " << value.m[1][2] << ")" << std::endl
		<< "(m20 : " << value.m[2][0] << ", m21 : " << value.m[2][1] << ", m22 : " << value.m[2][2] << ")" << std::endl;
	return os;
}

Matrix3x3 Matrix3x3::Identity()
{
	return Matrix3x3(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f);
}

Matrix3x3 Matrix3x3::Translate(const Vector2D& translate)
{
	return Matrix3x3(1.0f, 0.0f, translate.m_x,
					 0.0f, 1.0f, translate.m_y,
					 0.0f, 0.0f, 1.0f);
}

Matrix3x3 Matrix3x3::RotateZ(const float& angle)
{
	float angleInRadiant = angle * M_PI / 180.0f;
	return Matrix3x3(cosf(angleInRadiant), -sinf(angleInRadiant), 0.0f,
					 sinf(angleInRadiant), cosf(angleInRadiant), 0.0f,
					 0.0f,0.0f,1.0f);
}

Matrix3x3 Matrix3x3::ScaleXY(const Vector2D& scale)
{
	return Matrix3x3(scale.m_x, 0.0f, 0.0f,
		0.0f, scale.m_y, 0.0f,
		0.0f, 0.0f, 1.0f);
}

Matrix3x3 Matrix3x3::TS(const Vector2D& translate, const Vector2D& scale)
{
	return Translate(translate) * ScaleXY(scale);
}

Matrix3x3 Matrix3x3::RS(const float& angle, const Vector2D& scale)
{
	return RotateZ(angle) * ScaleXY(scale);
}

Matrix3x3 Matrix3x3::TR(const Vector2D& translate, const float& angle)
{
	return Translate(translate) * RotateZ(angle);
}

Matrix3x3 Matrix3x3::TRS(const Vector2D& translate, const float& angle, const Vector2D& scale)
{
	return Translate(translate) * RotateZ(angle) * ScaleXY(scale);
}

Vector2D operator*(const Matrix3x3& a, const Vector2D& b)
{
	return Vector2D(a.m[0][0] * b.m_x + a.m[0][1] * b.m_y + a.m[0][2] * 1.0f, a.m[1][0] * b.m_x + a.m[1][1] * b.m_y + a.m[1][2] * 1.0f);
}

Vector2D operator*(const Vector2D& b, const Matrix3x3& a)
{
	return Vector2D(b.m_x * a.m[0][0] + b.m_y * a.m[0][1] + 1.0f * a.m[0][2], b.m_x * a.m[1][0] + b.m_y * a.m[1][1] + 1.0f * a.m[1][2]);
}

std::array<float, 9> Matrix3x3::GetAsArray() const
{
	return { m[0][0], m[0][1], m[0][2],
			m[1][0], m[1][1], m[1][2],
			m[2][0], m[2][1], m[2][2] };
}