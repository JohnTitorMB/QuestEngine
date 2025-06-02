#ifndef  _ASSETSMANGER_H_
#define _ASSETSMANGER_H_
#include "Assets/Mesh.h"
#include "Assets/Texture2D.h"
#include "Assets/CubeMap.h"
#include "Assets/Material.h"
#include "Assets/Shader.h"
#include "Assets/EntityGroupAsset.h"
#include <unordered_map>
#include <filesystem>
#include "Assets/RenderTexture2D.h"
#include <typeindex>

class QuestEngine;
class AssetsManager
{
	static AssetsManager* m_assetsManager;
	friend class QuestEngine;
private:
	std::unordered_map<std::string, Assets*> m_assetsNameMap;
	std::unordered_map<std::string, Assets*> m_assetsFilePathMap;
	std::unordered_map<std::type_index, std::vector<Assets*>> m_assetsByType;
	std::set<Assets*> m_assets;
	std::string GenerateUniqueAssetName(const std::string& baseName);
	static void Destroy();
protected:
	AssetsManager();
	~AssetsManager();

public:
	static AssetsManager* Instance();
	AssetsManager(AssetsManager& other) = delete;

	static Mesh* CreateMesh(const std::string& assetName, bool useOneVbo);
	static Texture2D* CreateTexture2D(const std::string& assetName, std::string filePath, bool verifyFilePathLoaded = false);
	static RenderTexture2D* CreateRenderTexture2D(const std::string& assetName, const int width, const int height);
	static CubeMap* CreateCubeMap(const std::string& assetName, std::string filePath, bool verifyFilePathLoaded = false);
	static Material* CreateBlinnPhongMaterial(const std::string& assetName, Texture* ambientTexture, Texture* diffuseTexture, Texture* specularTexture, Texture* emissiveTexture, ColorRGB ambientColor, ColorRGB diffuseColor, ColorRGB specularColor, ColorRGB emissiveColor, float shininess);
	static Material* CreateMaterial(const std::string& assetName);
	static Shader* CreateShader(const std::string& assetName, std::string vertexShaderFilePath, std::string fragmentShaderFilePath);
	static EntityGroupAsset* CreateEntityGroup(const std::string& assetName);

	template<class T = Assets>
	static T* GetAsset(const std::string& assetName)
	{
		AssetsManager* assetsManager = AssetsManager::Instance();

		auto it = assetsManager->m_assetsNameMap.find(assetName);
		if (it != assetsManager->m_assetsNameMap.end()) {
			Assets* asset = it->second;
			if (std::is_base_of<Assets,T>::value) {
				return static_cast<T*>(asset);
			}
			else {
				return nullptr;
			}
		}
		else {
			return nullptr;
		}
	}

	template<class T>
	static std::vector<T*> GetsAssets()
	{
		std::vector<T*> result;

		AssetsManager* assetsManager = AssetsManager::Instance();
		auto it = assetsManager->m_assetsByType.find(typeid(T));
		if (it != assetsManager->m_assetsByType.end()) {
			for (Assets* asset : it->second) {
				if (T* casted = dynamic_cast<T*>(asset))
					result.push_back(casted);
			}
		}

		return result;
	}
};
#endif

