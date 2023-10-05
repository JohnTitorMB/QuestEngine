#include "Texture.h"
#include "../Library/stb_image.h"

Texture::Texture(std::string filePath)
{
	GenereTextureID();
	unsigned char * data = LoadTexture(filePath);
	UpdateTextureData(data);
	stbi_image_free(data);
}

Texture::~Texture()
{
}

unsigned char* Texture::LoadTexture(std::string filePath)
{
	int channel_in_file = 0;
	int desired_channel = 4;
	
	stbi_set_flip_vertically_on_load(1);
	unsigned char* data = stbi_load(filePath.c_str(), &m_width, &m_height, &channel_in_file, desired_channel);
	return data;
}

void Texture::GenereTextureID()
{
	glGenTextures(1, &m_textureID);
}

void Texture::UpdateTextureData(const unsigned char* data)
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void Texture::UseTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}
