#include "SpotLight.h"
#include "../AssetsManager.h"
SpotLightComponent::SpotLightComponent() :LightComponent()
{
	m_lightType = LightType::Spot;
	m_linearValue = 1.0f;
	m_constantValue = 1.0f;
	m_quadraticValue = 1.0f;

	m_spotAngle = 45;
	m_spotSmoothValue = 0.1f;
}

SpotLightComponent::SpotLightComponent(const SpotLightComponent& other) : LightComponent(other)
{
	m_spotAngle = other.m_spotAngle;
	m_spotSmoothValue = other.m_spotSmoothValue;
	m_constantValue = other.m_constantValue;
	m_linearValue = other.m_linearValue;
	m_quadraticValue = other.m_quadraticValue;

	m_shadowMapWidth = other.m_shadowMapWidth;
	m_shadowMapHeight = other.m_shadowMapHeight;
	m_shadowNear = other.m_shadowNear;
	m_shadowFar = other.m_shadowFar;
	m_shadowBlurResolution = other.m_shadowBlurResolution;
	m_shadowMinBias = other.m_shadowMinBias;
	m_shadowMaxBias = other.m_shadowMaxBias;
}

void SpotLightComponent::Start()
{
	m_shadowMap = AssetsManager::CreateRenderTexture2D("ShadowMap", m_shadowMapWidth, m_shadowMapHeight);

	Texture::LayerTextureInfo layerTextureInfo = Texture::LayerTextureInfo();
	layerTextureInfo.m_generateMimpap = false;
	layerTextureInfo.m_minificationFilter = MinificationFilter::Point;
	layerTextureInfo.m_magnificationFilter = MagnificationFilter::Point;
	layerTextureInfo.m_wrapVerticalParameter = Wrap::ClampToBorder;
	layerTextureInfo.m_wrapHorizontalParameter = Wrap::ClampToBorder;
	layerTextureInfo.m_borderColor = Vector4D(1, 1, 1, 1);
	m_shadowMap->AttachDepthTextureBuffer(DepthRenderableFormat::DEPTH_COMPONENT, DataType::FLOAT, layerTextureInfo);
}

RenderTexture2D* SpotLightComponent::GetShadowMap() const
{
	return m_shadowMap;
}

void SpotLightComponent::SetShadowMapSize(int width, int height)
{
	m_shadowMapWidth = width;
	m_shadowMapHeight = height;
	if (m_shadowMap != nullptr)
		m_shadowMap->Resize(m_shadowMapWidth, m_shadowMapHeight);
}

void SpotLightComponent::SetShadowNear(float near)
{
	m_shadowNear = near;
}

void SpotLightComponent::SetShadowFar(float far)
{
	m_shadowFar = far;
}

void SpotLightComponent::SetShadowBlurResolution(int blurResolution)
{
	m_shadowBlurResolution = blurResolution;
}

void SpotLightComponent::SetShadowMinBias(float bias)
{
	m_shadowMinBias = bias;
}

void SpotLightComponent::SetShadowMaxBias(float bias)
{
	m_shadowMaxBias = bias;
}

float SpotLightComponent::GetShadowNear() const
{
	return m_shadowNear;
}

float SpotLightComponent::GetShadowFar() const
{
	return m_shadowFar;
}

int SpotLightComponent::GetShadowMapWidth() const
{
	return m_shadowMapWidth;
}

int SpotLightComponent::GetShadowMapHeight() const
{
	return m_shadowMapHeight;
}

int SpotLightComponent::GetShadowBlurResolution() const
{
	return m_shadowBlurResolution;
}

float SpotLightComponent::GetShadowMinBias() const
{
	return m_shadowMinBias;
}

float SpotLightComponent::GetShadowMaxBias() const
{
	return m_shadowMaxBias;
}

Component* SpotLightComponent::Clone()
{
	SpotLightComponent* spotLightComponent = new SpotLightComponent(*this);
	clonnedObject = spotLightComponent;
	clonnedObject->baseObject = this;
	return spotLightComponent;
}

void SpotLightComponent::AssignPointerAndReference()
{
	LightComponent::AssignPointerAndReference();
}
