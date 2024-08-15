#include "Texture.h"
#include <iostream>
#define GL_MIRROR_CLAMP_TO_EDGE_EXT 0x8743
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

Texture::Texture(std::string filePath)
{
	GenereTextureID();
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_textureID);
}

unsigned char* Texture::LoadTexture(std::string filePath)
{
	int channel_in_file = 0;
	int desired_channel = 4;
	
	stbi_set_flip_vertically_on_load(1);
	unsigned char* data = stbi_load(filePath.c_str(), &m_width, &m_height, &channel_in_file, desired_channel);
	return data;
}


std::vector<unsigned char> Texture::GetSubData(const unsigned char* data, int srcWidth, int srcHeight, int x, int y, int subWidth, int subHeight, bool inversedX, bool inversedY)
{
	std::vector<unsigned char> subdata(subWidth * subHeight * 4);

	for (int i = y; i < y + subHeight; ++i) {
		for (int j = x; j < x + subWidth; ++j)
		{
			int src_index = i * srcWidth * 4 + j * 4;
			int dst_index = (inversedY ? subHeight - 1 - (i - y) : (i - y)) * subWidth * 4 + (inversedX ? subWidth -1 - (j - x) : (j - x)) * 4;
			subdata[dst_index] = data[src_index];
			subdata[dst_index + 1] = data[src_index + 1];
			subdata[dst_index + 2] = data[src_index + 2];
			subdata[dst_index + 3] = data[src_index + 3];
		}
	}

	return subdata;
}

void Texture::GenereTextureID()
{
	glGenTextures(1, &m_textureID);
}




void Texture::Bind(int textureIndex)
{
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture((int)m_textureType, m_textureID);
}


void Texture::Unbind(int textureIndex)
{
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture((int)m_textureType, m_textureID);
}

void Texture::SetWrapHorizontalParameter(Wrap wrapHorizontalParameter)
{
	m_wrapHorizontalParameter = wrapHorizontalParameter;
	glBindTexture((int)m_textureType, m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_S, (GLint)m_wrapHorizontalParameter);
}

void Texture::SetVerticalParameter(Wrap wrapVerticalParameter)
{
	m_wrapVerticalParameter = wrapVerticalParameter;
	glBindTexture((int)m_textureType, m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_T, (GLint)m_wrapVerticalParameter);
}

void Texture::SetDepthParameter(Wrap wrapDepthParameter)
{
	m_wrapDepthParameter = wrapDepthParameter;
	glBindTexture((int)m_textureType, m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_R, (GLint)m_wrapDepthParameter);
}

Wrap Texture::GetHorizontalParameter()const
{
	return m_wrapHorizontalParameter;
}

Wrap Texture::GetVerticalParameter()const
{
	return m_wrapVerticalParameter;
}

Wrap Texture::GetDepthParameter()const
{
	return m_wrapDepthParameter;
}


void Texture::SetMinification(MinificationFilter minificationFilter)
{
	m_minificationFilter = minificationFilter;
	glBindTexture((int)m_textureType, m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MIN_FILTER, (GLint)m_minificationFilter);
}

void Texture::SetMagnification(MagnificationFilter magnificationFilter)
{
	m_magnificationFilter = magnificationFilter;
	glBindTexture((int)m_textureType, m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MAG_FILTER, (GLint)m_magnificationFilter);
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
	glBindTexture((int)m_textureType, m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_BASE_LEVEL, (GLint)m_mipmapBaseLevel);
}

void Texture::SetMipmapMaxLevel(int mipmapMaxLevel)
{
	m_mipmapMaxLevel = mipmapMaxLevel;
	glBindTexture((int)m_textureType, m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MAX_LEVEL, (GLint)m_mipmapMaxLevel);
}

void Texture::SetMipmapMinLOD(int mipmapMinLOD)
{
	m_mipmapMinLOD = mipmapMinLOD;
	glBindTexture((int)m_textureType, m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MIN_LOD, (GLint)m_mipmapMinLOD);
}

void Texture::SetMipmapMaxLOD(int mipmapMaxLOD)
{
	m_mipmapMaxLOD = mipmapMaxLOD;
	glBindTexture((int)m_textureType, m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MAX_LOD, (GLint)m_mipmapMaxLOD);
}

void Texture::SetMipmapLODBias(int mipmapLODBias)
{
	m_mipmapLODBias = mipmapLODBias;
	glBindTexture((int)m_textureType, m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_LOD_BIAS, (GLint)m_mipmapLODBias);
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
	glBindTexture((int)m_textureType, m_textureID);
	glTexParameterf((int)m_textureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_anisotropyValue);

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

float Texture::GetWidth() const
{
	return m_width;
}

float Texture::GetHeight() const
{
	return m_height;
}
