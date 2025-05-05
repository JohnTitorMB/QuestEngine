#ifndef _MATERIAL_H_
#define _MATERIAL_H_
#include "Texture.h"
#include "../ColorRGB.h"
#include "../ColorRGBY.h"
#include "Assets.h"
#include <unordered_map>
#include <vector>
#include "../../Math/Vector2D.h"
#include "../../Math/Vector3D.h"
#include "../../Math/Vector4D.h"
#include "Texture.h"
#include "../ColorRGB.h"

class Material : public Assets
{
	std::unordered_map<std::string, int> m_integerKeyValue;
	std::unordered_map<std::string, float> m_floatKeyValue;
	std::unordered_map<std::string, Vector2D> m_vector2DKeyValue;
	std::unordered_map<std::string, Vector3D> m_vector3DKeyValue;
	std::unordered_map<std::string, Vector4D> m_vector4DKeyValue;
	std::unordered_map<std::string, ColorRGB> m_colorKeyValue;
	std::unordered_map<std::string, ColorRGBY> m_colorRGBYKeyValue;

	std::unordered_map<std::string, Texture*> m_textureKeyValue;
	std::unordered_map<std::string,int> m_textureLayerKeyValue;
	std::unordered_map<std::string, std::vector<float>> m_floatArrayKeyValue;

public:
	Material();
	std::string name = "";

	const std::unordered_map<std::string, int>& GetIntMap();
	const std::unordered_map<std::string, float>& GetFloatMap();
	const std::unordered_map<std::string, Vector2D>& GetVector2DMap();
	const std::unordered_map<std::string, Vector3D>& GetVector3DMap();
	const std::unordered_map<std::string, Vector4D>& GetVector4DMap();
	const std::unordered_map<std::string, Texture*>& GetTextureMap();
	const std::unordered_map<std::string, int>& GetTextureSubLayerMap();
	const std::unordered_map<std::string, std::vector<float>>& GetFloatArrayMap();

	const std::unordered_map<std::string, ColorRGB>& GetColorRGBMap();
	const std::unordered_map<std::string, ColorRGBY>& GetColorYMap();



	void SetInt(std::string key, int value);
	void SetFloat(std::string key, float value);
	void SetVector2D(std::string key, Vector2D value);
	void SetVector3D(std::string key, Vector3D value);
	void SetVector4D(std::string key, Vector4D value);
	void SetColor(std::string key, ColorRGB value);
	void SetColorY(std::string key, ColorRGBY value);
	void SetTexture(std::string key, Texture* value, int subLayerIndex = 0);

	void SetFloatArray(std::string key, std::vector<float> floatArray);

	int GetInt(const std::string& key)const;
	float GetFloat(const std::string& key)const;
	Vector2D GetVector2D(const std::string& key)const;
	Vector3D GetVector3D(const std::string& key)const;
	Vector4D GetVector4D(const std::string& key)const;
	ColorRGB GetColor(const std::string& key)const;
	ColorRGBY GetColorY(const std::string& key) const;

	Texture* GetTexture(const std::string& key)const;
	int GetTextureSubLayer(const std::string& key) const;
	std::vector<float> GetFloatArray(const std::string& key)const;

};

#endif

