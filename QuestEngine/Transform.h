#ifndef _TRANSFORM_H_
#define  _TRANSFORM_H_
#include "Vector2D.h"

struct Transform
{
public :
	Transform();
	Transform(Vector2D position, Vector2D scale, float angle);
	Vector2D m_position;
	Vector2D m_scale;
	float m_angle;
};

#endif // !_TRANSFORM_H_


