#include "Material.h"

Material::Material()
{
	/*
	m_diffuseTexture = nullptr;
	m_specularTexture = nullptr;
	m_ambientTexture = nullptr;*/
}

const std::unordered_map<std::string, int>& Material::GetIntMap()
{
	return m_integerKeyValue;
}

const std::unordered_map<std::string, float>& Material::GetFloatMap()
{
	return m_floatKeyValue;
}

const std::unordered_map<std::string, Vector2D>& Material::GetVector2DMap()
{
	return m_vector2DKeyValue;
}

const std::unordered_map<std::string, Vector3D>& Material::GetVector3DMap()
{
	return m_vector3DKeyValue;
}

const std::unordered_map<std::string, Vector4D>& Material::GetVector4DMap()
{
	return m_vector4DKeyValue;
}

const std::unordered_map<std::string, Texture*>& Material::GetTextureMap()
{
	return m_textureKeyValue;
}

const std::unordered_map<std::string, int>& Material::GetTextureSubLayerMap()
{
    return m_textureLayerKeyValue;
}

const std::unordered_map<std::string, std::vector<float>>& Material::GetFloatArrayMap()
{
    return m_floatArrayKeyValue;
}

void Material::SetInt(std::string key, int value)
{
	m_integerKeyValue[key] = value;
}

void Material::SetFloat(std::string key, float value)
{
	m_floatKeyValue[key] = value;
}

void Material::SetVector2D(std::string key, Vector2D value)
{
	m_vector2DKeyValue[key] = value;
}

void Material::SetVector3D(std::string key, Vector3D value)
{
	m_vector3DKeyValue[key] = value;
}

void Material::SetVector4D(std::string key, Vector4D value)
{
	m_vector4DKeyValue[key] = value;
}

void Material::SetColor(std::string key, Color value)
{
	m_vector4DKeyValue[key] = Vector4D(value.m_r, value.m_g, value.m_b, value.m_a);
}

void Material::SetTexture(std::string key, Texture* value, int subLayerIndex)
{
	m_textureKeyValue[key] = value;
    m_textureLayerKeyValue[key] = subLayerIndex;
}

void Material::SetFloatArray(std::string key, std::vector<float> floatArray)
{
    m_floatArrayKeyValue[key] = floatArray;
}

int Material::GetInt(const std::string& key) const {
    auto it = m_integerKeyValue.find(key);
    if (it != m_integerKeyValue.end()) {
        return it->second;
    }
    return 0;
}

float Material::GetFloat(const std::string& key) const {
    auto it = m_floatKeyValue.find(key);
    if (it != m_floatKeyValue.end()) {
        return it->second;
    }
    return 0.0f; 
}

Vector2D Material::GetVector2D(const std::string& key) const {
    auto it = m_vector2DKeyValue.find(key);
    if (it != m_vector2DKeyValue.end()) {
        return it->second;
    }
    return Vector2D();
}

Vector3D Material::GetVector3D(const std::string& key) const {
    auto it = m_vector3DKeyValue.find(key);
    if (it != m_vector3DKeyValue.end()) {
        return it->second;
    }
    return Vector3D(); 
}

Vector4D Material::GetVector4D(const std::string& key) const {
    auto it = m_vector4DKeyValue.find(key);
    if (it != m_vector4DKeyValue.end()) {
        return it->second;
    }
    return Vector4D(); 
}

Color Material::GetColor(const std::string& key) const {
    auto it = m_vector4DKeyValue.find(key); 
    if (it != m_vector4DKeyValue.end()) {
        return Color(it->second.m_x, it->second.m_y, it->second.m_z, it->second.m_w);
    }
    return Color(); 
}

Texture* Material::GetTexture(const std::string& key) const {
    auto it = m_textureKeyValue.find(key);
    if (it != m_textureKeyValue.end()) {
        return it->second;
    }
    return nullptr; 
}

int Material::GetTextureSubLayer(const std::string& key) const {
    auto it = m_textureLayerKeyValue.find(key);
    if (it != m_textureLayerKeyValue.end()) {
        return it->second;
    }
    return 0;
}

std::vector<float> Material::GetFloatArray(const std::string& key) const
{
    auto it = m_floatArrayKeyValue.find(key);
    if (it != m_floatArrayKeyValue.end()) {
        return it->second;
    }
    return std::vector<float>();
}