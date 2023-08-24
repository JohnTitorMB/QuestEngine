#ifndef _TRANSFORM_H_
#define  _TRANSFORM_H_
#include "../Math/Vector3D.h"
#include "../Math/Matrix4x4.h"
struct Transform
{
public :
	Transform();
	Transform(Vector3D position, Vector3D scale, Vector3D angles);
	Vector3D m_position;
	Vector3D m_scale;
	Vector3D m_angles;

	Matrix4x4 TransformMatrix()const;
};

#endif // !_TRANSFORM_H_


