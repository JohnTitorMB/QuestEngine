#ifndef _DIRECTIONALLIGHT_H_
#define _DIRECTIONALLIGHT_H_
#include "Light.h"

class DirectionalLight : public Light
{

public:
	Vector3D m_direction;
	DirectionalLight();
};
#endif
