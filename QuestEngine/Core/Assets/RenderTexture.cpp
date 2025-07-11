#include "RenderTexture.h"
#include <iostream>



RenderTexture::RenderTexture(int width, int height)
{
	m_width = width;
	m_height = height;
	glGenFramebuffers(1, &m_framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "framebuffer not properly initialized : " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	else
		std::cout << "framebuffer is initialized" << std::endl;
}

RenderTexture::~RenderTexture()
{
	glDeleteFramebuffers(1, &m_framebufferID);

	for (const GLuint& id : m_backBuffer)
	{
		if (id != 0)
			glDeleteTextures(1, &id);
	}

	m_backBuffer.clear();
}

int RenderTexture::GetFrameBufferID()const
{
	return m_framebufferID;
}

void RenderTexture::BindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
}

void RenderTexture::UnBindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::DetachBuffer(BufferAttachment bufferAttachment)
{
}

void RenderTexture::Resize(int newWidth, int newHeight)
{
}

void RenderTexture::SwapBuffer()
{
}

void RenderTexture::AttachColorTextureBuffer(ColorRenderableFormat colorRenderableFormat, ColorFormat colorFormat, DataType dataTye, unsigned int colorAttachementIndex, LayerTextureInfo layerTextureInfo)
{
	int bufferAttachmentIndex = (int)BufferAttachment::ColorAttachment0 + colorAttachementIndex;
	if (bufferAttachmentIndex > (int)BufferAttachment::ColorAttachment31)
	{
		std::cout << "RenderTexture cannot attach a buffer to a color index greater than 31" << std::endl;
		return;
	}

	AttachTextureBuffer((BufferAttachment)bufferAttachmentIndex, (InternalFormat)colorRenderableFormat, (Format)colorFormat, dataTye, layerTextureInfo);
}

void RenderTexture::AttachDepthTextureBuffer(DepthRenderableFormat depthRenderableFormat, DataType dataTye, LayerTextureInfo layerTextureInfo)
{
	AttachTextureBuffer(BufferAttachment::DepthAttachment, (InternalFormat)depthRenderableFormat, Format::DEPTH_COMPONENT, dataTye, layerTextureInfo);
}

void RenderTexture::AttachStencilTextureBuffer(StencilRenderableFormat stencilRenderableFormat, DataType dataTye, LayerTextureInfo layerTextureInfo)
{
	AttachTextureBuffer(BufferAttachment::StencilAttachment, (InternalFormat)stencilRenderableFormat, Format::STENCIL_INDEX, dataTye, layerTextureInfo);
}

void RenderTexture::AttachDepthStencilTextureBuffer(DepthStencilRenderableFormat depthStencilRenderableFormat, DataType dataTye, LayerTextureInfo layerTextureInfo)
{
	AttachTextureBuffer(BufferAttachment::Depth_Stencil_Attachment, (InternalFormat)depthStencilRenderableFormat, Format::DEPTH_STENCIL, dataTye, layerTextureInfo);
}

void RenderTexture::AttachTextureBuffer(BufferAttachment bufferAttachement, InternalFormat internalRenderableFormat, Format format, DataType dataTye, LayerTextureInfo layerTextureInfo)
{
}