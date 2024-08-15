#ifndef _SKYCYLECOMPONENT_H_
#define _SKYCYLECOMPONENT_H_

#include "../Core/Component.h"
#include "../Math/Vector4D.h"
#include "../Core/Color.h"
#include "../Core/Assets/Material.h"
#include "../Core/Components/DirectionalLight.h"
class SkydomeCycleComponent : public Component
{
protected:
	void Update() override;
	void Start() override;
public:
	SkydomeCycleComponent();
	SkydomeCycleComponent(const SkydomeCycleComponent& SkydomeCycleComponent);
	~SkydomeCycleComponent();
	Component* Clone()override;
	void AssignPointerAndReference()override;

	float time = 16.86f;
	float cycleDuration = 60.0f;
	Material* skyDomeMaterial;
	DirectionalLightComponent* directionalLightComponent;
	Color sunRiseColorA = Color(124/255.0f, 181/255.0f, 198/255.0f, 1.0f);
	Color sunRiseColorB = Color(255 / 255.0f, 233 / 255.0f, 111 / 255.0f, 1.0f);

	Color morningColorA = Color(145 / 255.0f, 193 / 255.0f, 203 / 255.0f, 1.0f);
	Color morningColorB = Color(169 / 255.0f, 232 / 255.0f, 241 / 255.0f, 1.0f);

	Color afternoonColorA = Color(60 / 255.0f, 127 / 255.0f, 170 / 255.0f, 1.0f);
	Color afternoonColorB = Color(112 / 255.0f, 188 / 255.0f, 220 / 255.0f, 1.0f);

	Color sunsetColorA = Color(153 / 255.0f, 69 / 255.0f, 102 / 255.0f, 1.0f);
	Color sunsetColorB = Color(243 / 255.0f, 182 / 255.0f, 5 / 255.0f, 1.0f);

	Color duskColorA = Color(51 / 255.0f, 42 / 255.0f, 89 / 255.0f, 1.0f);
	Color duskColorB = Color(213 / 255.0f, 93 / 255.0f, 17 / 255.0f, 1.0f);

	Color dawnColorA = Color(90 / 255.0f, 48 / 255.0f, 88 / 255.0f, 1.0f);
	Color dawnColorB = Color(171 / 255.0f, 118 / 255.0f, 102 / 255.0f, 1.0f);

	Color nightColorA = Color(18 / 255.0f, 17 / 255.0f, 35 / 255.0f, 1.0f);
	Color nightColorB = Color(45 / 255.0f, 42 / 255.0f, 85 / 255.0f, 1.0f);


	float sunRiseTime = 0.281f;
	float morningTime = 0.3905f;
	float afternoonTime = 0.625f;
	float sunsetTime = 0.865f;
	float duskTime = 0.875f;
	float nightTime = 0.0725f;
	float dawnTime = 0.2705f;

};

#endif