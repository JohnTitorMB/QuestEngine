#ifndef  _ASSETSMANGER_H_
#define _ASSETSMANGER_H_
#include "Assets/Mesh.h"
#include "Assets/Texture.h"
#include "Assets/Material.h"
#include "Assets/Shader.h"
#include <unordered_map>

class AssetsManager
{
	static AssetsManager* m_assetsManager;
private:
	std::unordered_map<std::string, Assets*> m_assets;

protected:
	AssetsManager();
	~AssetsManager();

public:
	static AssetsManager* Instance();
	AssetsManager(AssetsManager& other) = delete;

	static Mesh* CreateMesh(const std::string& assetName, bool useOneVbo);
	static Texture* CreateTexture(const std::string& assetName, std::string filePath);
	static Material* CreateMaterial(const std::string& assetName, Texture* ambientTexture, Texture* diffuseTexture, Texture* specularTexture, Color ambientColor, Color diffuseColor, Color specularColor, float shininess);
	static Shader* CreateShader(const std::string& assetName, std::string vertexShaderFilePath, std::string fragmentShaderFilePath);


	template<class T = Assets>
	static T* GetAsset(const std::string& assetName)
	{
		AssetsManager* assetsManager = AssetsManager::Instance();

		auto it = assetsManager->m_assets.find(assetName);
		if (it != assetsManager->m_assets.end()) {
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
};
#endif

