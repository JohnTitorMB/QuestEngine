#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_
#include "Light.h"
class PointLight : public Light
{
	
public:
	Vector3D m_position;
	float m_constantValue;
	float m_linearValue;
	float m_quadraticValue;
	PointLight();
};

#endif

