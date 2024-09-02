#include "Texture.h"
#include <iostream>
#define _CRT_SECURE_NO_WARNINGS
#include "../../Library/stb_image_write.h"
#define GL_MIRROR_CLAMP_TO_EDGE_EXT 0x8743
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

Texture::Texture(std::string filePath)
{
}

Texture::Texture()
{
}

Texture::~Texture()
{
	for (const LayerTextureInfo& info : m_layerTextureInfos)
		glDeleteTextures(1, &info.m_textureID);

	m_layerTextureInfos.clear();
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



void Texture::Bind(int textureIndex, int layer)
{
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture((int)m_textureType, m_layerTextureInfos[layer].m_textureID);
}


void Texture::Unbind(int textureIndex, int layer)
{
	glBindTexture((int)m_textureType, 0);
}

void Texture::SetWrapHorizontalParameter(Wrap wrapHorizontalParameter, int layer)
{
	m_layerTextureInfos[layer].m_wrapHorizontalParameter = wrapHorizontalParameter;
	glBindTexture((int)m_textureType, m_layerTextureInfos[layer].m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_S, (GLint)m_layerTextureInfos[layer].m_wrapHorizontalParameter);
}

void Texture::SetVerticalParameter(Wrap wrapVerticalParameter, int layer)
{
	m_layerTextureInfos[layer].m_wrapVerticalParameter = wrapVerticalParameter;
	glBindTexture((int)m_textureType, m_layerTextureInfos[layer].m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_T, (GLint)m_layerTextureInfos[layer].m_wrapVerticalParameter);
}

void Texture::SetDepthParameter(Wrap wrapDepthParameter, int layer)
{
	m_layerTextureInfos[layer].m_wrapDepthParameter = wrapDepthParameter;
	glBindTexture((int)m_textureType, m_layerTextureInfos[layer].m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_R, (GLint)m_layerTextureInfos[layer].m_wrapDepthParameter);
}

void Texture::GenerateMipmap(int layer)
{
	glBindTexture((int)m_textureType, m_layerTextureInfos[layer].m_textureID);
	glGenerateMipmap((int)m_textureType);
}

void Texture::GenerateAllMipmap(bool forceGeneration)
{
	for (int i = 0; i < m_layerTextureInfos.size(); i++)
	{
		if (forceGeneration || m_layerTextureInfos[i].m_generateMimpap)
			GenerateMipmap(i);
	}	
}

Wrap Texture::GetHorizontalParameter(int layer)const
{
	return m_layerTextureInfos[layer].m_wrapHorizontalParameter;
}

Wrap Texture::GetVerticalParameter(int layer)const
{
	return m_layerTextureInfos[layer].m_wrapVerticalParameter;
}

Wrap Texture::GetDepthParameter(int layer)const
{
	return m_layerTextureInfos[layer].m_wrapDepthParameter;
}


void Texture::SetMinification(MinificationFilter minificationFilter, int layer)
{
	m_layerTextureInfos[layer].m_minificationFilter = minificationFilter;
	glBindTexture((int)m_textureType, m_layerTextureInfos[layer].m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MIN_FILTER, (GLint)m_layerTextureInfos[layer].m_minificationFilter);
}

void Texture::SetMagnification(MagnificationFilter magnificationFilter, int layer)
{
	m_layerTextureInfos[layer].m_magnificationFilter = magnificationFilter;
	glBindTexture((int)m_textureType, m_layerTextureInfos[layer].m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MAG_FILTER, (GLint)m_layerTextureInfos[layer].m_magnificationFilter);
}

MinificationFilter Texture::GetMinification(int layer)const
{
	return m_layerTextureInfos[layer].m_minificationFilter;
}

MagnificationFilter Texture::GetMagnification(int layer)const
{
	return m_layerTextureInfos[layer].m_magnificationFilter;
}

void Texture::SetMipmapBaseLevel(int mipmapBaseLevel, int layer)
{
	m_layerTextureInfos[layer].m_mipmapBaseLevel = mipmapBaseLevel;
	glBindTexture((int)m_textureType, m_layerTextureInfos[layer].m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_BASE_LEVEL, (GLint)m_layerTextureInfos[layer].m_mipmapBaseLevel);
}

void Texture::SetMipmapMaxLevel(int mipmapMaxLevel, int layer)
{
	m_layerTextureInfos[layer].m_mipmapMaxLevel = mipmapMaxLevel;
	glBindTexture((int)m_textureType, m_layerTextureInfos[layer].m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MAX_LEVEL, (GLint)m_layerTextureInfos[layer].m_mipmapMaxLevel);
}

void Texture::SetMipmapMinLOD(int mipmapMinLOD, int layer)
{
	m_layerTextureInfos[layer].m_mipmapMinLOD = mipmapMinLOD;
	glBindTexture((int)m_textureType, m_layerTextureInfos[layer].m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MIN_LOD, (GLint)m_layerTextureInfos[layer].m_mipmapMinLOD);
}

void Texture::SetMipmapMaxLOD(int mipmapMaxLOD, int layer)
{
	m_layerTextureInfos[layer].m_mipmapMaxLOD = mipmapMaxLOD;
	glBindTexture((int)m_textureType, m_layerTextureInfos[layer].m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MAX_LOD, (GLint)m_layerTextureInfos[layer].m_mipmapMaxLOD);
}

void Texture::SetMipmapLODBias(int mipmapLODBias, int layer)
{
	m_layerTextureInfos[layer].m_mipmapLODBias = mipmapLODBias;
	glBindTexture((int)m_textureType, m_layerTextureInfos[layer].m_textureID);
	glTexParameteri((int)m_textureType, GL_TEXTURE_LOD_BIAS, (GLint)m_layerTextureInfos[layer].m_mipmapLODBias);
}

int Texture::GetMipmapBaseLevel(int layer)const
{
	return m_layerTextureInfos[layer].m_mipmapBaseLevel;
}

int Texture::GetMipmapMaxLevel(int layer)const
{
	return m_layerTextureInfos[layer].m_mipmapMaxLevel;
}

int Texture::GetMipmapMinLOD(int layer)const
{
	return m_layerTextureInfos[layer].m_mipmapMinLOD;
}

int Texture::GetMipmapMaxLOD(int layer)const
{
	return m_layerTextureInfos[layer].m_mipmapMaxLOD;
}

int Texture::GetMipmapLODBIAS(int layer)const
{
	return m_layerTextureInfos[layer].m_mipmapLODBias;
}

void Texture::SetAnisotropy(float anisotropyValue, int layer)
{
	float maxAnisotropy = GetMaxGPUAnisotropy();
	m_layerTextureInfos[layer].m_anisotropyValue = anisotropyValue > maxAnisotropy ? maxAnisotropy : anisotropyValue;
	glBindTexture((int)m_textureType, m_layerTextureInfos[layer].m_textureID);
	glTexParameterf((int)m_textureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_layerTextureInfos[layer].m_anisotropyValue);

}

float Texture::GetAnisotropy(int layer)const
{
	return m_layerTextureInfos[layer].m_anisotropyValue;
}

float Texture::GetMaxGPUAnisotropy(int layer)const
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

int Texture::GetSubLayerCount() const
{
	return m_layerTextureInfos.size();
}
