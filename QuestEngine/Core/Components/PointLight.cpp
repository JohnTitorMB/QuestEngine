#include "PointLight.h"
#include "../AssetsManager.h"

PointLightComponent::PointLightComponent() :LightComponent()
{
	m_lightType = LightType::Point;
	m_linearValue = 1.0f;
	m_constantValue = 1.0f;
	m_quadraticValue = 1.0f;
}

PointLightComponent::PointLightComponent(const PointLightComponent& other) : LightComponent(other)
{
	m_linearValue = other.m_linearValue;
	m_constantValue = other.m_constantValue;
	m_quadraticValue = other.m_quadraticValue;

	m_shadowMapWidth = other.m_shadowMapWidth;
	m_shadowMapHeight = other.m_shadowMapHeight;
	m_shadowNear = other.m_shadowNear;
	m_shadowFar = other.m_shadowFar;
	m_shadowBlurResolution = other.m_shadowBlurResolution;
	m_shadowMinBias = other.m_shadowMinBias;
	m_shadowMaxBias = other.m_shadowMaxBias;
	m_shadowBlurRange = other.m_shadowBlurRange;
}

void PointLightComponent::Start()
{
	m_shadowMap = AssetsManager::CreateRenderCubeMap("ShadowMap", m_shadowMapWidth, m_shadowMapHeight);

	Texture::LayerTextureInfo layerTextureInfo = Texture::LayerTextureInfo();
	layerTextureInfo.m_generateMimpap = false;
	layerTextureInfo.m_minificationFilter = MinificationFilter::Point;
	layerTextureInfo.m_magnificationFilter = MagnificationFilter::Point;
	layerTextureInfo.m_wrapVerticalParameter = Wrap::ClampToEdge;
	layerTextureInfo.m_wrapHorizontalParameter = Wrap::ClampToEdge;
	m_shadowMap->AttachDepthTextureBuffer(DepthRenderableFormat::DEPTH_COMPONENT, DataType::FLOAT, layerTextureInfo);
}

RenderCubeMap* PointLightComponent::GetShadowMap() const
{
	return m_shadowMap;
}

void PointLightComponent::SetShadowMapSize(int width, int height)
{
	m_shadowMapWidth = width;
	m_shadowMapHeight = height;
	if (m_shadowMap != nullptr)
		m_shadowMap->Resize(m_shadowMapWidth, m_shadowMapHeight);
}

void PointLightComponent::SetShadowNear(float near)
{
	m_shadowNear = near;
}

void PointLightComponent::SetShadowFar(float far)
{
	m_shadowFar = far;
}

void PointLightComponent::SetShadowBlurResolution(int blurResolution)
{
	m_shadowBlurResolution = blurResolution;
}

void PointLightComponent::SetShadowBlurRange(float range)
{
	m_shadowBlurRange = range;
}

void PointLightComponent::SetShadowMinBias(float bias)
{
	m_shadowMinBias = bias;
}

void PointLightComponent::SetShadowMaxBias(float bias)
{
	m_shadowMaxBias = bias;
}

float PointLightComponent::GetShadowNear() const
{
	return m_shadowNear;
}

float PointLightComponent::GetShadowFar() const
{
	return m_shadowFar;
}

int PointLightComponent::GetShadowMapWidth() const
{
	return m_shadowMapWidth;
}

int PointLightComponent::GetShadowMapHeight() const
{
	return m_shadowMapHeight;
}

int PointLightComponent::GetShadowBlurResolution() const
{
	return m_shadowBlurResolution;
}

float PointLightComponent::GetShadowMinBias() const
{
	return m_shadowMinBias;
}

float PointLightComponent::GetShadowMaxBias() const
{
	return m_shadowMaxBias;
}

float PointLightComponent::GetShadowBlurRange() const
{
	return m_shadowBlurRange;
}

Component* PointLightComponent::Clone()
{
	PointLightComponent* pointLightComponent = new PointLightComponent(*this);
	clonnedObject = pointLightComponent;
	clonnedObject->baseObject = this;
	return pointLightComponent;
}

void PointLightComponent::AssignPointerAndReference()
{
	LightComponent::AssignPointerAndReference();
}
