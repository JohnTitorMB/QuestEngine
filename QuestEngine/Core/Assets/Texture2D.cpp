#include "Texture2D.h"

Texture2D::Texture2D(std::string filePath) : Texture(filePath)
{
	m_layerTextureInfos.push_back(Texture::LayerTextureInfo());
	glGenTextures(1, &m_layerTextureInfos[0].m_textureID);

	
	unsigned char* data = LoadTexture(filePath);
	m_textureType = TextureType::TEXTURE2D;
	
	if (data)
	{
		UpdateTextureData(data);
		stbi_image_free(data);
		m_path = filePath;
	}
}

void Texture2D::SetMipmapTexture(int level, std::string filePath)
{
	int channel_in_file = 0;
	int desired_channel = 4;
	int width = 0;
	int height = 0;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channel_in_file, desired_channel);
	glBindTexture((int)m_textureType, m_layerTextureInfos[0].m_textureID);
	glTexImage2D((int)m_textureType, level, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

void Texture2D::UpdateTextureData(const unsigned char* data)
{

	glBindTexture((int)m_textureType, m_layerTextureInfos[0].m_textureID);
	glTexImage2D((int)m_textureType, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri((int)m_textureType, GL_TEXTURE_MIN_FILTER, (GLint)m_layerTextureInfos[0].m_minificationFilter);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MAG_FILTER, (GLint)m_layerTextureInfos[0].m_magnificationFilter);
	glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_S, (GLint)m_layerTextureInfos[0].m_wrapHorizontalParameter);
	glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_T, (GLint)m_layerTextureInfos[0].m_wrapVerticalParameter);

	glTexParameteri((int)m_textureType, GL_TEXTURE_BASE_LEVEL, (GLint)m_layerTextureInfos[0].m_mipmapBaseLevel);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MAX_LEVEL, (GLint)m_layerTextureInfos[0].m_mipmapMaxLevel);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MIN_LOD, (GLint)m_layerTextureInfos[0].m_mipmapMinLOD);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MAX_LOD, (GLint)m_layerTextureInfos[0].m_mipmapMaxLOD);
	glTexParameteri((int)m_textureType, GL_TEXTURE_LOD_BIAS, (GLint)m_layerTextureInfos[0].m_mipmapLODBias);
	glGenerateMipmap((int)m_textureType);

	SetAnisotropy(m_layerTextureInfos[0].m_anisotropyValue);

}
