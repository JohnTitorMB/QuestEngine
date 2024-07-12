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

MeshRendererComponent::MeshRendererComponent(const MeshRendererComponent& other) : SceneComponent(other)
{
	m_polygonMode = other.m_polygonMode;
	m_shader = other.m_shader;
	m_mesh = other.m_mesh;
	m_drawPartialMesh = other.m_drawPartialMesh;
	m_partialMeshElementCount = other.m_partialMeshElementCount;
	m_partialMeshStartIndex = other.m_partialMeshStartIndex;
	m_material = other.m_material;
}

void MeshRendererComponent::SendMaterialToShader()const
{
	const std::unordered_map<std::string, int> integerKeyValue = m_material->GetIntMap();
	const std::unordered_map<std::string, float> floatKeyValue = m_material->GetFloatMap();
	const std::unordered_map<std::string, Vector2D> vector2DKeyValue = m_material->GetVector2DMap();
	const std::unordered_map<std::string, Vector3D> vector3DKeyValue = m_material->GetVector3DMap();
	const std::unordered_map<std::string, Vector4D> vector4DKeyValue = m_material->GetVector4DMap();
	const std::unordered_map<std::string, Texture*> textureKeyValue = m_material->GetTextureMap();

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
			pair.second->Bind(textureIndex);
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

	glPolygonMode(GL_FRONT_AND_BACK, (int)m_polygonMode);
	if(camera->GetUseDepthZeroToOneProjection())
		glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
	else
		glClipControl(GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE);

	if (camera->GetUseReversedZProjection())
	{
		glDepthFunc(GL_GREATER);
		glClearDepth(0.0f);
	}
	else
	{
		glDepthFunc(GL_LESS);
		glClearDepth(1.0f);
	}
	
	shader->UseShader();
	

	shader->SetUniformFloat("viewNear", 0.01f);
	shader->SetUniformFloat("viewFar", camera->GetFar());
	shader->SetUniformVector3D("viewDirection", camera->GetForwardVector());

	Matrix4x4 modelMatrix = GetTransform().TransformMatrix();
	shader->SetUniformMatrix4x4("model", modelMatrix);
	shader->SetUniformMatrix4x4("view", camera->ViewMatrix());
	shader->SetUniformMatrix4x4("projection", camera->ProjectionMatrix(window->GetWidth(), window->GetHeight()));

	Matrix3x3 normalMatrix = (Matrix3x3)(modelMatrix).Inverse().Transpose();
	shader->SetUniformMatrix3x3("normalMatrix", normalMatrix);
	shader->SetUniformVector3D("uViewPos", camera->GetTransform().GetPosition());


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

Material* MeshRendererComponent::GetMaterial()const
{
	return m_material;
}