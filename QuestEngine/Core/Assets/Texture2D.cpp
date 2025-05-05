#include "Texture2D.h"
#include "../ColorManagement/Color.h"
#include "../ColorManagement/RGBModel.h"
#include "../ColorManagement/ColorConversion.h"
#include "../../Math/Mathf.h"

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

void Texture2D::SetTextureInternalFormat(InternalFormat internalFormat)
{
	m_layerTextureInfos[0].m_internalFormat = internalFormat;
	RefreshTextureData(0);
}

void Texture2D::RefreshTextureData(int layer)
{
	unsigned char* data = LoadTexture(m_path);

	if (data)
	{
		UpdateTextureData(data);
		stbi_image_free(data);
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
	glTexImage2D((int)m_textureType, level, (int)m_layerTextureInfos[0].m_internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

void Texture2D::UpdateTextureData(const unsigned char* data)
{

    auto& info = m_layerTextureInfos[0];

    glBindTexture((int)m_textureType, info.m_textureID);

    // --- CPU Conversion ---
    if (info.m_textureConversionMode == TextureConversionMode::CPUConvert)
    {
        std::vector<unsigned char> convertedData(m_width * m_height * 4);

        auto srcColorSpace = ColorManagement::GetRGBColorSpaceFromType(info.m_colorSpace);
        auto dstColorSpace = ColorManagement::Instance().GetGPUWorkingSpace();

        for (int i = 0; i < m_width * m_height; ++i)
        {
            std::unique_ptr<ColorModel> model = std::make_unique<RGBModel>(
                data[i * 4 + 0] / 255.0f,
                data[i * 4 + 1] / 255.0f,
                data[i * 4 + 2] / 255.0f
            );

            Color color(std::move(model), srcColorSpace, data[i * 4 + 3] / 255.0f);
            Color converted = ColorConversion::Convert(color, ColorModelType::RGB, dstColorSpace);
            const RGBModel& rgb = static_cast<const RGBModel&>(converted.GetColorModel());

            convertedData[i * 4 + 0] = static_cast<unsigned char>(Mathf::Clamp(rgb.m_r * 255.0f, 0.0f, 255.0f));
            convertedData[i * 4 + 1] = static_cast<unsigned char>(Mathf::Clamp(rgb.m_g * 255.0f, 0.0f, 255.0f));
            convertedData[i * 4 + 2] = static_cast<unsigned char>(Mathf::Clamp(rgb.m_b * 255.0f, 0.0f, 255.0f));
            convertedData[i * 4 + 3] = static_cast<unsigned char>(Mathf::Clamp(converted.m_alpha * 255.0f, 0.0f, 255.0f));
        }

        glTexImage2D((int)m_textureType, 0, (int)info.m_internalFormat, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, convertedData.data());
    }
    else
    {
        // No conversion
        glTexImage2D((int)m_textureType, 0, (int)info.m_internalFormat, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    // --- Texture parameters ---
    glTexParameteri((int)m_textureType, GL_TEXTURE_MIN_FILTER, (GLint)info.m_minificationFilter);
    glTexParameteri((int)m_textureType, GL_TEXTURE_MAG_FILTER, (GLint)info.m_magnificationFilter);
    glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_S, (GLint)info.m_wrapHorizontalParameter);
    glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_T, (GLint)info.m_wrapVerticalParameter);

    glTexParameteri((int)m_textureType, GL_TEXTURE_BASE_LEVEL, (GLint)info.m_mipmapBaseLevel);
    glTexParameteri((int)m_textureType, GL_TEXTURE_MAX_LEVEL, (GLint)info.m_mipmapMaxLevel);
    glTexParameteri((int)m_textureType, GL_TEXTURE_MIN_LOD, (GLint)info.m_mipmapMinLOD);
    glTexParameteri((int)m_textureType, GL_TEXTURE_MAX_LOD, (GLint)info.m_mipmapMaxLOD);
    glTexParameteri((int)m_textureType, GL_TEXTURE_LOD_BIAS, (GLint)info.m_mipmapLODBias);
    glGenerateMipmap((int)m_textureType);

    SetAnisotropy(info.m_anisotropyValue);

}
