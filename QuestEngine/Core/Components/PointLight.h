#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_
#include "Light.h"
#include "../../Math/Vector3D.h"
#include "../Assets/RenderCubeMap.h"
class PointLightComponent : public LightComponent
{
	RenderCubeMap* m_shadowMap = nullptr;
	float m_shadowNear = 0.3f;
	float m_shadowFar = 100.0f;
	float m_shadowMinBias = 0.005f;
	float m_shadowMaxBias = 0.05f;

	int m_shadowMapWidth = 4096;
	int m_shadowMapHeight = 4096;
	int m_shadowBlurResolution = 3;
	float m_shadowBlurRange = 0.2f;
public:
	PointLightComponent();
	PointLightComponent(const PointLightComponent& other);
	void Start()override;
	RenderCubeMap* GetShadowMap()const;
	void SetShadowMapSize(int width, int height);
	void SetShadowNear(float near);
	void SetShadowFar(float far);
	void SetShadowBlurResolution(int blurResolution);
	void SetShadowBlurRange(float range);
	void SetShadowMinBias(float bias);
	void SetShadowMaxBias(float bias);

	float GetShadowNear()const;
	float GetShadowFar()const;

	int GetShadowMapWidth()const;
	int GetShadowMapHeight()const;
	int GetShadowBlurResolution()const;
	float GetShadowMinBias()const;
	float GetShadowMaxBias()const;
	float GetShadowBlurRange()const;

	float m_constantValue;
	float m_linearValue;
	float m_quadraticValue;
	Component* Clone()override;
	void AssignPointerAndReference()override;
};

#endif

