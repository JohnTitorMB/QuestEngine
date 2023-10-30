#include "Texture.h"
#include "../Library/stb_image.h"
#include <iostream>
#define GL_MIRROR_CLAMP_TO_EDGE_EXT 0x8743
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)m_minificationFilter); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)m_magnificationFilter); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)m_wrapHorizontalParameter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)m_wrapVerticalParameter);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, (GLint)m_mipmapBaseLevel);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, (GLint)m_mipmapMaxLevel);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, (GLint)m_mipmapMinLOD);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, (GLint)m_mipmapMaxLOD);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, (GLint)m_mipmapLODBias);
	glGenerateMipmap(GL_TEXTURE_2D);

	SetAnisotropy(m_anisotropyValue);

}

void Texture::SetMipmapTexture(int level, std::string filePath)
{
	int channel_in_file = 0;
	int desired_channel = 4;

	int width = 0;
	int height = 0;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channel_in_file, desired_channel);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

}

void Texture::UseTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::SetWrapHorizontalParameter(Wrap wrapHorizontalParameter)
{
	m_wrapHorizontalParameter = wrapHorizontalParameter;
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)m_wrapHorizontalParameter);
}

void Texture::SetVerticalParameter(Wrap wrapVerticalParameter)
{
	m_wrapVerticalParameter = wrapVerticalParameter;
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)m_wrapVerticalParameter);
}

Wrap Texture::GetHorizontalParameter()const
{
	return m_wrapHorizontalParameter;
}

Wrap Texture::GetVerticalParameter()const
{
	return m_wrapVerticalParameter;
}

void Texture::SetMinification(MinificationFilter minificationFilter)
{
	m_minificationFilter = minificationFilter;
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)m_minificationFilter);
}

void Texture::SetMagnification(MagnificationFilter magnificationFilter)
{
	m_magnificationFilter = magnificationFilter;
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)m_magnificationFilter);
}

MinificationFilter Texture::SetMinification()const
{
	return m_minificationFilter;
}

MagnificationFilter Texture::SetMagnification()const
{
	return m_magnificationFilter;
}

void Texture::SetMipmapBaseLevel(int mipmapBaseLevel)
{
	m_mipmapBaseLevel = mipmapBaseLevel;
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, (GLint)m_mipmapBaseLevel);
}

void Texture::SetMipmapMaxLevel(int mipmapMaxLevel)
{
	m_mipmapMaxLevel = mipmapMaxLevel;
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, (GLint)m_mipmapMaxLevel);
}

void Texture::SetMipmapMinLOD(int mipmapMinLOD)
{
	m_mipmapMinLOD = mipmapMinLOD;
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, (GLint)m_mipmapMinLOD);
}

void Texture::SetMipmapMaxLOD(int mipmapMaxLOD)
{
	m_mipmapMaxLOD = mipmapMaxLOD;
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, (GLint)m_mipmapMaxLOD);
}

void Texture::SetMipmapLODBias(int mipmapLODBias)
{
	m_mipmapLODBias = mipmapLODBias;
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, (GLint)m_mipmapLODBias);
}

int Texture::GetMipmapBaseLevel()const
{
	return m_mipmapBaseLevel;
}

int Texture::GetMipmapMaxLevel()const
{
	return m_mipmapMaxLevel;
}

int Texture::GetMipmapMinLOD()const
{
	return m_mipmapMinLOD;
}

int Texture::GetMipmapMaxLOD()const
{
	return m_mipmapMaxLOD;
}

int Texture::GetMipmapLODBIAS()const
{
	return m_mipmapLODBias;
}

void Texture::SetAnisotropy(float anisotropyValue)
{
	float maxAnisotropy = GetMaxGPUAnisotropy();
	m_anisotropyValue = anisotropyValue > maxAnisotropy ? maxAnisotropy : anisotropyValue;
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_anisotropyValue);

}

float Texture::GetAnisotropy()const
{
	return m_anisotropyValue;
}

float Texture::GetMaxGPUAnisotropy()const
{
	GLfloat maxAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	return maxAnisotropy;
}