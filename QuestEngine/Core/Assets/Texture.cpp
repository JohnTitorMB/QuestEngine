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



int Texture::GetTextureLayerType(int layer)
{
	if (m_layerTextureInfos[layer].m_useMultisampledTexture && m_textureType == TextureType::TEXTURE2D)
		return (int)GL_TEXTURE_2D_MULTISAMPLE;
	else
		return (int)m_textureType;
}

void Texture::Bind(int textureIndex, int layer)
{
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(GetTextureLayerType(layer), m_layerTextureInfos[layer].m_textureID);
}


void Texture::Unbind(int textureIndex, int layer)
{
	glBindTexture((int)m_textureType, 0);
}

void Texture::SetWrapHorizontalParameter(Wrap wrapHorizontalParameter, int layer)
{
	m_layerTextureInfos[layer].m_wrapHorizontalParameter = wrapHorizontalParameter;
	glBindTexture(GetTextureLayerType(layer), m_layerTextureInfos[layer].m_textureID);
	glTexParameteri(GetTextureLayerType(layer), GL_TEXTURE_WRAP_S, (GLint)m_layerTextureInfos[layer].m_wrapHorizontalParameter);
}

void Texture::SetWrapVerticalParameter(Wrap wrapVerticalParameter, int layer)
{
	m_layerTextureInfos[layer].m_wrapVerticalParameter = wrapVerticalParameter;
	glBindTexture(GetTextureLayerType(layer), m_layerTextureInfos[layer].m_textureID);
	glTexParameteri(GetTextureLayerType(layer), GL_TEXTURE_WRAP_T, (GLint)m_layerTextureInfos[layer].m_wrapVerticalParameter);
}

void Texture::SetWrapDepthParameter(Wrap wrapDepthParameter, int layer)
{
	m_layerTextureInfos[layer].m_wrapDepthParameter = wrapDepthParameter;
	glBindTexture(GetTextureLayerType(layer), m_layerTextureInfos[layer].m_textureID);
	glTexParameteri(GetTextureLayerType(layer), GL_TEXTURE_WRAP_R, (GLint)m_layerTextureInfos[layer].m_wrapDepthParameter);
}

void Texture::GenerateMipmap(int layer)
{
	glBindTexture((int)GetTextureLayerType(layer), m_layerTextureInfos[layer].m_textureID);
	glGenerateMipmap((int)GetTextureLayerType(layer));
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
	glBindTexture((int)GetTextureLayerType(layer), m_layerTextureInfos[layer].m_textureID);
	glTexParameteri((int)GetTextureLayerType(layer), GL_TEXTURE_MIN_FILTER, (GLint)m_layerTextureInfos[layer].m_minificationFilter);
}

void Texture::SetMagnification(MagnificationFilter magnificationFilter, int layer)
{
	m_layerTextureInfos[layer].m_magnificationFilter = magnificationFilter;
	glBindTexture((int)GetTextureLayerType(layer), m_layerTextureInfos[layer].m_textureID);
	glTexParameteri((int)GetTextureLayerType(layer), GL_TEXTURE_MAG_FILTER, (GLint)m_layerTextureInfos[layer].m_magnificationFilter);
}

void Texture::SetTextureConversionMode(TextureConversionMode textureConversionMode, int layer)
{
	m_layerTextureInfos[layer].m_textureConversionMode = textureConversionMode;
	RefreshTextureData(layer);
}

void Texture::SetTextureColorSpace(ColorManagement::RGBColorSpaceType colorSpace, int layer)
{
	m_layerTextureInfos[layer].m_colorSpace = colorSpace;
	RefreshTextureData(layer);
}

void Texture::SetTextureColorManagementParam(ColorManagement::RGBColorSpaceType colorSpace, TextureConversionMode textureConversionMode, int layer)
{
	m_layerTextureInfos[layer].m_textureConversionMode = textureConversionMode;
	m_layerTextureInfos[layer].m_colorSpace = colorSpace;
	RefreshTextureData(layer);
}

void Texture::SetBorderColor(Vector4D color, int layer)
{
	m_layerTextureInfos[layer].m_borderColor = color;
	glBindTexture((int)GetTextureLayerType(layer), m_layerTextureInfos[layer].m_textureID);
	float borderColor[] = { color.m_x, color.m_y, color.m_z, color.m_w };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void Texture::RefreshTextureData(int layer)
{

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
	glBindTexture((int)GetTextureLayerType(layer), m_layerTextureInfos[layer].m_textureID);
	glTexParameteri((int)GetTextureLayerType(layer), GL_TEXTURE_BASE_LEVEL, (GLint)m_layerTextureInfos[layer].m_mipmapBaseLevel);
}

void Texture::SetMipmapMaxLevel(int mipmapMaxLevel, int layer)
{
	m_layerTextureInfos[layer].m_mipmapMaxLevel = mipmapMaxLevel;
	glBindTexture((int)GetTextureLayerType(layer), m_layerTextureInfos[layer].m_textureID);
	glTexParameteri((int)GetTextureLayerType(layer), GL_TEXTURE_MAX_LEVEL, (GLint)m_layerTextureInfos[layer].m_mipmapMaxLevel);
}

void Texture::SetMipmapMinLOD(int mipmapMinLOD, int layer)
{
	m_layerTextureInfos[layer].m_mipmapMinLOD = mipmapMinLOD;
	glBindTexture((int)GetTextureLayerType(layer), m_layerTextureInfos[layer].m_textureID);
	glTexParameteri((int)GetTextureLayerType(layer), GL_TEXTURE_MIN_LOD, (GLint)m_layerTextureInfos[layer].m_mipmapMinLOD);
}

void Texture::SetMipmapMaxLOD(int mipmapMaxLOD, int layer)
{
	m_layerTextureInfos[layer].m_mipmapMaxLOD = mipmapMaxLOD;
	glBindTexture((int)GetTextureLayerType(layer), m_layerTextureInfos[layer].m_textureID);
	glTexParameteri((int)GetTextureLayerType(layer), GL_TEXTURE_MAX_LOD, (GLint)m_layerTextureInfos[layer].m_mipmapMaxLOD);
}

void Texture::SetMipmapLODBias(int mipmapLODBias, int layer)
{
	m_layerTextureInfos[layer].m_mipmapLODBias = mipmapLODBias;
	glBindTexture((int)GetTextureLayerType(layer), m_layerTextureInfos[layer].m_textureID);
	glTexParameteri((int)GetTextureLayerType(layer), GL_TEXTURE_LOD_BIAS, (GLint)m_layerTextureInfos[layer].m_mipmapLODBias);
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
	glBindTexture((int)GetTextureLayerType(layer), m_layerTextureInfos[layer].m_textureID);
	glTexParameterf((int)GetTextureLayerType(layer), GL_TEXTURE_MAX_ANISOTROPY_EXT, m_layerTextureInfos[layer].m_anisotropyValue);

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

const Texture::LayerTextureInfo Texture::GetTextureLayerInfo(int layer) const
{
	return m_layerTextureInfos[layer];
}

bool Texture::SaveTextureToPNG(GLuint textureID, int width, int height, const std::string& filename)
{
	glBindTexture(GL_TEXTURE_2D, textureID);

	std::vector<unsigned char> pixels(width * height * 4);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

	// Flip vertically (OpenGL's origin is bottom-left, PNG's is top-left)
	for (int y = 0; y < height / 2; ++y) {
		int top = y * width * 4;
		int bottom = (height - y - 1) * width * 4;
		for (int x = 0; x < width * 4; ++x) {
			std::swap(pixels[top + x], pixels[bottom + x]);
		}
	}

	int result = stbi_write_png(filename.c_str(), width, height, 4, pixels.data(), width * 4);
	glBindTexture(GL_TEXTURE_2D, 0);
	return result != 0;
}