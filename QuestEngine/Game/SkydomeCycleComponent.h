#ifndef _SKYCYLECOMPONENT_H_
#define _SKYCYLECOMPONENT_H_

#include "../Core/Component.h"
#include "../Math/Vector4D.h"
#include "../Core/ColorRGB.h"
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
	ColorRGB sunRiseColorA = ColorRGB(124/255.0f, 181/255.0f, 198/255.0f, 1.0f);
	ColorRGB sunRiseColorB = ColorRGB(255 / 255.0f, 233 / 255.0f, 111 / 255.0f, 1.0f);

	ColorRGB morningColorA = ColorRGB(145 / 255.0f, 193 / 255.0f, 203 / 255.0f, 1.0f);
	ColorRGB morningColorB = ColorRGB(169 / 255.0f, 232 / 255.0f, 241 / 255.0f, 1.0f);

	ColorRGB afternoonColorA = ColorRGB(60 / 255.0f, 127 / 255.0f, 170 / 255.0f, 1.0f);
	ColorRGB afternoonColorB = ColorRGB(112 / 255.0f, 188 / 255.0f, 220 / 255.0f, 1.0f);

	ColorRGB sunsetColorA = ColorRGB(153 / 255.0f, 69 / 255.0f, 102 / 255.0f, 1.0f);
	ColorRGB sunsetColorB = ColorRGB(243 / 255.0f, 182 / 255.0f, 5 / 255.0f, 1.0f);

	ColorRGB duskColorA = ColorRGB(51 / 255.0f, 42 / 255.0f, 89 / 255.0f, 1.0f);
	ColorRGB duskColorB = ColorRGB(213 / 255.0f, 93 / 255.0f, 17 / 255.0f, 1.0f);

	ColorRGB dawnColorA = ColorRGB(90 / 255.0f, 48 / 255.0f, 88 / 255.0f, 1.0f);
	ColorRGB dawnColorB = ColorRGB(171 / 255.0f, 118 / 255.0f, 102 / 255.0f, 1.0f);

	ColorRGB nightColorA = ColorRGB(18 / 255.0f, 17 / 255.0f, 35 / 255.0f, 1.0f);
	ColorRGB nightColorB = ColorRGB(45 / 255.0f, 42 / 255.0f, 85 / 255.0f, 1.0f);


	float sunRiseTime = 0.281f;
	float morningTime = 0.3905f;
	float afternoonTime = 0.625f;
	float sunsetTime = 0.865f;
	float duskTime = 0.875f;
	float nightTime = 0.0725f;
	float dawnTime = 0.2705f;

};

#endif