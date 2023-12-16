#ifndef _TRANSFORM_H_
#define  _TRANSFORM_H_
#include "../Math/Vector3D.h"
#include "../Math/Matrix4x4.h"
#include "../Math/Quaternion.h"
struct Transform
{
	Vector3D m_position;
	Vector3D m_scale;
	Quaternion m_rotation;
public :
	Transform();
	Transform(Vector3D position, Vector3D scale, Quaternion rotation);

	void SetPosition(const Vector3D& position);
	void SetScale(const Vector3D& scale);
	void SetEulerAngles(const Vector3D& eulerAngles);
	void SetRotation(const Quaternion& rotation);

	Vector3D GetPosition();
	Vector3D GetScale();
	Vector3D GetEulerAngles();
	Quaternion GetRotation();

	Matrix4x4 TransformMatrix()const;
};

#endif // !_TRANSFORM_H_


