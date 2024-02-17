#include "AssetsManager.h"

AssetsManager* AssetsManager::m_assetsManager = nullptr;

AssetsManager::AssetsManager()
{

}

AssetsManager::~AssetsManager()
{

	for (auto it = m_assets.begin(); it != m_assets.end(); ++it)
	{
		std::pair<std::string, Assets*> assetsPair = *it;
		
		if(assetsPair.second != nullptr)
			delete assetsPair.second;
	}

	m_assets.clear();

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
	Mesh* mesh = new Mesh();
	assetsManager->m_assets.emplace(assetName,mesh);
	return mesh;
}

Texture* AssetsManager::CreateTexture(const std::string& assetName, std::string filePath)
{
	AssetsManager* assetsManager = AssetsManager::Instance();
	Texture* texture = new Texture(filePath);
	assetsManager->m_assets.emplace(assetName, texture);

	return texture;
}

Material* AssetsManager::CreateMaterial(const std::string& assetName, Texture* ambientTexture, Texture* diffuseTexture, Texture* specularTexture, Color ambientColor, Color diffuseColor, Color specularColor, float shininess)
{
	AssetsManager* assetsManager = AssetsManager::Instance();
	Material* material = new Material(ambientTexture, diffuseTexture, specularTexture, ambientColor, diffuseColor, specularColor, shininess);
	assetsManager->m_assets.emplace(assetName, material);
	return material;
}

Shader* AssetsManager::CreateShader(const std::string& assetName, std::string vertexShaderFilePath, std::string fragmentShaderFilePath)
{
	AssetsManager* assetsManager = AssetsManager::Instance();
	Shader* shader = new Shader(vertexShaderFilePath, fragmentShaderFilePath);
	assetsManager->m_assets.emplace(assetName, shader);

	return shader;
}

