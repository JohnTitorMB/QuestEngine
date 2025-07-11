#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include <string>
#include <glad/glad.h>
#include "Assets.h"
#include "../../Library/stb_image.h"
#include "../ColorManagement/ColorManagement.h"
#include "../../Math/Vector4D.h"

#include <vector>

enum class TextureType
{
	TEXTURE2D = GL_TEXTURE_2D,
	CUBEMAP = GL_TEXTURE_CUBE_MAP,
};

enum class Wrap
{
	Repeat = GL_REPEAT,
	ClampToEdge = GL_CLAMP_TO_EDGE,
	ClampToBorder = GL_CLAMP_TO_BORDER,
	MirrorRepeat = GL_MIRRORED_REPEAT,
	MirrorClampToEdge = 0x8743,
};

enum class MinificationFilter
{
	Point = GL_NEAREST,
	Bilinear = GL_LINEAR,
	PointMipmapPoint = GL_NEAREST_MIPMAP_NEAREST,
	PointMipmapBilinear = GL_NEAREST_MIPMAP_LINEAR,
	BilinearMipmapPoint = GL_LINEAR_MIPMAP_NEAREST,
	Trilinear = GL_LINEAR_MIPMAP_LINEAR,
};

enum class MagnificationFilter
{
	Point = GL_NEAREST,
	Bilinear = GL_LINEAR,
};

enum class DataType
{
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    BYTE = GL_BYTE,
    UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
    SHORT = GL_SHORT,
    UNSIGNED_INT = GL_UNSIGNED_INT,
    INT = GL_INT,
    HALF_FLOAT = GL_HALF_FLOAT,
    FLOAT = GL_FLOAT,
    UNSIGNED_BYTE_3_3_2 = GL_UNSIGNED_BYTE_3_3_2,
    UNSIGNED_BYTE_2_3_3_REV = GL_UNSIGNED_BYTE_2_3_3_REV,
    UNSIGNED_SHORT_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
    UNSIGNED_SHORT_5_6_5_REV = GL_UNSIGNED_SHORT_5_6_5_REV,
    UNSIGNED_SHORT_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
    UNSIGNED_SHORT_4_4_4_4_REV = GL_UNSIGNED_SHORT_4_4_4_4_REV,
    UNSIGNED_SHORT_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
    UNSIGNED_SHORT_1_5_5_5_REV = GL_UNSIGNED_SHORT_1_5_5_5_REV,
    UNSIGNED_INT_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8,
    UNSIGNED_INT_8_8_8_8_REV = GL_UNSIGNED_INT_8_8_8_8_REV,
    UNSIGNED_INT_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
    UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,
    UNSIGNED_INT_24_8 = GL_UNSIGNED_INT_24_8,
    UNSIGNED_INT_10F_11F_11F_REV = GL_UNSIGNED_INT_10F_11F_11F_REV,
    UNSIGNED_INT_5_9_9_9_REV = GL_UNSIGNED_INT_5_9_9_9_REV,
    FLOAT_32_UNSIGNED_INT_24_8_REV = GL_FLOAT_32_UNSIGNED_INT_24_8_REV,
};


// General enum class for all renderable internal formats
enum class InternalFormat
{
    // Internal color-renderable formats
    R8 = GL_R8,
    R8_SNORM = GL_R8_SNORM,
    R16 = GL_R16,
    R16_SNORM = GL_R16_SNORM,
    RG8 = GL_RG8,
    RG8_SNORM = GL_RG8_SNORM,
    RG16 = GL_RG16,
    RG16_SNORM = GL_RG16_SNORM,
    R3_G3_B2 = GL_R3_G3_B2,
    RGB4 = GL_RGB4,
    RGB5 = GL_RGB5,
    RGB565 = GL_RGB565,
    RGB8 = GL_RGB8,
    RGB8_SNORM = GL_RGB8_SNORM,
    RGB10 = GL_RGB10,
    RGB12 = GL_RGB12,
    RGB16 = GL_RGB16,
    RGB16_SNORM = GL_RGB16_SNORM,
    RGBA2 = GL_RGBA2,
    RGBA4 = GL_RGBA4,
    RGB5_A1 = GL_RGB5_A1,
    RGBA8 = GL_RGBA8,
    RGBA8_SNORM = GL_RGBA8_SNORM,
    RGB10_A2 = GL_RGB10_A2,
    RGB10_A2UI = GL_RGB10_A2UI,
    RGBA12 = GL_RGBA12,
    RGBA16 = GL_RGBA16,
    RGBA16_SNORM = GL_RGBA16_SNORM,
    SRGB8 = GL_SRGB8,
    SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,
    R16F = GL_R16F,
    RG16F = GL_RG16F,
    RGB16F = GL_RGB16F,
    RGBA16F = GL_RGBA16F,
    R32F = GL_R32F,
    RG32F = GL_RG32F,
    RGB32F = GL_RGB32F,
    RGBA32F = GL_RGBA32F,
    R11F_G11F_B10F = GL_R11F_G11F_B10F,
    R8I = GL_R8I,
    R8UI = GL_R8UI,
    R16I = GL_R16I,
    R16UI = GL_R16UI,
    R32I = GL_R32I,
    R32UI = GL_R32UI,
    RG8I = GL_RG8I,
    RG8UI = GL_RG8UI,
    RG16I = GL_RG16I,
    RG16UI = GL_RG16UI,
    RG32I = GL_RG32I,
    RG32UI = GL_RG32UI,
    RGB8I = GL_RGB8I,
    RGB8UI = GL_RGB8UI,
    RGB16I = GL_RGB16I,
    RGB16UI = GL_RGB16UI,
    RGB32I = GL_RGB32I,
    RGB32UI = GL_RGB32UI,
    RGBA8I = GL_RGBA8I,
    RGBA8UI = GL_RGBA8UI,
    RGBA16I = GL_RGBA16I,
    RGBA16UI = GL_RGBA16UI,
    RGBA32I = GL_RGBA32I,
    RGBA32UI = GL_RGBA32UI,

    // Internal depth-renderable formats
    DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
    DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
    DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
    DEPTH_COMPONENT32 = GL_DEPTH_COMPONENT32,
    DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,

    // Internal stencil-renderable formats
    STENCIL_INDEX1 = GL_STENCIL_INDEX1,
    STENCIL_INDEX4 = GL_STENCIL_INDEX4,
    STENCIL_INDEX8 = GL_STENCIL_INDEX8,
    STENCIL_INDEX16 = GL_STENCIL_INDEX16,

    // Combined formats (Depth + Stencil)
    DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
    DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8
};

// Enum class for Internal color-renderable formats
enum class ColorRenderableFormat
{
    R8 = (int)InternalFormat::R8,
    R8_SNORM = (int)InternalFormat::R8_SNORM,
    R16 = (int)InternalFormat::R16,
    R16_SNORM = (int)InternalFormat::R16_SNORM,
    RG8 = (int)InternalFormat::RG8,
    RG8_SNORM = (int)InternalFormat::RG8_SNORM,
    RG16 = (int)InternalFormat::RG16,
    RG16_SNORM = (int)InternalFormat::RG16_SNORM,
    R3_G3_B2 = (int)InternalFormat::R3_G3_B2,
    RGB4 = (int)InternalFormat::RGB4,
    RGB5 = (int)InternalFormat::RGB5,
    RGB565 = (int)InternalFormat::RGB565,
    RGB8 = (int)InternalFormat::RGB8,
    RGB8_SNORM = (int)InternalFormat::RGB8_SNORM,
    RGB10 = (int)InternalFormat::RGB10,
    RGB12 = (int)InternalFormat::RGB12,
    RGB16 = (int)InternalFormat::RGB16,
    RGB16_SNORM = (int)InternalFormat::RGB16_SNORM,
    RGBA2 = (int)InternalFormat::RGBA2,
    RGBA4 = (int)InternalFormat::RGBA4,
    RGB5_A1 = (int)InternalFormat::RGB5_A1,
    RGBA8 = (int)InternalFormat::RGBA8,
    RGBA8_SNORM = (int)InternalFormat::RGBA8_SNORM,
    RGB10_A2 = (int)InternalFormat::RGB10_A2,
    RGB10_A2UI = (int)InternalFormat::RGB10_A2UI,
    RGBA12 = (int)InternalFormat::RGBA12,
    RGBA16 = (int)InternalFormat::RGBA16,
    RGBA16_SNORM = (int)InternalFormat::RGBA16_SNORM,
    SRGB8 = (int)InternalFormat::SRGB8,
    SRGB8_ALPHA8 = (int)InternalFormat::SRGB8_ALPHA8,
    R16F = (int)InternalFormat::R16F,
    RG16F = (int)InternalFormat::RG16F,
    RGB16F = (int)InternalFormat::RGB16F,
    RGBA16F = (int)InternalFormat::RGBA16F,
    R32F = (int)InternalFormat::R32F,
    RG32F = (int)InternalFormat::RG32F,
    RGB32F = (int)InternalFormat::RGB32F,
    RGBA32F = (int)InternalFormat::RGBA32F,
    R11F_G11F_B10F = (int)InternalFormat::R11F_G11F_B10F,
    R8I = (int)InternalFormat::R8I,
    R8UI = (int)InternalFormat::R8UI,
    R16I = (int)InternalFormat::R16I,
    R16UI = (int)InternalFormat::R16UI,
    R32I = (int)InternalFormat::R32I,
    R32UI = (int)InternalFormat::R32UI,
    RG8I = (int)InternalFormat::RG8I,
    RG8UI = (int)InternalFormat::RG8UI,
    RG16I = (int)InternalFormat::RG16I,
    RG16UI = (int)InternalFormat::RG16UI,
    RG32I = (int)InternalFormat::RG32I,
    RG32UI = (int)InternalFormat::RG32UI,
    RGB8I = (int)InternalFormat::RGB8I,
    RGB8UI = (int)InternalFormat::RGB8UI,
    RGB16I = (int)InternalFormat::RGB16I,
    RGB16UI = (int)InternalFormat::RGB16UI,
    RGB32I = (int)InternalFormat::RGB32I,
    RGB32UI = (int)InternalFormat::RGB32UI,
    RGBA8I = (int)InternalFormat::RGBA8I,
    RGBA8UI = (int)InternalFormat::RGBA8UI,
    RGBA16I = (int)InternalFormat::RGBA16I,
    RGBA16UI = (int)InternalFormat::RGBA16UI,
    RGBA32I = (int)InternalFormat::RGBA32I,
    RGBA32UI = (int)InternalFormat::RGBA32UI,
};

//Enum class for formats
enum class Format
{
    RED = GL_RED,
    GREEN = GL_GREEN,
    BLUE = GL_BLUE,
    RG = GL_RG,
    RGB = GL_RGB,
    RGBA = GL_RGBA,
    BGR = GL_BGR,
    BGRA = GL_BGRA,
    RED_INTEGER = GL_RED_INTEGER,
    GREEN_INTEGER = GL_GREEN_INTEGER,
    BLUE_INTEGER = GL_BLUE_INTEGER,
    RG_INTEGER = GL_RG_INTEGER,
    RGB_INTEGER = GL_RGB_INTEGER,
    RGBA_INTEGER = GL_RGBA_INTEGER,
    BGR_INTEGER = GL_BGR_INTEGER,
    BGRA_INTEGER = GL_BGRA_INTEGER,
    STENCIL_INDEX = GL_STENCIL_INDEX,
    DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
    DEPTH_STENCIL = GL_DEPTH_STENCIL,
};

//Enum class for color formats
enum class ColorFormat
{
    RED = (int)Format::RED,
    GREEN = (int)Format::GREEN,
    BLUE = (int)Format::BLUE,
    RG = (int)Format::RG,
    RGB = (int)Format::RGB,
    RGBA = (int)Format::RGBA,
    BGR = (int)Format::BGR,
    BGRA = (int)Format::BGRA,
    RED_Integer = (int)Format::RED_INTEGER,
    GREEN_Integer = (int)Format::GREEN_INTEGER,
    BLUE_Integer = (int)Format::BLUE_INTEGER,
    RG_Integer = (int)Format::RG_INTEGER,
    RGB_Integer = (int)Format::RGB_INTEGER,
    RGBA_Integer = (int)Format::RGBA_INTEGER,
    BGR_Integer = (int)Format::BGR_INTEGER,
    BGRA_Integer = (int)Format::BGRA_INTEGER,
};

// Enum class Internal depth-renderable formats
enum class DepthRenderableFormat
{
    DEPTH_COMPONENT = (int)InternalFormat::DEPTH_COMPONENT,
    DEPTH_COMPONENT16 = (int)InternalFormat::DEPTH_COMPONENT16,
    DEPTH_COMPONENT24 = (int)InternalFormat::DEPTH_COMPONENT24,
    DEPTH_COMPONENT32 = (int)InternalFormat::DEPTH_COMPONENT32,
    DEPTH_COMPONENT32F = (int)InternalFormat::DEPTH_COMPONENT32F,
};

// Enum class for Internal stencil-renderable formats
enum class StencilRenderableFormat
{
    STENCIL_INDEX1 = (int)InternalFormat::STENCIL_INDEX1,
    STENCIL_INDEX4 = (int)InternalFormat::STENCIL_INDEX4,
    STENCIL_INDEX8 = (int)InternalFormat::STENCIL_INDEX8,
    STENCIL_INDEX16 = (int)InternalFormat::STENCIL_INDEX16,
};

// Enum class for combined Depth + Stencil Internalformats
enum class DepthStencilRenderableFormat
{
    DEPTH24_STENCIL8 = (int)InternalFormat::DEPTH24_STENCIL8,
    DEPTH32F_STENCIL8 = (int)InternalFormat::DEPTH32F_STENCIL8
};

class Texture : public Assets
{
public:

    enum class TextureConversionMode {
        None,       
        CPUConvert, 
        GPUConvert 
    };


	struct LayerTextureInfo
	{
		GLuint m_textureID = 0;
		Wrap m_wrapHorizontalParameter = Wrap::Repeat;
		Wrap m_wrapVerticalParameter = Wrap::Repeat;
		Wrap m_wrapDepthParameter = Wrap::Repeat;
		MagnificationFilter m_magnificationFilter = MagnificationFilter::Bilinear;
		MinificationFilter m_minificationFilter = MinificationFilter::Trilinear;

		int m_mipmapBaseLevel = 0;
		int m_mipmapMaxLevel = 1000;
		int m_mipmapMinLOD = -1000;
		int m_mipmapMaxLOD = 1000;
		int m_mipmapLODBias = 0;
		float m_anisotropyValue = 16;
		bool m_generateMimpap = true;

        bool m_useMultisampledTexture = false;
        int m_samples = 4;
        bool m_useFixedsamplelocations = true;
        bool m_enableDoubleBuffering = false;

        DataType m_dataType = (DataType)0;
        Format m_format = (Format)0;
        InternalFormat m_internalFormat = InternalFormat::RGBA8;
        Vector4D m_borderColor = Vector4D(1, 1, 1, 1);
        TextureConversionMode m_textureConversionMode = TextureConversionMode::GPUConvert;
        ColorManagement::RGBColorSpaceType m_colorSpace = ColorManagement::RGBColorSpaceType::SRGB;
    };
    int GetTextureLayerType(int layer = 0);

public:

	Texture();
	Texture(std::string filePath);
	virtual ~Texture();

	void Bind(int textureIndex, int layer = 0);
	void Unbind(int textureIndex, int layer = 0);

	void SetWrapHorizontalParameter(Wrap wrapHorizontalParameter, int layer = 0);
	void SetWrapVerticalParameter(Wrap wrapVerticalParameter, int layer = 0);
	void SetWrapDepthParameter(Wrap wrapVerticalParameter, int layer = 0);
	void GenerateMipmap(int layer = 0);
	void GenerateAllMipmap(bool forceGeneration = true);

	Wrap GetHorizontalParameter(int layer = 0)const;
	Wrap GetVerticalParameter(int layer = 0)const;
	Wrap GetDepthParameter(int layer = 0)const;

	void SetMinification(MinificationFilter minificationFilter, int layer = 0);
	void SetMagnification(MagnificationFilter magnificationFilter, int layer = 0);

    void SetTextureConversionMode(TextureConversionMode textureConversionMode, int layer = 0);
    void SetTextureColorSpace(ColorManagement::RGBColorSpaceType colorSpace, int layer = 0);
    void SetTextureColorManagementParam(ColorManagement::RGBColorSpaceType colorSpace, TextureConversionMode textureConversionMode, int layer = 0);

    void SetBorderColor(Vector4D color, int layer = 0);

    virtual void RefreshTextureData(int layer);

	MinificationFilter GetMinification(int layer = 0)const;
	MagnificationFilter GetMagnification(int layer = 0)const;

	void SetMipmapBaseLevel(int mipmapBaseLevel, int layer = 0);
	void SetMipmapMaxLevel(int mipmapMaxLevel, int layer = 0);
	void SetMipmapMinLOD(int mipmapMinLOD, int layer = 0);
	void SetMipmapMaxLOD(int mipmapMaxLOD, int layer = 0);
	void SetMipmapLODBias(int mipmapLODBias, int layer = 0);
    static bool SaveTextureToPNG(GLuint textureID, int width, int height, const std::string& filename);

	int GetMipmapBaseLevel(int layer = 0)const;
	int GetMipmapMaxLevel(int layer = 0)const;
	int GetMipmapMinLOD(int layer = 0)const;
	int GetMipmapMaxLOD(int layer = 0)const;
	int GetMipmapLODBIAS(int layer = 0)const;

	void SetAnisotropy(float anisotropyValue, int layer = 0);
	float GetAnisotropy(int layer = 0)const;
	float GetMaxGPUAnisotropy(int layer = 0)const;
	float GetWidth()const;
	float GetHeight()const;
	int GetSubLayerCount()const;
    const LayerTextureInfo GetTextureLayerInfo(int layer = 0)const;
	std::string m_path;

protected:
	unsigned char* LoadTexture(std::string filePath);
	
	int m_width = 0;
	int m_height = 0;

	std::vector<LayerTextureInfo> m_layerTextureInfos;
	TextureType m_textureType = TextureType::TEXTURE2D;

	static std::vector<unsigned char> GetSubData(const unsigned char* data, int srcWidth, int srcHeight, int x, int y, int subWidth, int subHeight, bool inversedX = false, bool inversedY = false);


};

#endif // !_TEXTURE_H_

