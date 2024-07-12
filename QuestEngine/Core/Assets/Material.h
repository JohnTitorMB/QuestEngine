#ifndef _MATERIAL_H_
#define _MATERIAL_H_
#include "Texture.h"
#include "../Color.h"
#include "Assets.h"
#include <unordered_map>
#include "../../Math/Vector2D.h"
#include "../../Math/Vector3D.h"
#include "../../Math/Vector4D.h"
#include "Texture.h"
#include "../Color.h"

/*
class Material : public Assets
{
public:
	Material();
	Material(Texture* ambientTexture, Texture* diffuseTexture, Texture* specularTexture, Color ambientColor, Color diffuseColor, Color specularColor, float shininess = 32);

	Color m_ambientColor;
	Color m_diffuseColor;
	Color m_specularColor;

	Texture* m_ambientTexture;
	Texture* m_diffuseTexture;
	Texture* m_specularTexture;
	float m_shininess = 32;
	std::string name = "";
};
*/
class Material : public Assets
{
	std::unordered_map<std::string, int> m_integerKeyValue;
	std::unordered_map<std::string, float> m_floatKeyValue;
	std::unordered_map<std::string, Vector2D> m_vector2DKeyValue;
	std::unordered_map<std::string, Vector3D> m_vector3DKeyValue;
	std::unordered_map<std::string, Vector4D> m_vector4DKeyValue;
	std::unordered_map<std::string, Texture*> m_textureKeyValue;

public:
	Material();
	std::string name = "";

	const std::unordered_map<std::string, int>& GetIntMap();
	const std::unordered_map<std::string, float>& GetFloatMap();
	const std::unordered_map<std::string, Vector2D>& GetVector2DMap();
	const std::unordered_map<std::string, Vector3D>& GetVector3DMap();
	const std::unordered_map<std::string, Vector4D>& GetVector4DMap();
	const std::unordered_map<std::string, Texture*>& GetTextureMap();

	void SetInt(std::string key, int value);
	void SetFloat(std::string key, float value);
	void SetVector2D(std::string key, Vector2D value);
	void SetVector3D(std::string key, Vector3D value);
	void SetVector4D(std::string key, Vector4D value);
	void SetColor(std::string key, Color value);
	void SetTexture(std::string key, Texture* value);

	int GetInt(const std::string& key)const;
	float GetFloat(const std::string& key)const;
	Vector2D GetVector2D(const std::string& key)const;
	Vector3D GetVector3D(const std::string& key)const;
	Vector4D GetVector4D(const std::string& key)const;
	Color GetColor(const std::string& key)const;
	Texture* GetTexture(const std::string& key)const;
};

#endif

