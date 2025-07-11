#ifndef _SPOTLIGHT_H_
#define _SPOTLIGHT_H_
#include "Light.h"
#include "../Assets/RenderTexture2D.h"
class SpotLightComponent : public LightComponent
{
	RenderTexture2D* m_shadowMap = nullptr;
	float m_shadowNear = 0.3f;
	float m_shadowFar = 100.0f;
	float m_shadowMinBias = 0.005f;
	float m_shadowMaxBias = 0.05f;
	int m_shadowMapWidth = 4096;
	int m_shadowMapHeight = 4096;
	int m_shadowBlurResolution = 1;
public:

	SpotLightComponent();
	SpotLightComponent(const SpotLightComponent& other);
	void Start()override;
	RenderTexture2D* GetShadowMap()const;
	void SetShadowMapSize(int width, int height);
	void SetShadowNear(float near);
	void SetShadowFar(float far);
	void SetShadowBlurResolution(int blurResolution);
	void SetShadowMinBias(float bias);
	void SetShadowMaxBias(float bias);

	float GetShadowNear()const;
	float GetShadowFar()const;

	int GetShadowMapWidth()const;
	int GetShadowMapHeight()const;
	int GetShadowBlurResolution()const;
	float GetShadowMinBias()const;
	float GetShadowMaxBias()const;
	float m_spotAngle;
	float m_spotSmoothValue;

	float m_constantValue;
	float m_linearValue;
	float m_quadraticValue;

	Component* Clone()override;
	void AssignPointerAndReference()override;
};

#endif

