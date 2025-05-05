#include "CubeMap.h"
#include <vector>
#include <iostream>
#include "../ColorManagement/Color.h"
#include "../ColorManagement/RGBModel.h"
#include "../ColorManagement/ColorConversion.h"
#include "../../Math/Mathf.h"

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
    RefreshTextureData(0);
}

void CubeMap::RefreshTextureData(int layer)
{
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
    auto& info = m_layerTextureInfos[0];
    glBindTexture(GL_TEXTURE_CUBE_MAP, info.m_textureID);

    int subWidth = m_width / 4;
    int subHeight = m_height / 3;

    auto srcColorSpace = ColorManagement::GetRGBColorSpaceFromType(info.m_colorSpace);
    auto dstColorSpace = ColorManagement::Instance().GetGPUWorkingSpace();

    auto uploadFace = [&](CubeMapFace face, int offsetX, int offsetY) {
        std::vector<unsigned char> rawSubData = GetSubData(data, m_width, m_height, offsetX, offsetY, subWidth, subHeight, false, true);

        if (info.m_textureConversionMode == TextureConversionMode::CPUConvert) {
            std::vector<unsigned char> convertedData(subWidth * subHeight * 4);

            for (int i = 0; i < subWidth * subHeight; ++i) {
                std::unique_ptr<ColorModel> model = std::make_unique<RGBModel>(
                    rawSubData[i * 4 + 0] / 255.0f,
                    rawSubData[i * 4 + 1] / 255.0f,
                    rawSubData[i * 4 + 2] / 255.0f
                );

                Color color(std::move(model), srcColorSpace, rawSubData[i * 4 + 3] / 255.0f);
                Color converted = ColorConversion::Convert(color, ColorModelType::RGB, dstColorSpace);
                const RGBModel& rgb = static_cast<const RGBModel&>(converted.GetColorModel());

                convertedData[i * 4 + 0] = static_cast<unsigned char>(Mathf::Clamp(rgb.m_r * 255.0f, 0.0f, 255.0f));
                convertedData[i * 4 + 1] = static_cast<unsigned char>(Mathf::Clamp(rgb.m_g * 255.0f, 0.0f, 255.0f));
                convertedData[i * 4 + 2] = static_cast<unsigned char>(Mathf::Clamp(rgb.m_b * 255.0f, 0.0f, 255.0f));
                convertedData[i * 4 + 3] = static_cast<unsigned char>(Mathf::Clamp(converted.m_alpha * 255.0f, 0.0f, 255.0f));
            }

            glTexImage2D((int)face, 0, (int)info.m_internalFormat, subWidth, subHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, convertedData.data());
        }
        else {
            glTexImage2D((int)face, 0, (int)info.m_internalFormat, subWidth, subHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawSubData.data());
        }
        };

    uploadFace(CubeMapFace::NEGATIVE_X, 0, subHeight);
    uploadFace(CubeMapFace::POSITIVE_X, subWidth * 2, subHeight);
    uploadFace(CubeMapFace::NEGATIVE_Y, subWidth, 0);
    uploadFace(CubeMapFace::POSITIVE_Y, subWidth, subHeight * 2);
    uploadFace(CubeMapFace::NEGATIVE_Z, subWidth * 3, subHeight);
    uploadFace(CubeMapFace::POSITIVE_Z, subWidth, subHeight);

    glTexParameteri((int)m_textureType, GL_TEXTURE_MIN_FILTER, (GLint)info.m_minificationFilter);
    glTexParameteri((int)m_textureType, GL_TEXTURE_MAG_FILTER, (GLint)info.m_magnificationFilter);
    glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_S, (GLint)info.m_wrapHorizontalParameter);
    glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_T, (GLint)info.m_wrapVerticalParameter);
    glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_R, (GLint)info.m_wrapDepthParameter);

    glTexParameteri((int)m_textureType, GL_TEXTURE_BASE_LEVEL, (GLint)info.m_mipmapBaseLevel);
    glTexParameteri((int)m_textureType, GL_TEXTURE_MAX_LEVEL, (GLint)info.m_mipmapMaxLevel);
    glTexParameteri((int)m_textureType, GL_TEXTURE_MIN_LOD, (GLint)info.m_mipmapMinLOD);
    glTexParameteri((int)m_textureType, GL_TEXTURE_MAX_LOD, (GLint)info.m_mipmapMaxLOD);
    glTexParameteri((int)m_textureType, GL_TEXTURE_LOD_BIAS, (GLint)info.m_mipmapLODBias);
    glGenerateMipmap((int)m_textureType);

    SetAnisotropy(info.m_anisotropyValue);
}
