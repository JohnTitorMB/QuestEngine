#include "DirectionalLight.h"
#include "../AssetsManager.h"

DirectionalLightComponent::DirectionalLightComponent() : LightComponent()
{
	m_lightType = LightType::Directional;	
}

DirectionalLightComponent::DirectionalLightComponent(const DirectionalLightComponent& other) : LightComponent(other)
{
	m_shadowDistance = other.m_shadowDistance;
	m_shadowSize = other.m_shadowSize;
	m_shadowMapWidth = other.m_shadowMapWidth;
	m_shadowMapHeight = other.m_shadowMapHeight;
	m_shadowNear = other.m_shadowNear;
	m_shadowFar = other.m_shadowFar;
	m_shadowMinBias = other.m_shadowMinBias;
	m_shadowMaxBias = other.m_shadowMaxBias;
	m_shadowMapTargetPos = other.m_shadowMapTargetPos;
	m_shadowBlurResolution = other.m_shadowBlurResolution;
}

DirectionalLightComponent::~DirectionalLightComponent()
{

}

void DirectionalLightComponent::Start()
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

void DirectionalLightComponent::SetShadowDistance(float distance)
{
	m_shadowDistance = distance;
}

void DirectionalLightComponent::SetShadowSize(float size)
{
	m_shadowSize = size;
}

void DirectionalLightComponent::SetShadowMapSize(int width, int height)
{
	m_shadowMapWidth = width;
	m_shadowMapHeight = height;
	if (m_shadowMap != nullptr)
		m_shadowMap->Resize(m_shadowMapWidth, m_shadowMapHeight);
}

void DirectionalLightComponent::SetShadowMapTargetPos(Vector3D pos)
{
	m_shadowMapTargetPos = pos;
}

void DirectionalLightComponent::SetShadowNear(float near)
{
	m_shadowNear = near;
}

void DirectionalLightComponent::SetShadowFar(float far)
{
	m_shadowFar = far;
}

void DirectionalLightComponent::SetShadowMinBias(float bias)
{
	m_shadowMinBias = bias;
}

void DirectionalLightComponent::SetShadowMaxBias(float bias)
{
	m_shadowMaxBias = bias;
}

void DirectionalLightComponent::SetShadowBlurResolution(int blurResolution)
{
	m_shadowBlurResolution = blurResolution;
}

RenderTexture2D* DirectionalLightComponent::GetShadowMap() const
{
	return m_shadowMap;
}

float DirectionalLightComponent::GetShadowDistance() const
{
	return m_shadowDistance;
}

float DirectionalLightComponent::GetShadowSize() const
{
	return m_shadowSize;
}

float DirectionalLightComponent::GetShadowNear() const
{
	return m_shadowNear;
}

float DirectionalLightComponent::GetShadowFar() const
{
	return m_shadowFar;
}

float DirectionalLightComponent::GetShadowMinBias() const
{
	return m_shadowMinBias;
}

float DirectionalLightComponent::GetShadowMaxBias() const
{
	return m_shadowMaxBias;
}

int DirectionalLightComponent::GetShadowMapWidth() const
{
	return m_shadowMapWidth;
}

int DirectionalLightComponent::GetShadowMapHeight() const
{
	return m_shadowMapHeight;
}

Vector3D DirectionalLightComponent::GetShadowMapTargetPos() const
{
	return m_shadowMapTargetPos;
}

int DirectionalLightComponent::GetShadowBlurResolution() const
{
	return m_shadowBlurResolution;
}

Component* DirectionalLightComponent::Clone()
{
	DirectionalLightComponent* directionalLightComponent = new DirectionalLightComponent(*this);
	clonnedObject = directionalLightComponent;
	clonnedObject->baseObject = this;
	return directionalLightComponent;
}

void DirectionalLightComponent::AssignPointerAndReference()
{
	LightComponent::AssignPointerAndReference();
}
