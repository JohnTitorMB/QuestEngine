#ifndef _OBJLOADER_H_
#define _OBJLOADER_H_
#include <iostream>
#include <string>
#include "Assets/EntityGroupAsset.h"
#include "Assets/Mesh.h"
#include <unordered_map>
#include "Assets/Material.h"
#include <fstream>
#include <mutex>


namespace std {


    template <>
    struct hash<std::tuple<unsigned int, unsigned int, unsigned int>> {
        size_t operator()(const std::tuple<unsigned int, unsigned int, unsigned int>& t) const {
            size_t seed = 0;
            // Mélangez les hachages de chaque élément du tuple avec des valeurs de hachage différentes
            seed ^= std::hash<unsigned int>{}(std::get<0>(t)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= std::hash<unsigned int>{}(std::get<1>(t)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= std::hash<unsigned int>{}(std::get<2>(t)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
}

namespace OBJLibrary
{
    struct OBJObject;
    struct Group;
    struct GeometryData;
    struct MaterialData;
 
    struct OBJObject
    {
        std::string m_name;
        std::vector<Group> m_groups;
    };

    struct Group
    {
        Group()
        {
            m_geometryBlockStartIndex = 0;
            m_geometryBlockCount = 0;
            m_name = "";
        }
        std::string m_name;
        int m_geometryBlockStartIndex = 0;
        int m_geometryBlockCount = 0;
    };

    struct MaterialData
    {
        MaterialData()
        {
            m_materialName = "";
            m_ambientMapPath = "";
            m_diffuseMapPath = "";
            m_specularMapPath = "";
            m_ambientColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
            m_diffuseColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
            m_specularColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
            m_shininess = 32;
        }
        std::string m_materialName;
        Color m_ambientColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
        Color m_diffuseColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
        Color m_specularColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
        float m_shininess = 32;
        float m_alpha = 1.0f;

        std::string m_ambientMapPath;
        std::string m_diffuseMapPath;
        std::string m_specularMapPath;
        std::string m_alphaMapPath;
    };

    struct GeometryData
    {
        int materialID = -1;

        std::vector<Vector3D> vertices;
        std::vector<Vector2D> uvs;
        std::vector<Vector3D> normals;
        std::vector<unsigned int> indices;

    };


    class OBJLoader
    {
    private:

        static Entity* CreateEntity(std::string entityName, EntityGroupAsset* entityGroupAsset, Mesh*& mesh);
        static Entity* CreateSceneEntity(std::string entityName, EntityGroupAsset* entityGroupAsset);
        static Material* CreateMaterial(std::string assetName, MaterialData materialData);
        static inline float ReadFloatFromBuffer(const std::vector<char>& buffer, size_t& pos);
        static inline long ReadLongFromBuffer(const std::vector<char>& buffer, size_t& pos);
        static inline void ReadVector2DFromBuffer(const std::vector<char>& buffer, size_t& pos, Vector2D& result);
        static inline void ReadVector3DFromBuffer(const std::vector<char>& buffer, size_t& pos, Vector3D& result);
        static inline void ReadRGBColorFromBuffer(const std::vector<char>& buffer, size_t& pos, Color& result);
        static inline void SkipLine(const std::vector<char>& buffer, size_t& pos);
        static inline std::string ReadRemainStringFromBuffer(const std::vector<char>& buffer, size_t& pos);
        static inline std::string GetFullPath(const std::filesystem::path& path, std::string subFilePath);

    public:
        static void LoadOBJData(std::string objFilePath, std::vector<OBJObject>* objObjectCollection, std::vector<GeometryData>* geometryDataCollection, std::vector<MaterialData>* materialCollection, const float scaleFactor = 1.0f);
        static std::unordered_map<std::string, MaterialData> LoadMTLData(const std::string& mtlFilePath);

        static EntityGroupAsset* LoadOBJ(std::string assetsName, const std::string& filePath, const float scaleFactor = 1.0f);
    };
}




#endif // !_OBJLOADER_H_