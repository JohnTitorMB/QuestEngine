#ifndef _DIRECTIONALLIGHT_H_
#define _DIRECTIONALLIGHT_H_
#include "Light.h"
#include "../Assets/RenderTexture2D.h"

class DirectionalLightComponent : public LightComponent
{
	RenderTexture2D* m_shadowMap = nullptr;
	float m_shadowDistance = 15.0f;
	float m_shadowSize = 10.0f;
	float m_shadowNear = 0.1f;
	float m_shadowFar = 1000.0f;
	float m_shadowMinBias = 0.005f;
	float m_shadowMaxBias = 0.05f;
	int m_shadowMapWidth = 4096;
	int m_shadowMapHeight = 4096;
	Vector3D m_shadowMapTargetPos = Vector3D::Zero;
	int m_shadowBlurResolution = 1;
public:
	DirectionalLightComponent();
	DirectionalLightComponent(const DirectionalLightComponent& other);
	~DirectionalLightComponent();
	void Start()override;
	void SetShadowDistance(float distance);
	void SetShadowSize(float size);
	void SetShadowMapSize(int width, int height);
	void SetShadowMapTargetPos(Vector3D pos);
	void SetShadowNear(float near);
	void SetShadowFar(float far);
	void SetShadowMinBias(float bias);
	void SetShadowMaxBias(float bias);
	void SetShadowBlurResolution(int blurResolution);

	RenderTexture2D* GetShadowMap()const;
	float GetShadowDistance()const;
	float GetShadowSize()const;
	float GetShadowNear()const;
	float GetShadowFar()const;
	float GetShadowMinBias()const;
	float GetShadowMaxBias()const;
	int GetShadowMapWidth()const;
	int GetShadowMapHeight()const;
	Vector3D GetShadowMapTargetPos()const;
	int GetShadowBlurResolution()const;

	Component* Clone()override;
	void AssignPointerAndReference()override;
};
#endif

