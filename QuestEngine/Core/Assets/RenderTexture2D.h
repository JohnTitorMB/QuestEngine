#ifndef _RENDER_TEXTURE2D_H
#define _RENDER_TEXTURE2D_H
#include "RenderTexture.h"


class RenderTexture2D : public RenderTexture
{
public : 
    struct RenderBufferInfo
    {
        bool m_useMultisampledRenderBuffer = false;
        int m_samples = 4;

        //Internal Value
        GLuint m_renderBufferID = 0;
        InternalFormat m_interRerableFormat = InternalFormat::DEPTH24_STENCIL8;
    };

private:
    std::unordered_map <BufferAttachment, RenderBufferInfo> m_renderBuffersInfo;

	void AttachTextureBuffer(BufferAttachment bufferAttachement, InternalFormat internalRenderableFormat, Format format, DataType dataTye, LayerTextureInfo layerTextureInfo = LayerTextureInfo())override;
	void AttachRenderBuffer(BufferAttachment bufferAttachement, InternalFormat internalRenderableFormat, RenderBufferInfo renderBufferInfo = RenderBufferInfo());

public:

 

	RenderTexture2D(int width, int height);
    ~RenderTexture2D();

    void AttachColorTextureBufferMS(ColorRenderableFormat colorRenderableFormat, unsigned int colorAttachementIndex, LayerTextureInfo layerTextureInfo = LayerTextureInfo());
    void AttachColorRenderBuffer(ColorRenderableFormat colorRenderableFormat, unsigned int colorAttachementIndex, RenderBufferInfo renderBufferInfo = RenderBufferInfo());

    void AttachDepthTextureBufferMS(DepthRenderableFormat depthRenderableFormat, LayerTextureInfo layerTextureInfo = LayerTextureInfo());
    void AttachDepthRenderBuffer(DepthRenderableFormat depthRenderableFormat, RenderBufferInfo renderBufferInfo = RenderBufferInfo());
    
    void AttachStencilTextureBufferMS(StencilRenderableFormat stencilRenderableFormat, LayerTextureInfo layerTextureInfo = LayerTextureInfo());
    void AttachStencilRenderBuffer(StencilRenderableFormat stencilRenderableFormat, RenderBufferInfo renderBufferInfo = RenderBufferInfo());

    void AttachDepthStencilTextureBufferMS(DepthStencilRenderableFormat depthStencilRenderableFormat, LayerTextureInfo layerTextureInfo = LayerTextureInfo());
    void AttachDepthStencilRenderBuffer(DepthStencilRenderableFormat depthStencilRenderableFormat, RenderBufferInfo renderBufferInfo = RenderBufferInfo());
    void DetachBuffer(BufferAttachment bufferAttachment);

    void Resize(int newWidth, int newHeight)override;
    void SwapBuffer()override;
    
    void SetTexureMSSample(BufferAttachment bufferAttachement,int sample);
    void SetRenderBufferMSSample(BufferAttachment bufferAttachement,int sample);

    void DrawBuffer(DrawBufferType drawBuffer);
    void ReadBuffer(ReadBufferMode mode);
    static void Blit(RenderTexture2D* rtA, RenderTexture2D* rtB, int srcX0, int srcY0, int srcX1, int srcY1, int dstX0, int dstY0, int dstX1, int dstY1, BlitBitField mask, BlitFilter filter);
};

#endif

