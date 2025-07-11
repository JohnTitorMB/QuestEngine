#ifndef _RENDER_CUBEMAP_H
#define _RENDER_CUBEMAP_H
#include "Texture.h"
#include "RenderTexture.h"
#include <unordered_map>

class RenderCubeMap : public RenderTexture
{

private:
    int m_currentFaceIndex = 0;

    void AttachTextureBuffer(BufferAttachment bufferAttachement, InternalFormat internalRenderableFormat, Format format, DataType dataTye, LayerTextureInfo layerTextureInfo = LayerTextureInfo())override;

public:

    RenderCubeMap(int width, int height);
    ~RenderCubeMap();

    void DetachBuffer(BufferAttachment bufferAttachment)override;

    void Resize(int newWidth, int newHeight)override;
    void SwapBuffer()override;

    void AttachFaceToFramebuffer(BufferAttachment attachment, int faceIndex);

    static void Blit(RenderCubeMap* rtA, RenderCubeMap* rtB, int srcX0, int srcY0, int srcX1, int srcY1, int dstX0, int dstY0, int dstX1, int dstY1, BlitBitField mask, BlitFilter filter);
    static void Blit(RenderCubeMap* rtA, RenderCubeMap* rtB, bool blitAllFaces, int srcX0, int srcY0, int srcX1, int srcY1, int dstX0, int dstY0, int dstX1, int dstY1, BlitBitField mask, BlitFilter filter);
};

#endif

