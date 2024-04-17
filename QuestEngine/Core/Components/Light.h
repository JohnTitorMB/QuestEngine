#ifndef  _LIGHT_H_
#define _LIGHT_H_
#include "../Color.h"
#include "../../Math/Vector3D.h"
#include "../Component.h"
#include "SceneComponent.h"
#include <iostream>
class LightComponent : public SceneComponent
{
protected:
	~LightComponent() = default;
	LightComponent();
	LightComponent(const LightComponent& other);
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

public:
	Component* Clone()override;
	void AssignPointerAndReference()override;
};

#endif