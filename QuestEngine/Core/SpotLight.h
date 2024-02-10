#ifndef _SPOTLIGHT_H_
#define _SPOTLIGHT_H_
#include "Light.h"
class SpotLight : public Light
{
	
public:


	Vector3D m_position;
	Vector3D m_direction;
	float m_spotAngle;
	float m_spotSmoothValue;

	float m_constantValue;
	float m_linearValue;
	float m_quadraticValue;
	SpotLight();
};

#endif

