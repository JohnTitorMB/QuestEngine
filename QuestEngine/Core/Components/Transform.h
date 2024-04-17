#ifndef _TRANSFORM_H_
#define  _TRANSFORM_H_
#include "../../Math/Vector3D.h"
#include "../../Math/Matrix4x4.h"
#include "../../Math/Quaternion.h"
#include "../Component.h"

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

	Vector3D GetPosition()const;
	Vector3D GetScale()const;
	Vector3D GetEulerAngles()const;
	Quaternion GetRotation()const;

	Matrix4x4 TransformMatrix()const;

	Vector3D TransformPosition(const Vector3D& v)const;
	Vector3D InverseTransformPosition(const Vector3D& v)const;

	Vector3D TransformVector(const Vector3D& v)const;
	Vector3D InverseTransformVector(const Vector3D& v)const;

	Vector3D TransformVectorWithoutScale(const Vector3D& v)const;
	Vector3D InverseTransformVectorWithoutScale(const Vector3D& v)const;
};

#endif // !_TRANSFORM_H_


