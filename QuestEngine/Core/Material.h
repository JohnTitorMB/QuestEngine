#ifndef _MATERIAL_H_
#define _MATERIAL_H_
#include "Texture.h"
#include "Color.h"
class Material
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
};
#endif

