#ifndef _RENDER_TEXTURE2D_H
#define _RENDER_TEXTURE2D_H
#include "Texture.h"
#include <unordered_map>

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

enum class BufferAttachment
{
	ColorAttachment0 = GL_COLOR_ATTACHMENT0,
	ColorAttachment1 = GL_COLOR_ATTACHMENT1,
	ColorAttachment2 = GL_COLOR_ATTACHMENT2,
	ColorAttachment3 = GL_COLOR_ATTACHMENT3,
	ColorAttachment4 = GL_COLOR_ATTACHMENT4,
	ColorAttachment5 = GL_COLOR_ATTACHMENT5,
	ColorAttachment6 = GL_COLOR_ATTACHMENT6,
	ColorAttachment7 = GL_COLOR_ATTACHMENT7,
	ColorAttachment8 = GL_COLOR_ATTACHMENT8,
	ColorAttachment9 = GL_COLOR_ATTACHMENT9,
	ColorAttachment10 = GL_COLOR_ATTACHMENT10,
	ColorAttachment11 = GL_COLOR_ATTACHMENT11,
	ColorAttachment12 = GL_COLOR_ATTACHMENT12,
	ColorAttachment13 = GL_COLOR_ATTACHMENT13,
	ColorAttachment14 = GL_COLOR_ATTACHMENT14,
	ColorAttachment15 = GL_COLOR_ATTACHMENT15,
	ColorAttachment16 = GL_COLOR_ATTACHMENT16,
	ColorAttachment17 = GL_COLOR_ATTACHMENT17,
	ColorAttachment18 = GL_COLOR_ATTACHMENT18,
	ColorAttachment19 = GL_COLOR_ATTACHMENT19,
	ColorAttachment20 = GL_COLOR_ATTACHMENT20,
	ColorAttachment21 = GL_COLOR_ATTACHMENT21,
	ColorAttachment22 = GL_COLOR_ATTACHMENT22,
	ColorAttachment23 = GL_COLOR_ATTACHMENT23,
	ColorAttachment24 = GL_COLOR_ATTACHMENT24,
	ColorAttachment25 = GL_COLOR_ATTACHMENT25,
	ColorAttachment26 = GL_COLOR_ATTACHMENT26,
	ColorAttachment27 = GL_COLOR_ATTACHMENT27,
	ColorAttachment28 = GL_COLOR_ATTACHMENT28,
	ColorAttachment29 = GL_COLOR_ATTACHMENT29,
	ColorAttachment30 = GL_COLOR_ATTACHMENT30,
	ColorAttachment31 = GL_COLOR_ATTACHMENT31,
	DepthAttachment = GL_DEPTH_ATTACHMENT,
	StencilAttachment = GL_STENCIL_ATTACHMENT,
	Depth_Stencil_Attachment = GL_DEPTH_STENCIL_ATTACHMENT,
};

// General enum class for all renderable internal formats
enum class InternalRenderableFormat
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
    R8 = (int)InternalRenderableFormat::R8,
    R8_SNORM = (int)InternalRenderableFormat::R8_SNORM,
    R16 = (int)InternalRenderableFormat::R16,
    R16_SNORM = (int)InternalRenderableFormat::R16_SNORM,
    RG8 = (int)InternalRenderableFormat::RG8,
    RG8_SNORM = (int)InternalRenderableFormat::RG8_SNORM,
    RG16 = (int)InternalRenderableFormat::RG16,
    RG16_SNORM = (int)InternalRenderableFormat::RG16_SNORM,
    R3_G3_B2 = (int)InternalRenderableFormat::R3_G3_B2,
    RGB4 = (int)InternalRenderableFormat::RGB4,
    RGB5 = (int)InternalRenderableFormat::RGB5,
    RGB565 = (int)InternalRenderableFormat::RGB565,
    RGB8 = (int)InternalRenderableFormat::RGB8,
    RGB8_SNORM = (int)InternalRenderableFormat::RGB8_SNORM,
    RGB10 = (int)InternalRenderableFormat::RGB10,
    RGB12 = (int)InternalRenderableFormat::RGB12,
    RGB16 = (int)InternalRenderableFormat::RGB16,
    RGB16_SNORM = (int)InternalRenderableFormat::RGB16_SNORM,
    RGBA2 = (int)InternalRenderableFormat::RGBA2,
    RGBA4 = (int)InternalRenderableFormat::RGBA4,
    RGB5_A1 = (int)InternalRenderableFormat::RGB5_A1,
    RGBA8 = (int)InternalRenderableFormat::RGBA8,
    RGBA8_SNORM = (int)InternalRenderableFormat::RGBA8_SNORM,
    RGB10_A2 = (int)InternalRenderableFormat::RGB10_A2,
    RGB10_A2UI = (int)InternalRenderableFormat::RGB10_A2UI,
    RGBA12 = (int)InternalRenderableFormat::RGBA12,
    RGBA16 = (int)InternalRenderableFormat::RGBA16,
    RGBA16_SNORM = (int)InternalRenderableFormat::RGBA16_SNORM,
    SRGB8 = (int)InternalRenderableFormat::SRGB8,
    SRGB8_ALPHA8 = (int)InternalRenderableFormat::SRGB8_ALPHA8,
    R16F = (int)InternalRenderableFormat::R16F,
    RG16F = (int)InternalRenderableFormat::RG16F,
    RGB16F = (int)InternalRenderableFormat::RGB16F,
    RGBA16F = (int)InternalRenderableFormat::RGBA16F,
    R32F = (int)InternalRenderableFormat::R32F,
    RG32F = (int)InternalRenderableFormat::RG32F,
    RGB32F = (int)InternalRenderableFormat::RGB32F,
    RGBA32F = (int)InternalRenderableFormat::RGBA32F,
    R11F_G11F_B10F = (int)InternalRenderableFormat::R11F_G11F_B10F,
    R8I = (int)InternalRenderableFormat::R8I,
    R8UI = (int)InternalRenderableFormat::R8UI,
    R16I = (int)InternalRenderableFormat::R16I,
    R16UI = (int)InternalRenderableFormat::R16UI,
    R32I = (int)InternalRenderableFormat::R32I,
    R32UI = (int)InternalRenderableFormat::R32UI,
    RG8I = (int)InternalRenderableFormat::RG8I,
    RG8UI = (int)InternalRenderableFormat::RG8UI,
    RG16I = (int)InternalRenderableFormat::RG16I,
    RG16UI = (int)InternalRenderableFormat::RG16UI,
    RG32I = (int)InternalRenderableFormat::RG32I,
    RG32UI = (int)InternalRenderableFormat::RG32UI,
    RGB8I = (int)InternalRenderableFormat::RGB8I,
    RGB8UI = (int)InternalRenderableFormat::RGB8UI,
    RGB16I = (int)InternalRenderableFormat::RGB16I,
    RGB16UI = (int)InternalRenderableFormat::RGB16UI,
    RGB32I = (int)InternalRenderableFormat::RGB32I,
    RGB32UI = (int)InternalRenderableFormat::RGB32UI,
    RGBA8I = (int)InternalRenderableFormat::RGBA8I,
    RGBA8UI = (int)InternalRenderableFormat::RGBA8UI,
    RGBA16I = (int)InternalRenderableFormat::RGBA16I,
    RGBA16UI = (int)InternalRenderableFormat::RGBA16UI,
    RGBA32I = (int)InternalRenderableFormat::RGBA32I,
    RGBA32UI = (int)InternalRenderableFormat::RGBA32UI,
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
    DEPTH_COMPONENT = (int)InternalRenderableFormat::DEPTH_COMPONENT,
    DEPTH_COMPONENT16 = (int)InternalRenderableFormat::DEPTH_COMPONENT16,
    DEPTH_COMPONENT24 = (int)InternalRenderableFormat::DEPTH_COMPONENT24,
    DEPTH_COMPONENT32 = (int)InternalRenderableFormat::DEPTH_COMPONENT32,
    DEPTH_COMPONENT32F = (int)InternalRenderableFormat::DEPTH_COMPONENT32F,
};

// Enum class for Internal stencil-renderable formats
enum class StencilRenderableFormat
{
    STENCIL_INDEX1 = (int)InternalRenderableFormat::STENCIL_INDEX1,
    STENCIL_INDEX4 = (int)InternalRenderableFormat::STENCIL_INDEX4,
    STENCIL_INDEX8 = (int)InternalRenderableFormat::STENCIL_INDEX8,
    STENCIL_INDEX16 = (int)InternalRenderableFormat::STENCIL_INDEX16,
};

// Enum class for combined Depth + Stencil Internalformats
enum class DepthStencilRenderableFormat
{
    DEPTH24_STENCIL8 = (int)InternalRenderableFormat::DEPTH24_STENCIL8,
    DEPTH32F_STENCIL8 = (int)InternalRenderableFormat::DEPTH32F_STENCIL8
};

class RenderTexture2D : public Texture
{
	GLuint m_framebufferID;
    std::vector<BufferAttachment> m_textureBuffers;
    std::unordered_map <BufferAttachment, GLuint> m_renderBufferID;

    std::vector<GLuint> m_backBuffer;


	void AttachBuffer(BufferAttachment bufferAttachement, InternalRenderableFormat InternalRenderableFormat, Format format, DataType dataTye, bool useRenderBuffer, LayerTextureInfo LayerTextureInfo = LayerTextureInfo(), bool enableDoubleBuffering = false);

public:
	RenderTexture2D(int width, int height);
    ~RenderTexture2D();

    void BindFramebuffer();
    void UnBindFramebuffer();
    void AttachColorBuffer(ColorRenderableFormat colorRenderableFormat, ColorFormat colorFormat, DataType dataTye, bool useRenderBuffer, unsigned int colorAttachementIndex);
    void AttachColorBuffer(ColorRenderableFormat colorRenderableFormat, ColorFormat colorFormat, DataType dataTye, unsigned int colorAttachementIndex, LayerTextureInfo layerTextureInfo = LayerTextureInfo(), bool enableDoubleBuffering = false);
	void AttachDepthBuffer(DepthRenderableFormat depthRenderableFormat, DataType dataTye, bool useRenderBuffer);
	void AttachDepthBuffer(DepthRenderableFormat depthRenderableFormat, DataType dataTye, LayerTextureInfo layerTextureInfo = LayerTextureInfo(), bool enableDoubleBuffering = false);
    void AttachStencilBuffer(StencilRenderableFormat stencilRenderableFormat, DataType dataTye, bool useRenderBuffer);
    void AttachStencilBuffer(StencilRenderableFormat stencilRenderableFormat, DataType dataTye, LayerTextureInfo layerTextureInfo = LayerTextureInfo(), bool enableDoubleBuffering = false);
	void AttachDepthStencilBuffer(DepthStencilRenderableFormat depthStencilRenderableFormat, DataType dataTye, bool useRenderBuffer);
	void AttachDepthStencilBuffer(DepthStencilRenderableFormat depthStencilRenderableFormat, DataType dataTye, LayerTextureInfo layerTextureInfo = LayerTextureInfo(), bool enableDoubleBuffering = false);
    void DetachBuffer(BufferAttachment bufferAttchement);

    void SwapBuffer();
};

#endif

