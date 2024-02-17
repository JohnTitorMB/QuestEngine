#ifndef _SPOTLIGHT_H_
#define _SPOTLIGHT_H_
#include "Light.h"
class SpotLightComponent : public LightComponent
{
public:

	SpotLightComponent();

	Vector3D m_position;
	Vector3D m_direction;
	float m_spotAngle;
	float m_spotSmoothValue;

	float m_constantValue;
	float m_linearValue;
	float m_quadraticValue;
};

#endif

