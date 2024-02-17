#include "Material.h"

Material::Material()
{
	m_diffuseTexture = nullptr;
	m_specularTexture = nullptr;
	m_ambientTexture = nullptr;
}

Material::Material(Texture* ambientTexture, Texture* diffuseTexture, Texture* specularTexture, Color ambientColor, Color diffuseColor, Color specularColor, float shininess)
{
	m_ambientColor = ambientColor;
	m_diffuseColor = diffuseColor;
	m_specularColor = specularColor;
	m_ambientTexture = ambientTexture;
	m_diffuseTexture = diffuseTexture;
	m_specularTexture = specularTexture;
	m_shininess = shininess;
}