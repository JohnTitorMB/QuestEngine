#ifndef _DIRECTIONALLIGHT_H_
#define _DIRECTIONALLIGHT_H_
#include "Light.h"

class DirectionalLightComponent : public LightComponent
{

public:
	DirectionalLightComponent();
	DirectionalLightComponent(const DirectionalLightComponent& other);
	~DirectionalLightComponent();
	Component* Clone()override;
	void AssignPointerAndReference()override;
};
#endif

