#ifndef  _LIGHT_H_
#define _LIGHT_H_
#include "../Color.h"
#include "../../Math/Vector3D.h"
#include "../Component.h"
#include <iostream>
class LightComponent : public Component
{
protected:
	~LightComponent() = default;
	LightComponent();
public:

	enum class LightType
	{
		Directional,
		Point,
		Spot
	};

	LightType m_lightType;
	Color m_ambiantColor;
	Color m_diffuseColor;
	Color m_specularColor;
	float m_intensity = 1.0f;	
};



#endif