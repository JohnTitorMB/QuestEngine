#ifndef _DIRECTIONALLIGHT_H_
#define _DIRECTIONALLIGHT_H_
#include "Light.h"

class DirectionalLightComponent : public LightComponent
{

public:
	DirectionalLightComponent();
	~DirectionalLightComponent();
	Vector3D m_direction;

};
#endif

