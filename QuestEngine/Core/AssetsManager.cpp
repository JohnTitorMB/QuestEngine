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

	if (m_assetsManager)
	{
		delete m_assetsManager;
		m_assetsManager = nullptr;
	}
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

Texture* AssetsManager::CreateTexture(const std::string& assetName, std::string filePath, bool verifyFilePathLoaded)
{
	std::filesystem::path p(filePath);

	if (p.empty() || !std::filesystem::exists(filePath))
	{
		std::cout << "the asset " << assetName << "could not be created!the following path" << filePath  << "does not exist" << std::endl;
		return nullptr;
	}

	AssetsManager* assetsManager = AssetsManager::Instance();

	if (verifyFilePathLoaded && assetsManager->m_assetsFilePathMap.find(filePath) != assetsManager->m_assetsFilePathMap.end())
		return static_cast<Texture*>(assetsManager->m_assetsFilePathMap[filePath]);

	std::string newAssetName = assetName;

	if (assetsManager->m_assetsNameMap.find(assetName) != assetsManager->m_assetsNameMap.end())
		newAssetName = assetsManager->GenerateUniqueAssetName(assetName);

	Texture* texture = new Texture(filePath);
	assetsManager->m_assetsNameMap.emplace(newAssetName, texture);
	assetsManager->m_assetsFilePathMap.emplace(filePath, texture);
	assetsManager->m_assets.insert(texture);
	return texture;
}

Material* AssetsManager::CreateMaterial(const std::string& assetName, Texture* ambientTexture, Texture* diffuseTexture, Texture* specularTexture, Color ambientColor, Color diffuseColor, Color specularColor, float shininess)
{
	AssetsManager* assetsManager = AssetsManager::Instance();
	std::string newAssetName = assetName;
	if (assetsManager->m_assetsNameMap.find(assetName) != assetsManager->m_assetsNameMap.end())
		newAssetName = assetsManager->GenerateUniqueAssetName(assetName);

	Material* material = new Material(ambientTexture, diffuseTexture, specularTexture, ambientColor, diffuseColor, specularColor, shininess);
	assetsManager->m_assetsNameMap.emplace(newAssetName, material);
	assetsManager->m_assets.insert(material);
	return material;
}

Shader* AssetsManager::CreateShader(const std::string& assetName, std::string vertexShaderFilePath, std::string fragmentShaderFilePath)
{
	AssetsManager* assetsManager = AssetsManager::Instance();
	std::string newAssetName = assetName;
	if (assetsManager->m_assetsNameMap.find(assetName) != assetsManager->m_assetsNameMap.end())
		newAssetName = assetsManager->GenerateUniqueAssetName(assetName);

	Shader* shader = new Shader(vertexShaderFilePath, fragmentShaderFilePath);
	assetsManager->m_assetsNameMap.emplace(newAssetName, shader);
	assetsManager->m_assets.insert(shader);
	return shader;
}

EntityGroupAsset* AssetsManager::CreateEntityGroup(const std::string& assetName)
{
	AssetsManager* assetsManager = AssetsManager::Instance();
	std::string newAssetName = assetName;
	if (assetsManager->m_assetsNameMap.find(assetName) != assetsManager->m_assetsNameMap.end())
		newAssetName = assetsManager->GenerateUniqueAssetName(assetName);

	EntityGroupAsset* entityGroupAsset = new EntityGroupAsset();
	assetsManager->m_assetsNameMap.emplace(newAssetName, entityGroupAsset);
	assetsManager->m_assets.insert(entityGroupAsset);
	return entityGroupAsset;
}

