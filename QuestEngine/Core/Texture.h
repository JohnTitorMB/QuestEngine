#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include <string>
#include <glad/glad.h>

class Texture
{
public:
	Texture(std::string filePath);
	~Texture();
	void UseTexture();

private:
	unsigned char* LoadTexture(std::string filePath);
	void GenereTextureID();
	void UpdateTextureData(const unsigned char* data);

	GLuint m_textureID;
	int m_width = 0;
	int m_height = 0;
};

#endif // !_TEXTURE_H_

