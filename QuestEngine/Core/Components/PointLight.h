#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_
#include "Light.h"
#include "../../Math/Vector3D.h"
class PointLightComponent : public LightComponent
{

public:
	PointLightComponent();
	Vector3D m_position;
	float m_constantValue;
	float m_linearValue;
	float m_quadraticValue;
};

#endif

