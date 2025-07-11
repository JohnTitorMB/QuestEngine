#include "AssetsManager.h"
#include <iostream>
#include <sstream>

AssetsManager* AssetsManager::m_assetsManager = nullptr;

AssetsManager::AssetsManager()
{

}

AssetsManager::~AssetsManager()
{

	for (auto it = m_assets.begin(); it != m_assets.end(); ++it)
	{
		Assets* asset = *it;
		if(asset != nullptr)
			delete asset;
	}

	m_assets.clear();
	m_assetsFilePathMap.clear();
	m_assetsNameMap.clear();
}

AssetsManager* AssetsManager::Instance()
{
	if (m_assetsManager == nullptr)
		m_assetsManager = new AssetsManager();

	return m_assetsManager;
}

Mesh* AssetsManager::CreateMesh(const std::string& assetName, bool useOneVbo)
{
	AssetsManager* assetsManager = AssetsManager::Instance();
	std::string newAssetName = assetName;

	if (assetsManager->m_assetsNameMap.find(assetName) != assetsManager->m_assetsNameMap.end())
		newAssetName = assetsManager->GenerateUniqueAssetName(assetName);

	Mesh* mesh = new Mesh();
	assetsManager->m_assetsNameMap.emplace(newAssetName, mesh);
	assetsManager->m_assets.insert(mesh);
	assetsManager->m_assetsByType[typeid(*mesh)].push_back(mesh);
	mesh->m_name = newAssetName;
	return mesh;
}

std::string AssetsManager::GenerateUniqueAssetName(const std::string& baseName) {
	std::string uniqueName = baseName;
	int counter = 1;

	while (m_assetsNameMap.find(uniqueName) != m_assetsNameMap.end()) {
		std::stringstream ss;
		ss << baseName << "_" << counter;
		uniqueName = ss.str();
		++counter;
	}

	return uniqueName;
}

void AssetsManager::Destroy()
{
	delete m_assetsManager;
	m_assetsManager = nullptr;
}

Texture2D* AssetsManager::CreateTexture2D(const std::string& assetName, std::string filePath, bool verifyFilePathLoaded)
{
	std::filesystem::path p(filePath);

	if (p.empty() || !std::filesystem::exists(filePath))
	{
		std::cout << "the asset " << assetName << "could not be created!the following path" << filePath  << "does not exist" << std::endl;
		return nullptr;
	}

	AssetsManager* assetsManager = AssetsManager::Instance();

	if (verifyFilePathLoaded && assetsManager->m_assetsFilePathMap.find(filePath) != assetsManager->m_assetsFilePathMap.end())
		return static_cast<Texture2D*>(assetsManager->m_assetsFilePathMap[filePath]);

	std::string newAssetName = assetName;

	if (assetsManager->m_assetsNameMap.find(assetName) != assetsManager->m_assetsNameMap.end())
		newAssetName = assetsManager->GenerateUniqueAssetName(assetName);

	Texture2D* texture = new Texture2D(filePath);
	texture->m_name = newAssetName;
	assetsManager->m_assetsNameMap.emplace(newAssetName, texture);
	assetsManager->m_assetsFilePathMap.emplace(filePath, texture);
	assetsManager->m_assets.insert(texture);
	assetsManager->m_assetsByType[typeid(*texture)].push_back(texture);
	return texture;
}

RenderTexture2D* AssetsManager::CreateRenderTexture2D(const std::string& assetName, const int width, const int height)
{
	AssetsManager* assetsManager = AssetsManager::Instance();
	std::string newAssetName = assetName;

	if (assetsManager->m_assetsNameMap.find(assetName) != assetsManager->m_assetsNameMap.end())
		newAssetName = assetsManager->GenerateUniqueAssetName(assetName);

	RenderTexture2D* renderTexture = new RenderTexture2D(width, height);
	renderTexture->m_name = newAssetName;
	assetsManager->m_assetsNameMap.emplace(newAssetName, renderTexture);
	assetsManager->m_assets.insert(renderTexture);
	assetsManager->m_assetsByType[typeid(*renderTexture)].push_back(renderTexture);
	return renderTexture;
}

RenderCubeMap* AssetsManager::CreateRenderCubeMap(const std::string& assetName, const int width, const int height)
{
	AssetsManager* assetsManager = AssetsManager::Instance();
	std::string newAssetName = assetName;

	if (assetsManager->m_assetsNameMap.find(assetName) != assetsManager->m_assetsNameMap.end())
		newAssetName = assetsManager->GenerateUniqueAssetName(assetName);

	RenderCubeMap* renderCubemap = new RenderCubeMap(width, height);
	renderCubemap->m_name = newAssetName;
	assetsManager->m_assetsNameMap.emplace(newAssetName, renderCubemap);
	assetsManager->m_assets.insert(renderCubemap);
	assetsManager->m_assetsByType[typeid(*renderCubemap)].push_back(renderCubemap);
	return renderCubemap;
}

CubeMap* AssetsManager::CreateCubeMap(const std::string& assetName, std::string filePath, bool verifyFilePathLoaded)
{
	std::filesystem::path p(filePath);

	if (p.empty() || !std::filesystem::exists(filePath))
	{
		std::cout << "the asset " << assetName << "could not be created!the following path" << filePath << "does not exist" << std::endl;
		return nullptr;
	}

	AssetsManager* assetsManager = AssetsManager::Instance();

	if (verifyFilePathLoaded && assetsManager->m_assetsFilePathMap.find(filePath) != assetsManager->m_assetsFilePathMap.end())
		return static_cast<CubeMap*>(assetsManager->m_assetsFilePathMap[filePath]);

	std::string newAssetName = assetName;

	if (assetsManager->m_assetsNameMap.find(assetName) != assetsManager->m_assetsNameMap.end())
		newAssetName = assetsManager->GenerateUniqueAssetName(assetName);

	CubeMap* cubeMap = new CubeMap(filePath);
	cubeMap->m_name = newAssetName;
	assetsManager->m_assetsNameMap.emplace(newAssetName, cubeMap);
	assetsManager->m_assetsFilePathMap.emplace(filePath, cubeMap);
	assetsManager->m_assets.insert(cubeMap);
	assetsManager->m_assetsByType[typeid(*cubeMap)].push_back(cubeMap);
	return cubeMap;
}

Material* AssetsManager::CreateBlinnPhongMaterial(const std::string& assetName, Texture* ambientTexture, Texture* diffuseTexture, Texture* specularTexture, Texture* emissiveTexture, ColorRGB ambientColor, ColorRGB diffuseColor, ColorRGB specularColor, ColorRGB emissiveColor, float shininess)
{
	AssetsManager* assetsManager = AssetsManager::Instance();
	std::string newAssetName = assetName;
	if (assetsManager->m_assetsNameMap.find(assetName) != assetsManager->m_assetsNameMap.end())
		newAssetName = assetsManager->GenerateUniqueAssetName(assetName);

	Material* material = new Material();
	material->m_name = newAssetName;

	material->SetColor("material.ambientColor", ambientColor);
	material->SetColor("material.diffuseColor", diffuseColor);
	material->SetColor("material.specularColor", specularColor);
	material->SetColor("material.emissiveColor", emissiveColor);
	material->SetFloat("material.shininess", shininess);

	material->SetTexture("material.ambiantTexture", ambientTexture);
	material->SetTexture("material.diffuseTexture", diffuseTexture);
	material->SetTexture("material.specularTexture", specularTexture);
	material->SetTexture("material.emissiveTexture", emissiveTexture);

	material->SetVector4D("material.diffuseTextureST", Vector4D(0, 0, 1, 1));
	material->SetVector4D("material.ambiantTextureST", Vector4D(0, 0, 1, 1));
	material->SetVector4D("material.specularTextureST", Vector4D(0, 0, 1, 1));
	material->SetVector4D("material.emissiveTextureST", Vector4D(0, 0, 1, 1));

	Texture* defaultTexture = AssetsManager::GetAsset<Texture>("White");

	material->SetTexture("material.alphaTexture", defaultTexture);
	material->SetVector4D("material.alphaTextureST", Vector4D(0, 0, 1, 1));

	material->SetFloat("material.alpha", 1.0f);

	assetsManager->m_assetsNameMap.emplace(newAssetName, material);
	assetsManager->m_assets.insert(material);
	assetsManager->m_assetsByType[typeid(*material)].push_back(material);
	return material;
}

Material* AssetsManager::CreateMaterial(const std::string& assetName)
{
	AssetsManager* assetsManager = AssetsManager::Instance();
	std::string newAssetName = assetName;
	if (assetsManager->m_assetsNameMap.find(assetName) != assetsManager->m_assetsNameMap.end())
		newAssetName = assetsManager->GenerateUniqueAssetName(assetName);

	Material* material = new Material();
	material->m_name = newAssetName;
	assetsManager->m_assetsNameMap.emplace(newAssetName, material);
	assetsManager->m_assets.insert(material);
	assetsManager->m_assetsByType[typeid(*material)].push_back(material);
	return material;
}

Shader* AssetsManager::CreateShader(const std::string& assetName, std::string vertexShaderFilePath, std::string fragmentShaderFilePath)
{
	AssetsManager* assetsManager = AssetsManager::Instance();
	std::string newAssetName = assetName;
	if (assetsManager->m_assetsNameMap.find(assetName) != assetsManager->m_assetsNameMap.end())
		newAssetName = assetsManager->GenerateUniqueAssetName(assetName);

	Shader* shader = new Shader(vertexShaderFilePath, fragmentShaderFilePath);
	shader->m_name = newAssetName;
	assetsManager->m_assetsNameMap.emplace(newAssetName, shader);
	assetsManager->m_assets.insert(shader);
	assetsManager->m_assetsByType[typeid(*shader)].push_back(shader);
	return shader;
}

EntityGroupAsset* AssetsManager::CreateEntityGroup(const std::string& assetName)
{
	AssetsManager* assetsManager = AssetsManager::Instance();
	std::string newAssetName = assetName;
	if (assetsManager->m_assetsNameMap.find(assetName) != assetsManager->m_assetsNameMap.end())
		newAssetName = assetsManager->GenerateUniqueAssetName(assetName);

	EntityGroupAsset* entityGroupAsset = new EntityGroupAsset();
	entityGroupAsset->m_name = newAssetName;
	assetsManager->m_assetsNameMap.emplace(newAssetName, entityGroupAsset);
	assetsManager->m_assets.insert(entityGroupAsset);
	assetsManager->m_assetsByType[typeid(*entityGroupAsset)].push_back(entityGroupAsset);
	return entityGroupAsset;
}

