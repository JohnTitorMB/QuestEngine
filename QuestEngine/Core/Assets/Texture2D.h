#ifndef _TEXTURE2D_H_
#define _TEXTURE2D_H_
#include "Texture.h"

class Texture2D : public Texture
{
private:
	void UpdateTextureData(const unsigned char* data);
public:
	Texture2D(std::string filePath);
	void SetMipmapTexture(int level, std::string filePath);
	void SetTextureInternalFormat(InternalFormat internalFormat);

};

#endif //_TEXTURE2D_H_

