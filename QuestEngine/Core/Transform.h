#ifndef _TRANSFORM_H_
#define  _TRANSFORM_H_
#include "../Math/Vector2D.h"
#include "../Math/Matrix3x3.h"
struct Transform
{
public :
	Transform();
	Transform(Vector2D position, Vector2D scale, float angle);
	Vector2D m_position;
	Vector2D m_scale;
	float m_angle;

	Matrix3x3 TransformMatrix();
};

#endif // !_TRANSFORM_H_


