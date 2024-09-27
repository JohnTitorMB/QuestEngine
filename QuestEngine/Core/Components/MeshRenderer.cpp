#include "MeshRenderer.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "../LightingSettings.h"
#include <iostream>
#include <sstream>
#include "../../Game/DirectionalLightController.h"
#include "../AssetsManager.h"
#include "../World.h"
# define M_PI           3.14159265358979323846f  /* pi */


void MeshRendererComponent::SendMaterialToShader()const
{
	const std::unordered_map<std::string, int> integerKeyValue = m_material->GetIntMap();
	const std::unordered_map<std::string, float> floatKeyValue = m_material->GetFloatMap();
	const std::unordered_map<std::string, Vector2D> vector2DKeyValue = m_material->GetVector2DMap();
	const std::unordered_map<std::string, Vector3D> vector3DKeyValue = m_material->GetVector3DMap();
	const std::unordered_map<std::string, Vector4D> vector4DKeyValue = m_material->GetVector4DMap();
	const std::unordered_map<std::string, Texture*> textureKeyValue = m_material->GetTextureMap();
	std::unordered_map<std::string, int> textureSubLayerKeyValue = m_material->GetTextureSubLayerMap();

	for (auto pair : integerKeyValue)
		m_shader->SetUniformInt(pair.first, pair.second);

	for (auto pair : floatKeyValue)
		m_shader->SetUniformFloat(pair.first, pair.second);

	for (auto pair : vector2DKeyValue)
		m_shader->SetUniformVector2D(pair.first, pair.second);

	for (auto pair : vector3DKeyValue)
		m_shader->SetUniformVector3D(pair.first, pair.second);

	for (auto pair : vector4DKeyValue)
		m_shader->SetUniformVector4D(pair.first, pair.second);

	int textureIndex = 0;
	for (auto pair : textureKeyValue)
	{
		m_shader->SetUniformInt(pair.first, textureIndex);

		if (pair.second)
		{
			std::string name = pair.first;
			if(textureSubLayerKeyValue[name] < pair.second->GetSubLayerCount())
				pair.second->Bind(textureIndex, textureSubLayerKeyValue[name]);
			else
				pair.second->Bind(textureIndex, 0);
		}
		textureIndex++;
	}
}

void MeshRendererComponent::Draw(CameraComponent* camera, std::set<LightComponent*>lights, Window* window)const
{
	Shader* shader = m_shader;

	switch (World::m_renderingType)
	{
	case RenderingType::UV:
		shader = AssetsManager::GetAsset<Shader>("UVShader");
		break;
	case RenderingType::Normal:
		shader = AssetsManager::GetAsset<Shader>("NormalShader");
		break;
	case RenderingType::FragCoordZ:
		shader = AssetsManager::GetAsset<Shader>("FragCoordZShader");
		break;
	case RenderingType::LinearDepth:
		shader = AssetsManager::GetAsset<Shader>("LinearDepthShader");
		break;
	case RenderingType::Fog:
		shader = AssetsManager::GetAsset<Shader>("BlinnPhongShaderFog");
		break;
	default:
		break;
	}

	if (m_mesh == nullptr|| shader == nullptr || m_material == nullptr)
		return;
	
	if (m_ovverideMultiSamplingEnable)
	{
		if (m_enableMultiSampling)
			glEnable(GL_MULTISAMPLE);
		else
			glDisable(GL_MULTISAMPLE);
	}


	glPolygonMode(GL_FRONT_AND_BACK, (int)m_polygonMode);

	if (camera != nullptr)
	{
		if (camera->GetUseDepthZeroToOneProjection())
			glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
		else
			glClipControl(GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE);
	}

	//Cull Face Specification
	if (m_isCullFaceEnable)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	glCullFace((int)m_cullFace);
	glFrontFace((int)m_frontFace);

	//Stencil Specification
	if (m_isStencilTestEnable)
		glEnable(GL_STENCIL_TEST);
	else
		glDisable(GL_STENCIL_TEST);

	glStencilMaskSeparate(GL_BACK, m_stencilBackMask);
	glStencilFuncSeparate(GL_BACK, (int)m_stencilTestBackFunc, m_stencilTestBackRef, m_stencilTestBackMask);
	glStencilOpSeparate(GL_BACK, (int)m_stencilFailBackAction, (int)m_depthFailBackAction, (int)m_depthPassBackAction);

	glStencilMaskSeparate(GL_FRONT, m_stencilFrontMask);
	glStencilFuncSeparate(GL_FRONT, (int)m_stencilTestFrontFunc, m_stencilTestFrontRef, m_stencilTestFrontMask);
	glStencilOpSeparate(GL_FRONT, (int)m_stencilFailFrontAction, (int)m_depthFailFrontAction, (int)m_depthPassFrontAction);


	//Depth Specification
	if (m_isDepthTestEnable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	glDepthMask(m_isDepthMaskEnable);
	glDepthFunc((int)m_depthTestFunc);

	//Blend Specification

	//Blending Specification
	if (m_isBlendEnable)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);

	glBlendEquationSeparate((int)m_RGBBlendingMode, (int)m_AlphaBlendingMode);
	glBlendFuncSeparate((int)m_sourceRGBBlendingFactor, (int)m_destinationRGBBlendingFactor, (int)m_sourceAlphaBlendingFactor, (int)m_destinationAlphaBlendingFactor);
	glBlendColor(m_blendColor.m_r, m_blendColor.m_g, m_blendColor.m_b, m_blendColor.m_a);
	
	//Color Mask
	glColorMask(m_isRedMaskEnable, m_isGreenMaskEnable, m_isBlueMaskEnable, m_isAlphaMaskEnable);




	shader->UseShader();
	
	if (camera != nullptr)
	{
		shader->SetUniformFloat("viewNear", camera->GetNear());
		shader->SetUniformFloat("viewFar", camera->GetFar());
		shader->SetUniformVector3D("viewDirection", camera->GetForwardVector());

	}
	
	Matrix4x4 modelMatrix = GetTransform().TransformMatrix();
	shader->SetUniformMatrix4x4("model", modelMatrix);

	if (camera != nullptr)
	{
		if (!m_useViewMatrixWithoutTranslation)
			shader->SetUniformMatrix4x4("view", camera->ViewMatrix());
		else
			shader->SetUniformMatrix4x4("view", camera->ViewMatrixWithoutTranslation());

		RenderTexture2D* rt = camera->GetRenderTexture();
		int width = window->GetWidth();
		int height = window->GetHeight();
		if (rt != nullptr)
		{
			width = rt->GetWidth();
			height = rt->GetHeight();
		}



		float bCornerX = camera->m_viewportBottomCornerX * width;
		float bCornerY = camera->m_viewportBottomCornerY * height;

		float tCornerX = camera->m_viewportTopCornerX * width;
		float tCornerY = camera->m_viewportTopCornerY * height;

		shader->SetUniformMatrix4x4("projection", camera->ProjectionMatrix(tCornerX - bCornerX, tCornerY - bCornerY));

		
		shader->SetUniformVector3D("uViewPos", camera->GetTransform().GetPosition());
	}

	Matrix3x3 normalMatrix = (Matrix3x3)(modelMatrix).Inverse().Transpose();
	shader->SetUniformMatrix3x3("normalMatrix", normalMatrix);



	SendMaterialToShader();

	int directionalLightCounter = 0;
	int spotLightCounter = 0;
	int pointLightCounter = 0;
	for (auto it = lights.begin(); it != lights.end(); ++it)
	{
		LightComponent* light = *it;
		if (light->m_lightType == LightComponent::LightType::Directional)
		{
			DirectionalLightComponent* directionalLight = dynamic_cast<DirectionalLightComponent*>(light);
			shader->SetUniformColor("directionalLight.ambientColor", directionalLight->m_ambiantColor);
			shader->SetUniformColor("directionalLight.diffuseColor", directionalLight->m_diffuseColor);
			shader->SetUniformColor("directionalLight.specularColor", directionalLight->m_specularColor);
			shader->SetUniformVector3D("directionalLight.direction", directionalLight->GetForwardVector());
			shader->SetUniformFloat("directionalLight.intensity", directionalLight->m_intensity);
			directionalLightCounter++;
		}
		else if (light->m_lightType == LightComponent::LightType::Point)
		{
			PointLightComponent* pointLight = dynamic_cast<PointLightComponent*>(light);
			std::string ambiant = (std::ostringstream() << "pointLights[" << pointLightCounter << "].ambientColor").str();
			std::string diffuse = (std::ostringstream() << "pointLights[" << pointLightCounter << "].diffuseColor").str();
			std::string specular = (std::ostringstream() << "pointLights[" << pointLightCounter << "].specularColor").str();
			std::string position = (std::ostringstream() << "pointLights[" << pointLightCounter << "].position").str();

			std::string constant = (std::ostringstream() << "pointLights[" << pointLightCounter << "].constant").str();
			std::string linear = (std::ostringstream() << "pointLights[" << pointLightCounter << "].linear").str();
			std::string quadratic = (std::ostringstream() << "pointLights[" << pointLightCounter << "].quadratic").str();

			std::string intensity = (std::ostringstream() << "pointLights[" << pointLightCounter << "].intensity").str();

			shader->SetUniformColor(ambiant, pointLight->m_ambiantColor);
			shader->SetUniformColor(diffuse, pointLight->m_diffuseColor);
			shader->SetUniformColor(specular, pointLight->m_specularColor);
			shader->SetUniformVector3D(position, pointLight->GetWorldPosition());
			shader->SetUniformColor(ambiant, pointLight->m_ambiantColor);
			shader->SetUniformFloat(constant, pointLight->m_constantValue);
			shader->SetUniformFloat(linear, pointLight->m_linearValue);
			shader->SetUniformFloat(quadratic, pointLight->m_quadraticValue);
			shader->SetUniformFloat(intensity, pointLight->m_intensity);
			pointLightCounter++;
		}
		else if (light->m_lightType == LightComponent::LightType::Spot)
		{
			SpotLightComponent* spotLight = dynamic_cast<SpotLightComponent*>(light);
			std::string ambiant = (std::ostringstream() << "spotLights[" << spotLightCounter << "].ambientColor").str();
			std::string diffuse = (std::ostringstream() << "spotLights[" << spotLightCounter << "].diffuseColor").str();
			std::string specular = (std::ostringstream() << "spotLights[" << spotLightCounter << "].specularColor").str();
			std::string position = (std::ostringstream() << "spotLights[" << spotLightCounter << "].position").str();
			std::string direction = (std::ostringstream() << "spotLights[" << spotLightCounter << "].direction").str();
			
			std::string spotCosAngleName = (std::ostringstream() << "spotLights[" << spotLightCounter << "].spotCosAngle").str();
			std::string spotCosSmoothAngleName = (std::ostringstream() << "spotLights[" << spotLightCounter << "].spotCosSmoothAngle").str();

			std::string constant = (std::ostringstream() << "spotLights[" << spotLightCounter << "].constant").str();
			std::string linear = (std::ostringstream() << "spotLights[" << spotLightCounter << "].linear").str();
			std::string quadratic = (std::ostringstream() << "spotLights[" << spotLightCounter << "].quadratic").str();
			std::string intensity = (std::ostringstream() << "spotLights[" << spotLightCounter << "].intensity").str();

			shader->SetUniformColor(ambiant, spotLight->m_ambiantColor);
			shader->SetUniformColor(diffuse, spotLight->m_diffuseColor);
			shader->SetUniformColor(specular, spotLight->m_specularColor);
			shader->SetUniformVector3D(position, spotLight->GetWorldPosition());
			shader->SetUniformVector3D(direction, spotLight->GetForwardVector());

			float spotCosAngle = cosf(spotLight->m_spotAngle * M_PI/180.0f);
			float spotCosSmoothValue = cosf((spotLight->m_spotAngle - spotLight->m_spotAngle * spotLight->m_spotSmoothValue) * M_PI/180.0f);
			shader->SetUniformFloat(spotCosAngleName, spotCosAngle);
			shader->SetUniformFloat(spotCosSmoothAngleName, spotCosSmoothValue);	

			shader->SetUniformFloat(constant, spotLight->m_constantValue);
			shader->SetUniformFloat(linear, spotLight->m_linearValue);
			shader->SetUniformFloat(quadratic, spotLight->m_quadraticValue);
			shader->SetUniformFloat(intensity, spotLight->m_intensity);

			spotLightCounter++;
		}
	}
	shader->SetUniformInt("pointLightCount", pointLightCounter);
	shader->SetUniformInt("spotLightCount", spotLightCounter);
	shader->SetUniformInt("directionalLightCount", directionalLightCounter);
	shader->SetUniformColor("globalAmbiantColor", LightingSettings::m_globalAmbiantColor);

	
	m_mesh->UseMesh();
	

	if(m_drawPartialMesh)
		glDrawElements((int)m_mesh->m_shapeType, m_partialMeshElementCount, GL_UNSIGNED_INT, (void*)(m_partialMeshStartIndex * sizeof(unsigned int)));
	else
	{
		glDrawElements((int)m_mesh->m_shapeType, m_mesh->GetIndices().size(), GL_UNSIGNED_INT, 0);
	}
}

void MeshRendererComponent::SetDrawPartialMesh(bool drawPartialMesh)
{
	m_drawPartialMesh = drawPartialMesh;
}
void MeshRendererComponent::SetPartialMeshElementCount(int partialMeshElementCount)
{
	m_partialMeshElementCount = partialMeshElementCount;
}

void MeshRendererComponent::SetPartialMeshStartIndex(int partialMeshStartIndex)
{
	m_partialMeshStartIndex = partialMeshStartIndex;
}

void MeshRendererComponent::SetMesh(Mesh* mesh)
{
	m_mesh = mesh;
}

void MeshRendererComponent::SetShader(Shader* shader)
{
	m_shader = shader;
}

void MeshRendererComponent::SetMaterial(Material* material)
{
	m_material = material;
}

void MeshRendererComponent::SetPolygonMode(PolygonMode polygonMode)
{
	m_polygonMode = polygonMode;
}

void MeshRendererComponent::SetGeometryRenderingPriority(int value)
{
	m_geometryRenderingPriority = value;

	World::Instance()->RefreshPriorityRenderingComponent(this);
}

bool MeshRendererComponent::GetDrawPartialMesh()const
{
	return m_drawPartialMesh;
}

int MeshRendererComponent::GetPartialMeshElementCount()const
{
	return m_partialMeshElementCount;
}

int MeshRendererComponent::GetPartialMeshStartIndex()const
{
	return m_partialMeshStartIndex;
}

int MeshRendererComponent::GetGeometryRenderingPriority() const
{
	return m_geometryRenderingPriority;
}

Mesh* MeshRendererComponent::GetMesh()const
{
	return m_mesh;
}

Shader* MeshRendererComponent::GetShader()const
{
	return m_shader;
}

PolygonMode MeshRendererComponent::GetPolygonMode()const
{
	return m_polygonMode;
}

Component* MeshRendererComponent::Clone()
{
	MeshRendererComponent* meshRendererComponent = new MeshRendererComponent(*this);
	clonnedObject = meshRendererComponent;
	clonnedObject->baseObject = this;
	return meshRendererComponent;
}

void MeshRendererComponent::AssignPointerAndReference()
{
	SceneComponent::AssignPointerAndReference();
}

void MeshRendererComponent::EnableDepthMask(bool value)
{
	m_isDepthMaskEnable = value;
}

void MeshRendererComponent::EnableDepthTest(bool value)
{
	m_isDepthTestEnable = value;
}

void MeshRendererComponent::SetDepthTestFunc(DepthTestFunc depthTestFunc)
{
	m_depthTestFunc = depthTestFunc;
}

bool MeshRendererComponent::IsDepthMaskEnable()
{
	return m_isDepthMaskEnable;
}

bool MeshRendererComponent::IsDepthTestEnable()
{
	return m_isDepthTestEnable;
}

DepthTestFunc MeshRendererComponent::GetDepthTestFunc()
{
	return m_depthTestFunc;
}

void MeshRendererComponent::SetStencilBackMask(int mask)
{
	m_stencilBackMask = mask;
}

void MeshRendererComponent::SetStencilFrontMask(int mask)
{
	m_stencilFrontMask = mask;
}

void MeshRendererComponent::SetStencilTestFrontFunc(StencilTestFunc stencilTestFunc)
{
	m_stencilTestFrontFunc = stencilTestFunc;
}

void MeshRendererComponent::SetStencilTestBackFunc(StencilTestFunc stencilTestFunc)
{
	m_stencilTestBackFunc = stencilTestFunc;
}

void MeshRendererComponent::SetStencilTestFrontRef(int stencilTestRef)
{
	m_stencilTestFrontRef = stencilTestRef;
}

void MeshRendererComponent::SetStencilTestBackRef(int stencilTestRef)
{
	m_stencilTestBackRef = stencilTestRef;
}

void MeshRendererComponent::SetStencilTestFrontMask(int stencilTestMask)
{
	m_stencilTestFrontMask = stencilTestMask;
}

void MeshRendererComponent::SetStencilFailBackAction(StencilTestAction stencilTestAction)
{
	m_stencilFailBackAction = stencilTestAction;
}

void MeshRendererComponent::SetDepthFailBackAction(StencilTestAction stencilTestAction)
{
	m_depthFailBackAction = stencilTestAction;
}

void MeshRendererComponent::SetDepthPassBackAction(StencilTestAction stencilTestAction)
{
	m_depthPassBackAction = stencilTestAction;
}

void MeshRendererComponent::SetStencilFailFrontAction(StencilTestAction stencilTestAction)
{
	m_stencilFailFrontAction = stencilTestAction;
}

void MeshRendererComponent::SetDepthFailFrontAction(StencilTestAction stencilTestAction)
{
	m_depthFailFrontAction = stencilTestAction;
}

void MeshRendererComponent::SetDepthPassFrontAction(StencilTestAction stencilTestAction)
{
	m_depthPassFrontAction = stencilTestAction;
}

void MeshRendererComponent::EnableCullFace(bool value)
{
	m_isCullFaceEnable = value;
}

void MeshRendererComponent::SetCullFace(CullFace cullFace)
{
	m_cullFace = cullFace;
}

void MeshRendererComponent::SetFrontFace(FrontFace frontFace)
{
	m_frontFace = frontFace;
}

void MeshRendererComponent::SetStencilTestBackMask(int stencilTestMask)
{
	m_stencilTestBackMask = stencilTestMask;
}

void MeshRendererComponent::EnableStencilTest(bool value)
{
	m_isStencilTestEnable = value;
}

void MeshRendererComponent::EnableBlend(bool value)
{
	if(value != m_isBlendEnable)
		World::Instance()->RefreshBlendRenderingComponent(this, value);

	m_isBlendEnable = value;
}

void MeshRendererComponent::SetRGBBlendingMode(BlendingMode mode) 
{
	m_RGBBlendingMode = mode;
}

void MeshRendererComponent::SetAlphaBlendingMode(BlendingMode mode) 
{
	m_AlphaBlendingMode = mode;
}

void MeshRendererComponent::SetSourceRGBBlendingFactor(BlendingFactor factor) 
{
	m_sourceRGBBlendingFactor = factor;
}

void MeshRendererComponent::SetDestinationRGBBlendingFactor(BlendingFactor factor) 
{
	m_destinationRGBBlendingFactor = factor;
}

void MeshRendererComponent::SetSourceAlphaBlendingFactor(BlendingFactor factor) 
{
	m_sourceAlphaBlendingFactor = factor;
}

void MeshRendererComponent::SetDestinationAlphaBlendingFactor(BlendingFactor factor) 
{
	m_destinationAlphaBlendingFactor = factor;
}

void MeshRendererComponent::SetBlendColor(const Color& color) 
{
	m_blendColor = color;
}

void MeshRendererComponent::EnableRedMask(bool value)
{
	m_isRedMaskEnable = value;
}

void MeshRendererComponent::EnableGreenMask(bool value)
{
	m_isGreenMaskEnable = value;
}

void MeshRendererComponent::EnableBlueMask(bool value)
{
	m_isBlueMaskEnable = value;
}

void MeshRendererComponent::EnableAlphaMask(bool value)
{
	m_isAlphaMaskEnable = value;
}


bool MeshRendererComponent::IsStencilTestEnable()
{
	return m_isStencilTestEnable;
}

int MeshRendererComponent::GetStencilBackMask()
{
	return m_stencilBackMask;
}

int MeshRendererComponent::GetStencilFrontMask()
{
	return m_stencilFrontMask;
}

StencilTestFunc MeshRendererComponent::GetStencilTestFrontFunc()
{
	return m_stencilTestFrontFunc;
}
StencilTestFunc MeshRendererComponent::GetStencilTestBackFunc()
{
	return m_stencilTestBackFunc;
}

int MeshRendererComponent::GetStencilTestFrontRef()
{
	return m_stencilTestFrontRef;
}

int MeshRendererComponent::GetStencilTestBackRef()
{
	return m_stencilTestBackRef;
}

int MeshRendererComponent::GetStencilTestFrontMask()
{
	return m_stencilTestFrontMask;
}

StencilTestAction MeshRendererComponent::GetStencilFailBackAction()
{
	return m_stencilFailBackAction;
}

StencilTestAction MeshRendererComponent::GetDepthFailBackAction()
{
	return m_depthFailBackAction;
}

StencilTestAction MeshRendererComponent::GetDepthPassBackAction()
{
	return m_depthPassBackAction;
}

StencilTestAction MeshRendererComponent::GetStencilFailFrontAction()
{
	return m_stencilFailFrontAction;
}

StencilTestAction MeshRendererComponent::GetDepthFailFrontAction()
{
	return m_depthFailFrontAction;
}

StencilTestAction MeshRendererComponent::GetDepthPassFrontAction()
{
	return m_depthPassFrontAction;
}

bool MeshRendererComponent::IsCullFaceEnable(bool value)
{
	return m_isCullFaceEnable;
}

CullFace MeshRendererComponent::GetCullFace(CullFace cullFace)
{
	return m_cullFace;
}

FrontFace MeshRendererComponent::GetFrontFace(FrontFace frontFace)
{
	return m_frontFace;
}

int MeshRendererComponent::GetStencilTestBackMask()
{
	return m_stencilTestBackMask;
}

Material* MeshRendererComponent::GetMaterial()const
{
	return m_material;
}

bool MeshRendererComponent::IsBlendEnabled() const 
{
	return m_isBlendEnable;
}

BlendingMode MeshRendererComponent::GetRGBBlendingMode() const
{
	return m_RGBBlendingMode;
}

BlendingMode MeshRendererComponent::GetAlphaBlendingMode() const 
{
	return m_AlphaBlendingMode;
}

BlendingFactor MeshRendererComponent::GetSourceRGBBlendingFactor() const 
{
	return m_sourceRGBBlendingFactor;
}

BlendingFactor MeshRendererComponent::GetDestinationRGBBlendingFactor() const 
{
	return m_destinationRGBBlendingFactor;
}

BlendingFactor MeshRendererComponent::GetSourceAlphaBlendingFactor() const 
{
	return m_sourceAlphaBlendingFactor;
}

BlendingFactor MeshRendererComponent::GetDestinationAlphaBlendingFactor() const 
{
	return m_destinationAlphaBlendingFactor;
}

Color MeshRendererComponent::GetBlendColor() const 
{
	return m_blendColor;
}

bool MeshRendererComponent::IsRedMaskEnable() const
{
	return m_isRedMaskEnable;
}

bool MeshRendererComponent::IsGreenMaskEnable() const
{
	return m_isGreenMaskEnable;
}

bool MeshRendererComponent::IsBlueMaskEnable() const
{
	return m_isBlueMaskEnable;
}

bool MeshRendererComponent::IsAlphaMaskEnable() const
{
	return m_isAlphaMaskEnable;
}


bool MeshRendererComponent::GetOverrideMultiSamplingEnable() const {
	return m_ovverideMultiSamplingEnable;
}

void MeshRendererComponent::SetOverrideMultiSamplingEnable(bool overrideMultiSamplingEnable) {
	m_ovverideMultiSamplingEnable = overrideMultiSamplingEnable;
}

bool MeshRendererComponent::GetEnableMultiSampling() const {
	return m_enableMultiSampling;
}

void MeshRendererComponent::SetEnableMultiSampling(bool enableMultiSampling) {
	m_enableMultiSampling = enableMultiSampling;
}