#include "Transform.h"

Transform::Transform()
{

}

Transform::Transform(Vector2D position, Vector2D scale, float angle)
{
	m_position = position;
	m_scale = scale;
	m_angle = angle;
}