#include "OBJLoader.h"
#include <string>
#include "AssetsManager.h"
#include <filesystem>
#include <sstream>
#include <iostream>
#include "Components/MeshRenderer.h"
#include "World.h"
#include <unordered_map>
#include "../Utilities/GeometryUtilities.h"
#include "Components/SceneComponent.h"
#include <queue>
#include <thread>
#include <functional>

using namespace OBJLibrary;

Entity* OBJLoader::CreateEntity(std::string entityName, EntityGroupAsset* entityGroupAsset, Mesh*& mesh)
{
	Entity* entity = entityGroupAsset->CreateEntity<Entity>();
	entity->SetName(entityName);
	mesh = AssetsManager::CreateMesh(entityName + "Mesh", true);

	MeshRendererComponent* meshComponent = entity->AddComponent<MeshRendererComponent>(true);
	meshComponent->SetMesh(mesh);
	meshComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
	meshComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(3)));
	entity->SetRootComponent(meshComponent);
	return entity;
}

Entity* OBJLoader::CreateSceneEntity(std::string entityName, EntityGroupAsset* entityGroupAsset)
{
	Entity* entity = entityGroupAsset->CreateEntity<Entity>();
	entity->SetName(entityName);

	SceneComponent* sceneComponent = entity->AddComponent<SceneComponent>(true);
	entity->SetRootComponent(sceneComponent);
	return entity;
}

Material* OBJLibrary::OBJLoader::CreateMaterial(std::string assetName,MaterialData materialData)
{
	Texture* defaultTexture = AssetsManager::GetAsset<Texture>("White");
	Color whiteColor = Color(1.0f,1.0f,1.0f,1.0f);
	Material* material = AssetsManager::CreateMaterial(assetName, defaultTexture, defaultTexture, defaultTexture, whiteColor, whiteColor, whiteColor, 32);
	material->m_ambientColor = materialData.m_ambientColor;
	material->m_diffuseColor = materialData.m_diffuseColor;
	material->m_specularColor = materialData.m_specularColor;
	material->m_shininess = materialData.m_shininess;

	if (!materialData.m_ambientMapPath.empty())
	{
		Texture* ambiantTexture = AssetsManager::CreateTexture(assetName + "ambientTexture", materialData.m_ambientMapPath, true);
		if (ambiantTexture)
			material->m_ambientTexture = ambiantTexture;
	}

	if (!materialData.m_diffuseMapPath.empty())
	{
		Texture* diffuseTexture = AssetsManager::CreateTexture(assetName + "diffuseTexture", materialData.m_diffuseMapPath, true);
		if (diffuseTexture)
			material->m_diffuseTexture = diffuseTexture;
	}

	if (!materialData.m_specularMapPath.empty())
	{
		Texture* specularTexture = AssetsManager::CreateTexture(assetName + "specularTexture", materialData.m_specularMapPath, true);
		if (specularTexture)
			material->m_specularTexture = specularTexture;
	}

	return material;
}

inline float OBJLibrary::OBJLoader::ReadFloatFromBuffer(const std::vector<char>& buffer, size_t& pos)
{
	const char* start = buffer.data() + pos;

	// Parse x
	char* end;
	float x = std::strtof(start, &end);
	pos += end - start;
	return x;
}

inline long OBJLibrary::OBJLoader::ReadLongFromBuffer(const std::vector<char>& buffer, size_t& pos)
{
	const char* start = buffer.data() + pos;

	// Parse x
	char* end;
	long x = std::strtol(start, &end, 10);


	pos += end - start;
	return x;
}

inline Vector2D& OBJLibrary::OBJLoader::ReadVector2DFromBuffer(const std::vector<char>& buffer, size_t& pos)
{
	const char* start = buffer.data() + pos;

	// Parse x
	char* end;
	float x = std::strtof(start, &end);
	pos += end - start + 1;
	start = end + 1;

	// Parse y
	float y = std::strtof(start, &end);
	pos += end - start;

	Vector2D vec = Vector2D(x, y);
	return vec;
}

inline Vector3D& OBJLibrary::OBJLoader::ReadVector3DFromBuffer(const std::vector<char>& buffer, size_t& pos)
{
	const char* start = buffer.data() + pos;

	// Parse x
	char* end;
	float x = std::strtof(start, &end);
	pos += end - start + 1;

	start = end + 1;

	// Parse y
	float y = std::strtof(start, &end);
	pos += end - start + 1;
	start = end + 1;

	// Parse z
	float z = std::strtof(start, &end);
	pos += end - start;

	Vector3D vec = Vector3D(x, y, z);
	return vec;
}

inline Color& OBJLibrary::OBJLoader::ReadRGBColorFromBuffer(const std::vector<char>& buffer, size_t& pos)
{
	const char* start = buffer.data() + pos;

	// Parse r
	char* end;
	float r = std::strtof(start, &end);
	pos += end - start + 1;

	start = end + 1;

	// Parse g
	float g = std::strtof(start, &end);
	pos += end - start + 1;
	start = end + 1;

	// Parse b
	float b = std::strtof(start, &end);
	pos += end - start;

	Color color = Color(r, g, b, 1);
	return color;
}

void OBJLoader::SkipLine(const std::vector<char>& buffer, size_t& pos)
{
	while (pos < buffer.size() && buffer[pos] != '\n') {
		pos += 1;
	}
	if (pos < buffer.size() && buffer[pos] == '\n') {
		pos += 1;
	}
}

inline std::string OBJLibrary::OBJLoader::ReadRemainStringFromBuffer(const std::vector<char>& buffer, size_t& pos)
{
	std::string s = "";
	while (pos < buffer.size() && buffer[pos] != '\n' && buffer[pos] != '\r')
	{
		s += buffer[pos];
		pos++;
	}

	return s;
}

inline std::string OBJLibrary::OBJLoader::GetFullPath(const std::filesystem::path& path, std::string subFilePath)
{
	if (path.has_parent_path())
		subFilePath = path.parent_path().string() + "/" + subFilePath;

	return subFilePath;
}

void OBJLoader::LoadOBJData(std::string objFilePath, std::vector<OBJObject>* objObjectCollection, std::vector<GeometryData>* geometryDataCollection, std::vector<MaterialData>* materialCollection, const float scaleFactor)
{
	std::filesystem::path p(objFilePath);

	if (p.empty() || p.extension() != ".obj")
		return;

	std::ifstream file;
	file.open(objFilePath, std::ios::binary);

	if (!file.is_open()) {
		std::cout << "Could not open the file: " << objFilePath << std::endl;
		return;
	}

	file.seekg(0, file.end);
	std::streampos total_size = file.tellg();
	file.seekg(0);

	std::streampos start_offset = 0;
	std::streampos end_offset = total_size;
	std::vector<char> buffer(end_offset - start_offset + 1);
	file.read(buffer.data(), buffer.size());

	std::vector<Vector3D> vertices;
	std::vector<Vector2D> uvs;
	std::vector<Vector3D> normals;
	
	OBJObject currentOBJObject = OBJObject();
	currentOBJObject.m_name = "DefaultObject";

	Group currentGroup = Group();
	currentGroup.m_name = "DefaultGroup";
	currentGroup.m_geometryBlockCount++;

	currentOBJObject.m_groups.push_back(currentGroup);
	objObjectCollection->push_back(currentOBJObject);
	
	int currentMaterialID = -1;
	std::unordered_map<std::string, std::unordered_map<std::string, int>> materialsID;

	GeometryData geometryData = GeometryData();
	std::string currentMTLPath = "";
	geometryDataCollection->push_back(geometryData);

	size_t current_position = 0;
	size_t bufferSize = buffer.size();
	std::unordered_map<std::tuple<unsigned int, unsigned int, unsigned int>, unsigned int> tempIndicesCollection;

	while (current_position < bufferSize)
	{
		//Read UV
		if (buffer[current_position] == 'v' && buffer[current_position + 1] == 't')
		{
			current_position += 3;
			Vector2D uv = ReadVector2DFromBuffer(buffer, current_position);
			uvs.emplace_back(uv);

 			SkipLine(buffer, current_position);
		}
		// Read Normal
		else if (buffer[current_position] == 'v' && buffer[current_position + 1] == 'n')
		{
			current_position += 3;
			Vector3D normal = ReadVector3DFromBuffer(buffer, current_position);
			normals.emplace_back(normal);

			SkipLine(buffer, current_position);

		}
		// Read Vertex
		else if (buffer[current_position] == 'v')
		{
			current_position += 2;
			Vector3D vertex = ReadVector3DFromBuffer(buffer, current_position) * scaleFactor;
			vertices.emplace_back(vertex);
			SkipLine(buffer, current_position);
		}
		else if (buffer[current_position] == 'o')
		{
			current_position += 2;
			std::string objectName = ReadRemainStringFromBuffer(buffer, current_position);
			
			//verify is last geometry data is empty
			if (geometryDataCollection->back().indices.size() == 0)
				objObjectCollection->back().m_groups.back().m_geometryBlockCount--;
			else
			{
				GeometryData geometryData = GeometryData();
				geometryDataCollection->push_back(geometryData);
			}

			if (objObjectCollection->back().m_groups.back().m_geometryBlockCount == 0)
				objObjectCollection->back().m_groups.pop_back();

			if (objObjectCollection->back().m_groups.size() != 0)
				objObjectCollection->push_back(OBJObject());

			objObjectCollection->back().m_name = objectName;

			Group newGroup = Group();
			newGroup.m_geometryBlockCount++;
			newGroup.m_geometryBlockStartIndex = geometryDataCollection->size()-1;
			objObjectCollection->back().m_groups.push_back(newGroup);

			tempIndicesCollection.clear();
			
			SkipLine(buffer, current_position);

		}
		else if (buffer[current_position] == 'g')
		{
			current_position += 2;
			std::string groupName = ReadRemainStringFromBuffer(buffer, current_position);
			

			//verify is last geometry data is empty
			if (geometryDataCollection->back().indices.size() == 0)
				objObjectCollection->back().m_groups.back().m_geometryBlockCount--;
			else
			{
				GeometryData geometryData = GeometryData();
				geometryDataCollection->push_back(geometryData);
			}

			if (objObjectCollection->back().m_groups.back().m_geometryBlockCount != 0)
				objObjectCollection->back().m_groups.push_back(Group());
			
			objObjectCollection->back().m_groups.back().m_geometryBlockStartIndex = geometryDataCollection->size() - 1;
			objObjectCollection->back().m_groups.back().m_geometryBlockCount++;
			objObjectCollection->back().m_groups.back().m_name = groupName;
			tempIndicesCollection.clear();

			SkipLine(buffer, current_position);

		}
		else if (current_position + 5 < bufferSize && buffer[current_position] == 'm' &&
			buffer[current_position + 1] == 't' &&
			buffer[current_position + 2] == 'l' &&
			buffer[current_position + 3] == 'l' &&
			buffer[current_position + 4] == 'i' &&
			buffer[current_position + 5] == 'b')
		{
			current_position += 7;
			std::string mtlPath = ""; 
			mtlPath = ReadRemainStringFromBuffer(buffer, current_position);
				if (materialsID.find(mtlPath) == materialsID.end())
			{
				std::unordered_map<std::string, MaterialData> mtlData = LoadMTLData(GetFullPath(p, mtlPath));
				for (auto dataPair : mtlData)
				{
					materialCollection->push_back(dataPair.second);
					materialsID[mtlPath][dataPair.first] = materialCollection->size() - 1;
				}	
			}

			currentMTLPath = mtlPath;
			currentMaterialID = -1;		
			
			SkipLine(buffer, current_position);

		}
		else if (current_position + 5 < bufferSize && buffer[current_position] == 'u' &&
			buffer[current_position + 1] == 's' &&
			buffer[current_position + 2] == 'e' &&
			buffer[current_position + 3] == 'm' &&
			buffer[current_position + 4] == 't' &&
			buffer[current_position + 5] == 'l')
		{
			current_position += 7;
			std::string materialName = ReadRemainStringFromBuffer(buffer, current_position);

			//verify is last geometry data is not empty
			if (geometryDataCollection->back().indices.size() != 0)
			{
				GeometryData geometryData = GeometryData();
				geometryDataCollection->push_back(geometryData);
				objObjectCollection->back().m_groups.back().m_geometryBlockCount++;
			}

			if (materialsID.size() > 0 && materialsID[currentMTLPath].find(materialName) != materialsID[currentMTLPath].end())
				currentMaterialID = materialsID[currentMTLPath][materialName];
			else
				currentMaterialID = -1;

			geometryDataCollection->back().materialID = currentMaterialID;
			tempIndicesCollection.clear();
			
			SkipLine(buffer, current_position);
		}
		else if (buffer[current_position] == 'f')
		{
			current_position += 2;

			std::vector<std::tuple<int, int, int>> faceIndices;

			while (current_position < bufferSize && buffer[current_position] != '\n' && buffer[current_position] != '\r')
			{
				int indice0 = 0, indice1 = 0, indice2 = 0;
				indice0 = ReadLongFromBuffer(buffer, current_position);
				if (current_position < bufferSize && buffer[current_position] != '\n' && buffer[current_position] != '\r' && buffer[current_position] != ' ')
				{
					current_position += 1;

					if (current_position < bufferSize && buffer[current_position] != '\n' && buffer[current_position] != '\r' && buffer[current_position] != ' ')
					{
						indice1 = ReadLongFromBuffer(buffer, current_position);

						current_position += 1;
						if (current_position < bufferSize && buffer[current_position] != '\n' && buffer[current_position] != '\r' && buffer[current_position] != ' ')
							indice2 = ReadLongFromBuffer(buffer, current_position);
					}
				}

				if (indice0 < 0)
					indice0 = vertices.size() + 1 + indice0;

				if (indice1 < 0)
					indice1 = uvs.size() + 1 + indice1;

				if (indice2 < 0)
					indice2 = normals.size() + 1 + indice2;

				faceIndices.push_back({indice0, indice1, indice2 });

				if (current_position >= bufferSize || buffer[current_position] == '\r' || buffer[current_position] == '\n')
					break;

				current_position += 1;	
			}
			

			if (faceIndices.size() >= 3)
			{
				std::vector<Vector3D> facePoint;
				for (std::tuple<int, int, int> combinaison : faceIndices)
				{
					int vertIndex = std::get<0>(combinaison);
					Vector3D vert = vertIndex > 0 ? vertices[static_cast<size_t>(vertIndex) - 1] : vertices[vertices.size() + vertIndex];
					facePoint.push_back(vert);
				}

				std::vector<Vector2D> facePolygonPoint = GeometryUtilities::Progect3DPointsToAveragePlan(facePoint);
				std::vector<unsigned int> polygonsIndices = GeometryUtilities::TriangulatePolygonEarMethod(facePolygonPoint);

				for (unsigned int indice : polygonsIndices)
				{
					std::tuple<int, int, int> testTupple = faceIndices[indice];
					if (tempIndicesCollection.find(testTupple) != tempIndicesCollection.end())
						geometryDataCollection->back().indices.push_back(tempIndicesCollection[testTupple]);
					else
					{
						int vertIndex = std::get<0>(testTupple);
						Vector3D vert = vertices[static_cast<size_t>(vertIndex) - 1];
						geometryDataCollection->back().vertices.push_back(vert);

						int uvIndice = std::get<1>(testTupple);
						if (uvIndice != 0)
						{
							Vector2D uv = uvs[static_cast<size_t>(uvIndice) - 1];
							geometryDataCollection->back().uvs.push_back(uv);
						}
						else
							geometryDataCollection->back().uvs.push_back(Vector2D(0.0f, 0.0f));

						int normalIndice = std::get<2>(testTupple);
						if (normalIndice != 0)
						{
							Vector3D normal = normals[static_cast<size_t>(normalIndice) - 1];
							geometryDataCollection->back().normals.push_back(normal);
						}
						else
							geometryDataCollection->back().normals.push_back(Vector3D::Zero);

						geometryDataCollection->back().indices.push_back(geometryDataCollection->back().vertices.size() - 1);
						tempIndicesCollection[testTupple] = geometryDataCollection->back().vertices.size() - 1;
					}
				}
			}				
			SkipLine(buffer, current_position);
		}
		else
			SkipLine(buffer, current_position);
	}

	

	return;

}


EntityGroupAsset* OBJLoader::LoadOBJ(std::string assetsName, const std::string& filePath, const float scaleFactor)
{
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::milliseconds;

	auto t1 = high_resolution_clock::now();

	std::vector<OBJObject> objObjectCollection;
	std::vector<GeometryData> geometryDataCollection;
	std::vector<MaterialData> materialCollection;

	LoadOBJData(filePath, &objObjectCollection, &geometryDataCollection, &materialCollection, scaleFactor);

	auto t2 = high_resolution_clock::now();

	/* Getting number of milliseconds as an integer. */
	auto ms_int = duration_cast<milliseconds>(t2 - t1);

	/* Getting number of milliseconds as a double. */
	duration<double, std::milli> ms_double = t2 - t1;

	EntityGroupAsset* entityGroupAsset = AssetsManager::CreateEntityGroup(assetsName);
	Entity* objRoot = CreateSceneEntity(assetsName, entityGroupAsset);


	int currentObjectIndex = -2;
	for (int i = 0; i < objObjectCollection.size(); i++)
	{
		OBJObject& objObject = objObjectCollection.at(i);
		Entity* objEntity = CreateSceneEntity(objObject.m_name, entityGroupAsset);
		objEntity->GetRootComponent()->SetParent(objRoot->GetRootComponent());

		for (int j = 0; j < objObject.m_groups.size(); j++)
		{
			Group& group = objObject.m_groups.at(j);
			Entity* groupEntity = nullptr;
			groupEntity = entityGroupAsset->CreateEntity<Entity>();
			groupEntity->SetName(group.m_name);

			for (int k = 0; k < group.m_geometryBlockCount; k++)
			{
				GeometryData& geometryData = geometryDataCollection.at(group.m_geometryBlockStartIndex + k);
				Mesh* mesh = AssetsManager::CreateMesh(filePath + "_" + objObject.m_name + "_" + group.m_name + "_" + std::to_string(group.m_geometryBlockStartIndex + k) + "_Mesh", true);

				mesh->SetVertices(geometryData.vertices);
				mesh->SetUvs(geometryData.uvs);
				mesh->SetNormals(geometryData.normals);
				mesh->SetIndices(geometryData.indices);

				MeshRendererComponent* meshComponent = groupEntity->AddComponent<MeshRendererComponent>(true);
				meshComponent->SetMesh(mesh);
				meshComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
				int materialID = geometryData.materialID;
				MaterialData defaultMaterialData = MaterialData();
				MaterialData& materialData = materialID != -1 ? materialCollection.at(materialID) : defaultMaterialData;
				Material* material = CreateMaterial(filePath + "_" + objObject.m_name + "_" + group.m_name + "_" + std::to_string(group.m_geometryBlockStartIndex + k) + "_Material", materialData);
				meshComponent->SetMaterial(material);
				if (k == 0)
				{
					groupEntity->SetRootComponent(meshComponent);
					meshComponent->SetParent(objEntity->GetRootComponent());
				}
				else
					meshComponent->SetParent(groupEntity->GetRootComponent());
			}
		}
	}
	
	auto t3 = high_resolution_clock::now();
	/* Getting number of milliseconds as a double. */
	duration<double, std::milli> ms_double2 = t3 - t1;

	std::cout << "Load new obj : " << filePath << " in " << ms_double.count() << " / " << ms_double2.count() << "\n";

	return entityGroupAsset;
}

std::unordered_map<std::string, MaterialData> OBJLoader::LoadMTLData(const std::string& filePath)
{
	std::unordered_map<std::string, MaterialData> materialsData = std::unordered_map<std::string, MaterialData>();

	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Could not open the file: " << filePath << std::endl;
		return materialsData;
	}
	std::filesystem::path p(filePath);
	file.seekg(0, file.end);
	std::streampos total_size = file.tellg();

	file.seekg(0);
	std::streampos endPos = static_cast<size_t>(total_size) - 1;
	std::vector<char> buffer(total_size);
	file.read(buffer.data(), buffer.size());

	size_t current_position = 0;
	char character = buffer[current_position];
	bool isFirstMaterialInitialise = false;
	MaterialData currentMaterialData = MaterialData();
	while (current_position < buffer.size())
	{
		if (current_position + 1 < buffer.size() && buffer[current_position] == 'K' && buffer[current_position + 1] == 'a')
		{
			current_position += 3;
			Color color = ReadRGBColorFromBuffer(buffer, current_position);
			currentMaterialData.m_ambientColor = color;
			SkipLine(buffer, current_position);
		}
		else if (current_position + 1 < buffer.size() && buffer[current_position] == 'K' && buffer[current_position + 1] == 'd')
		{
			current_position += 3;
			Color color = ReadRGBColorFromBuffer(buffer, current_position);
			currentMaterialData.m_diffuseColor = color;
			SkipLine(buffer, current_position);
		}
		else if (current_position + 1 < buffer.size() && buffer[current_position] == 'K' && buffer[current_position + 1] == 's')
		{
			current_position += 3;
			Color color = ReadRGBColorFromBuffer(buffer, current_position);
			currentMaterialData.m_specularColor = color;
			SkipLine(buffer, current_position);
		}
		else if (current_position + 1 < buffer.size() && buffer[current_position] == 'N' && buffer[current_position + 1] == 's')
		{
			current_position += 3;
			currentMaterialData.m_shininess = ReadFloatFromBuffer(buffer, current_position);
			SkipLine(buffer, current_position);
		}
		else if (current_position + 5 < buffer.size() && buffer[current_position] == 'm' &&
			buffer[current_position + 1] == 'a' &&
			buffer[current_position + 2] == 'p' &&
			buffer[current_position + 3] == '_' &&
			buffer[current_position + 4] == 'K' &&
			buffer[current_position + 5] == 'a')
		{
			current_position += 7;
			std::string texturePath = ReadRemainStringFromBuffer(buffer, current_position);
			currentMaterialData.m_ambientMapPath = GetFullPath(p, texturePath);
			SkipLine(buffer, current_position);
		}
		else if (current_position + 5 < buffer.size() && buffer[current_position] == 'm' &&
			buffer[current_position + 1] == 'a' &&
			buffer[current_position + 2] == 'p' &&
			buffer[current_position + 3] == '_' &&
			buffer[current_position + 4] == 'K' &&
			buffer[current_position + 5] == 'd')
		{
			current_position += 7;
			std::string texturePath = ReadRemainStringFromBuffer(buffer, current_position);
			currentMaterialData.m_diffuseMapPath = GetFullPath(p, texturePath);
			SkipLine(buffer, current_position);
		}
		else if (current_position + 5 < buffer.size() && buffer[current_position] == 'm' &&
			buffer[current_position + 1] == 'a' &&
			buffer[current_position + 2] == 'p' &&
			buffer[current_position + 3] == '_' &&
			buffer[current_position + 4] == 'K' &&
			buffer[current_position + 5] == 's')
		{
			current_position += 7;
			std::string texturePath = ReadRemainStringFromBuffer(buffer, current_position);
			currentMaterialData.m_specularMapPath = GetFullPath(p, texturePath);
			SkipLine(buffer, current_position);
		}
		else if (current_position + 5 < buffer.size() && buffer[current_position] == 'n' &&
			buffer[current_position + 1] == 'e' &&
			buffer[current_position + 2] == 'w' &&
			buffer[current_position + 3] == 'm' &&
			buffer[current_position + 4] == 't' &&
			buffer[current_position + 5] == 'l')
		{
			current_position += 7;
			std::string materialName = ReadRemainStringFromBuffer(buffer, current_position);

			if (isFirstMaterialInitialise)
				materialsData[currentMaterialData.m_materialName] = currentMaterialData;
			else
				isFirstMaterialInitialise = true;

			if (materialsData.find(materialName) == materialsData.end())
			{
				currentMaterialData = MaterialData();
				currentMaterialData.m_materialName = materialName;
			}
			else
				currentMaterialData = materialsData[materialName];

			SkipLine(buffer, current_position);
		}
		else if (buffer[current_position] == '#')
			SkipLine(buffer, current_position);
		else
			current_position++;

		continue;
	}

	if (isFirstMaterialInitialise)
		materialsData[currentMaterialData.m_materialName] = currentMaterialData;

	file.close();
	return materialsData;
}


