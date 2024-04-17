#ifndef _SPOTLIGHT_H_
#define _SPOTLIGHT_H_
#include "Light.h"
class SpotLightComponent : public LightComponent
{
public:

	SpotLightComponent();
	SpotLightComponent(const SpotLightComponent& other);

	float m_spotAngle;
	float m_spotSmoothValue;

	float m_constantValue;
	float m_linearValue;
	float m_quadraticValue;

	Component* Clone()override;
	void AssignPointerAndReference()override;
};

#endif

