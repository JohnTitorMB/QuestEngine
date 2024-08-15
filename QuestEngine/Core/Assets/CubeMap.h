#ifndef _CUBEMAP_H_
#define _CUBEMAP_H_
#include "Texture.h"
enum class CubeMapFace
{
	POSITIVE_X  = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	NEGATIVE_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	POSITIVE_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	NEGATIVE_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	POSITIVE_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	NEGATIVE_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
};

class CubeMap : public Texture
{
private:
	void UpdateTextureData(const unsigned char* data);
public:
	CubeMap(std::string filePath);
	void SetMipmapTexture(int level, std::string filePath, CubeMapFace cubeMapFace);
};

#endif // _CUBEMAP_H_

