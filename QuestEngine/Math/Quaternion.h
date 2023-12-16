#ifndef _QUATERNION_H_
#define _QUATERNION_H_
#include "Vector3D.h"
#include "Vector2D.h"
#include "Mathf.h"
struct Matrix4x4;
struct Quaternion
{
	struct EulerAxesAngles
	{
		float m_angle1;
		float m_angle2;
		float m_angle3;
		Vector3D m_firstAxis;
		Vector3D m_secondAxis;
		Vector3D m_thirdAxis;

		EulerAxesAngles() {}

		EulerAxesAngles(float angle1, float angle2, float angle3, Vector3D firstAxis, Vector3D secondAxis, Vector3D thirdAxis)
		{
			m_angle1 = angle1;
			m_angle2 = angle2;
			m_angle3 = angle3;
			m_firstAxis = firstAxis;
			m_secondAxis = secondAxis;
			m_thirdAxis = thirdAxis;
		}
	};
	enum class EulerAngleMode
	{
		XYZ = 0,
		XZY = 1,
		YXZ = 2,
		YZX = 3,
		ZYX = 4,
		ZXY = 5,
		XYX = 6,
		YXY = 7,
		ZYZ = 8,
		YZY = 9,
		XZX = 10,
		ZXZ = 11,
	};
	//Variable
	float m_w;
	float m_x;
	float m_y;
	float m_z;

	//Constructor
	Quaternion();
	Quaternion(const Quaternion& value);
	Quaternion(const float w, const float x, const float y, const float z);
	Quaternion(const Vector3D& axis, float angle);
	Quaternion(const Vector3D& eulerAngles, EulerAngleMode eulerAngleMode = EulerAngleMode::XYZ, bool intrinsectRotation = false);

	//Method
	void Normalize();

	Quaternion Normalized()const;
	Quaternion Inverse()const;
	float Length()const;
	float LengthSquared()const;

	bool IsIdentity(float tolerence = Mathf::Epsilon7)const;
	bool IsNormalized(float tolerance = Mathf::Epsilon7)const;

	Vector3D GetForwardDirection()const;
	Vector3D GetRightDirection()const;
	Vector3D GetUpDirection()const;

	Vector3D RotateVector3D(const Vector3D& v)const;
	void Rotate(const Vector3D& axis, float angle, bool inWorld = true);
	void Rotate(const Quaternion& q, bool inWorld = true);
	void GoToRotation(const Vector3D& fromDirection, const Vector3D& toDirection);
	void GoToRotation(const Vector3D& fromDirection, const Vector3D& toDirection, const Vector3D& orthogonalDirection);

	Vector3D ToEulerAngle()const;
	float GetAngle()const;
	Vector3D GetAxis()const;
	void ToAxisAngle(Vector3D& Axis, float& angle)const;
	Matrix4x4 ToMatrix4x4() const;

	//Static function
	static Quaternion Identity();
	static float DotProduct(const Quaternion& a, const Quaternion& b);
	static Quaternion FromEulerAngle(const Vector3D& eulerAngle, EulerAngleMode eulerAngleMode = EulerAngleMode::XYZ);
	static Quaternion AxisAngle(const Vector3D& axis, const float angle);
	static Quaternion SetGoToRotation(const Vector3D& fromDirection, const Vector3D& toDirection);
	static Quaternion LookRotation(const Vector3D& forward, const Vector3D& up);
	static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t, bool longPath = false);
	static Quaternion LerpNormalized(const Quaternion& a, const Quaternion& b, float t, bool longPath = false);
	static Quaternion Slerp(const Quaternion& A, const Quaternion& B, float t, bool longPath = false);
	static Quaternion SlerpNormalized(const Quaternion& a, const Quaternion& b, float t, bool longPath = false);

	//Operator
	Quaternion operator-(const Quaternion& q)const;
	Quaternion operator+(const Quaternion& q)const;
	Quaternion operator*(const Quaternion& q)const;
	Quaternion operator*(float scale)const;
	Quaternion operator/(float scale)const;
	Quaternion operator/(const Quaternion& q)const;

	Quaternion operator-=(const Quaternion& q);
	Quaternion operator+=(const Quaternion& q);
	Quaternion operator*=(const Quaternion& q);
	Quaternion operator*=(float scale);
	Quaternion operator/=(float scale);
	Quaternion operator/=(const Quaternion& q);

	Vector3D operator*(const Vector3D& v)const;

	bool operator!=(const Quaternion& q)const;
	bool operator==(const Quaternion& q)const;
};
std::ostream& operator<<(std::ostream& os, const Quaternion& value);

#endif 

