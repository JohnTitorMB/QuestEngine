#include "CubeMap.h"
#include <vector>
#include <iostream>
CubeMap::CubeMap(std::string filePath) : Texture(filePath)
{
	m_layerTextureInfos.push_back(Texture::LayerTextureInfo());
	glGenTextures(1, &m_layerTextureInfos[0].m_textureID);

	unsigned char* data = LoadTexture(filePath);
	m_textureType = TextureType::CUBEMAP;

	m_layerTextureInfos[0].m_minificationFilter = MinificationFilter::Trilinear;
	m_layerTextureInfos[0].m_magnificationFilter = MagnificationFilter::Bilinear;

	m_layerTextureInfos[0].m_wrapDepthParameter = Wrap::ClampToEdge;
	m_layerTextureInfos[0].m_wrapHorizontalParameter = Wrap::ClampToEdge;
	m_layerTextureInfos[0].m_wrapVerticalParameter = Wrap::ClampToEdge;

	if (data)
	{
		UpdateTextureData(data);
		stbi_image_free(data);
		m_path = filePath;
	}
}

void CubeMap::SetTextureInternalFormat(InternalFormat internalFormat)
{
	m_layerTextureInfos[0].m_internalFormat = internalFormat;
	unsigned char* data = LoadTexture(m_path);

	if (data)
	{
		UpdateTextureData(data);
		stbi_image_free(data);
	}
}

void CubeMap::SetMipmapTexture(int level, std::string filePath, CubeMapFace cubeMapFace)
{
	int channel_in_file = 0;
	int desired_channel = 4;
	int width = 0;
	int height = 0;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channel_in_file, desired_channel);
	glBindTexture((int)m_textureType, m_layerTextureInfos[0].m_textureID);
	glTexImage2D((int)cubeMapFace, level, (int)m_layerTextureInfos[0].m_internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}


void CubeMap::UpdateTextureData(const unsigned char* data)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_layerTextureInfos[0].m_textureID);

	int subWidth = m_width/4;
	int subHeight = m_height/3;
	
	std::vector<unsigned char> leftSubData = GetSubData(data, m_width, m_height, 0, subHeight, subWidth, subHeight, false, true);
	glTexImage2D((int)CubeMapFace::NEGATIVE_X, 0, (int)m_layerTextureInfos[0].m_internalFormat, subWidth, subHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, leftSubData.data());

	std::vector<unsigned char> rightSubData = GetSubData(data, m_width, m_height, subWidth*2, subHeight, subWidth, subHeight, false, true);
	glTexImage2D((int)CubeMapFace::POSITIVE_X, 0, (int)m_layerTextureInfos[0].m_internalFormat, subWidth, subHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, rightSubData.data());

	std::vector<unsigned char> bottomSubData = GetSubData(data, m_width, m_height, subWidth, 0, subWidth, subHeight, false, true);
	glTexImage2D((int)CubeMapFace::NEGATIVE_Y, 0, (int)m_layerTextureInfos[0].m_internalFormat, subWidth, subHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bottomSubData.data());

	std::vector<unsigned char> topSubData = GetSubData(data, m_width, m_height, subWidth, subHeight*2, subWidth, subHeight, false, true);
	glTexImage2D((int)CubeMapFace::POSITIVE_Y, 0, (int)m_layerTextureInfos[0].m_internalFormat, subWidth, subHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, topSubData.data());

	std::vector<unsigned char> backSubData = GetSubData(data, m_width, m_height, subWidth * 3, subHeight, subWidth, subHeight, false, true);
	glTexImage2D((int)CubeMapFace::NEGATIVE_Z, 0, (int)m_layerTextureInfos[0].m_internalFormat, subWidth, subHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, backSubData.data());

	std::vector<unsigned char> frontSubData = GetSubData(data, m_width, m_height, subWidth, subHeight, subWidth, subHeight, false, true);
	glTexImage2D((int)CubeMapFace::POSITIVE_Z, 0, (int)m_layerTextureInfos[0].m_internalFormat, subWidth, subHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, frontSubData.data());

	
	glTexParameteri((int)m_textureType, GL_TEXTURE_MIN_FILTER, (GLint)m_layerTextureInfos[0].m_minificationFilter);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MAG_FILTER, (GLint)m_layerTextureInfos[0].m_magnificationFilter);
	glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_S, (GLint)m_layerTextureInfos[0].m_wrapHorizontalParameter);
	glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_T, (GLint)m_layerTextureInfos[0].m_wrapVerticalParameter);
	glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_R, (GLint)m_layerTextureInfos[0].m_wrapDepthParameter);

	glTexParameteri((int)m_textureType, GL_TEXTURE_BASE_LEVEL, (GLint)m_layerTextureInfos[0].m_mipmapBaseLevel);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MAX_LEVEL, (GLint)m_layerTextureInfos[0].m_mipmapMaxLevel);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MIN_LOD, (GLint)m_layerTextureInfos[0].m_mipmapMinLOD);
	glTexParameteri((int)m_textureType, GL_TEXTURE_MAX_LOD, (GLint)m_layerTextureInfos[0].m_mipmapMaxLOD);
	glTexParameteri((int)m_textureType, GL_TEXTURE_LOD_BIAS, (GLint)m_layerTextureInfos[0].m_mipmapLODBias);
	glGenerateMipmap((int)m_textureType);

	SetAnisotropy(m_layerTextureInfos[0].m_anisotropyValue);

}
