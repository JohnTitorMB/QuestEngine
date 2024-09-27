#ifndef _RENDER_TEXTURE2D_H
#define _RENDER_TEXTURE2D_H
#include "Texture.h"
#include <unordered_map>
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


enum class BlitBitField
{
    COLOR_BIT = GL_COLOR_BUFFER_BIT,
    DEPTH_BIT = GL_DEPTH_BUFFER_BIT,
    STENCIL_BIT = GL_STENCIL_BUFFER_BIT,
};

enum class BlitFilter
{
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
};

class RenderTexture2D : public Texture
{
public : 
    struct RenderBufferInfo
    {
        bool m_useMultisampledRenderBuffer = false;
        int m_samples = 4;

        //Internal Value
        GLuint m_renderBufferID = 0;
        InternalRenderableFormat m_interRerableFormat = InternalRenderableFormat::DEPTH24_STENCIL8;
    };

private:
	GLuint m_framebufferID;
    std::vector<BufferAttachment> m_textureBuffers;
    std::unordered_map <BufferAttachment, RenderBufferInfo> m_renderBuffersInfo;

    std::vector<GLuint> m_backBuffer;


	void AttachTextureBuffer(BufferAttachment bufferAttachement, InternalRenderableFormat internalRenderableFormat, Format format, DataType dataTye, LayerTextureInfo layerTextureInfo = LayerTextureInfo());
	void AttachRenderBuffer(BufferAttachment bufferAttachement, InternalRenderableFormat internalRenderableFormat, RenderBufferInfo renderBufferInfo = RenderBufferInfo());

public:

 

	RenderTexture2D(int width, int height);
    ~RenderTexture2D();

    int GetFrameBufferID()const;
    void BindFramebuffer();
    void UnBindFramebuffer();

    void AttachColorTextureBuffer(ColorRenderableFormat colorRenderableFormat, ColorFormat colorFormat, DataType dataTye, unsigned int colorAttachementIndex, LayerTextureInfo layerTextureInfo = LayerTextureInfo());
    void AttachColorTextureBufferMS(ColorRenderableFormat colorRenderableFormat, unsigned int colorAttachementIndex, LayerTextureInfo layerTextureInfo = LayerTextureInfo());
    void AttachColorRenderBuffer(ColorRenderableFormat colorRenderableFormat, unsigned int colorAttachementIndex, RenderBufferInfo renderBufferInfo = RenderBufferInfo());

    void AttachDepthTextureBuffer(DepthRenderableFormat depthRenderableFormat, DataType dataTye, LayerTextureInfo layerTextureInfo = LayerTextureInfo());
    void AttachDepthTextureBufferMS(DepthRenderableFormat depthRenderableFormat, LayerTextureInfo layerTextureInfo = LayerTextureInfo());
    void AttachDepthRenderBuffer(DepthRenderableFormat depthRenderableFormat, RenderBufferInfo renderBufferInfo = RenderBufferInfo());
    
    void AttachStencilTextureBuffer(StencilRenderableFormat stencilRenderableFormat, DataType dataTye, LayerTextureInfo layerTextureInfo = LayerTextureInfo());
    void AttachStencilTextureBufferMS(StencilRenderableFormat stencilRenderableFormat, LayerTextureInfo layerTextureInfo = LayerTextureInfo());
    void AttachStencilRenderBuffer(StencilRenderableFormat stencilRenderableFormat, RenderBufferInfo renderBufferInfo = RenderBufferInfo());

    void AttachDepthStencilTextureBuffer(DepthStencilRenderableFormat depthStencilRenderableFormat, DataType dataTye, LayerTextureInfo layerTextureInfo = LayerTextureInfo());
    void AttachDepthStencilTextureBufferMS(DepthStencilRenderableFormat depthStencilRenderableFormat, LayerTextureInfo layerTextureInfo = LayerTextureInfo());
    void AttachDepthStencilRenderBuffer(DepthStencilRenderableFormat depthStencilRenderableFormat, RenderBufferInfo renderBufferInfo = RenderBufferInfo());
    void DetachBuffer(BufferAttachment bufferAttachment);

    void Resize(int newWidth, int newHeight);
    void SwapBuffer();
    
    void SetTexureMSSample(BufferAttachment bufferAttachement,int sample);
    void SetRenderBufferMSSample(BufferAttachment bufferAttachement,int sample);

    static void Blit(RenderTexture2D* rtA, RenderTexture2D* rtB, int srcX0, int srcY0, int srcX1, int srcY1, int dstX0, int dstY0, int dstX1, int dstY1, BlitBitField mask, BlitFilter filter);
};

#endif

