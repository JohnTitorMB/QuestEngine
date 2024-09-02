#include "RenderTexture2D.h"
#include <iostream>

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

RenderTexture2D::RenderTexture2D(int width, int height) : Texture()
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

RenderTexture2D::~RenderTexture2D()
{
	for (const auto& info : m_renderBufferID)
		glDeleteRenderbuffers(1, &info.second);

	glDeleteFramebuffers(1, &m_framebufferID);

	for (const GLuint& id : m_backBuffer)
	{
		if(id != 0)
			glDeleteTextures(1, &id);
	}

	m_renderBufferID.clear();
	m_backBuffer.clear();
}

void RenderTexture2D::BindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
}

void RenderTexture2D::UnBindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture2D::AttachColorBuffer(ColorRenderableFormat colorRenderableFormat, ColorFormat colorFormat, DataType dataTye, bool useRenderBuffer, unsigned int colorAttachementIndex)
{
	int bufferAttachmentIndex = (int)BufferAttachment::ColorAttachment0 + colorAttachementIndex;
	if (bufferAttachmentIndex > (int)BufferAttachment::ColorAttachment31)
	{
		std::cout << "RenderTexture cannot attach a buffer to a color index greater than 31" << std::endl;
		return;
	}

	AttachBuffer((BufferAttachment)bufferAttachmentIndex, (InternalRenderableFormat)colorRenderableFormat, (Format)colorFormat, dataTye, useRenderBuffer);
}

void RenderTexture2D::AttachColorBuffer(ColorRenderableFormat colorRenderableFormat, ColorFormat colorFormat, DataType dataTye, unsigned int colorAttachementIndex, LayerTextureInfo layerTextureInfo, bool enableDoubleBuffering)
{
	int bufferAttachmentIndex = (int)BufferAttachment::ColorAttachment0 + colorAttachementIndex;
	if (bufferAttachmentIndex > (int)BufferAttachment::ColorAttachment31)
	{
		std::cout << "RenderTexture cannot attach a buffer to a color index greater than 31" << std::endl;
		return;
	}

	AttachBuffer((BufferAttachment)bufferAttachmentIndex, (InternalRenderableFormat)colorRenderableFormat, (Format)colorFormat, dataTye, false, layerTextureInfo, enableDoubleBuffering);
}

void RenderTexture2D::AttachDepthBuffer(DepthRenderableFormat depthRenderableFormat, DataType dataTye, bool useRenderBuffer)
{
	AttachBuffer(BufferAttachment::DepthAttachment, (InternalRenderableFormat)depthRenderableFormat, Format::DEPTH_COMPONENT, dataTye, useRenderBuffer);
}

void RenderTexture2D::AttachDepthBuffer(DepthRenderableFormat depthRenderableFormat, DataType dataTye, LayerTextureInfo layerTextureInfo, bool enableDoubleBuffering)
{
	AttachBuffer(BufferAttachment::DepthAttachment, (InternalRenderableFormat)depthRenderableFormat, Format::DEPTH_COMPONENT, dataTye, false, layerTextureInfo, enableDoubleBuffering);
}

void RenderTexture2D::AttachStencilBuffer(StencilRenderableFormat stencilRenderableFormat, DataType dataTye, bool useRenderBuffer)
{
	AttachBuffer(BufferAttachment::StencilAttachment, (InternalRenderableFormat)stencilRenderableFormat, Format::STENCIL_INDEX, dataTye, useRenderBuffer);
}

void RenderTexture2D::AttachStencilBuffer(StencilRenderableFormat stencilRenderableFormat, DataType dataTye, LayerTextureInfo layerTextureInfo, bool enableDoubleBuffering)
{
	AttachBuffer(BufferAttachment::StencilAttachment, (InternalRenderableFormat)stencilRenderableFormat, Format::STENCIL_INDEX, dataTye, false, layerTextureInfo, enableDoubleBuffering);
}

void RenderTexture2D::AttachDepthStencilBuffer(DepthStencilRenderableFormat depthStencilRenderableFormat, DataType dataTye, bool useRenderBuffer)
{
	AttachBuffer(BufferAttachment::Depth_Stencil_Attachment, (InternalRenderableFormat)depthStencilRenderableFormat, Format::DEPTH_STENCIL, dataTye, useRenderBuffer);
}

void RenderTexture2D::AttachDepthStencilBuffer(DepthStencilRenderableFormat depthStencilRenderableFormat, DataType dataTye, LayerTextureInfo layerTextureInfo, bool enableDoubleBuffering)
{
	AttachBuffer(BufferAttachment::Depth_Stencil_Attachment, (InternalRenderableFormat)depthStencilRenderableFormat, Format::DEPTH_STENCIL, dataTye, false, layerTextureInfo, enableDoubleBuffering);
}

void RenderTexture2D::DetachBuffer(BufferAttachment bufferAttachement)
{

	if (m_renderBufferID.find(bufferAttachement) != m_renderBufferID.end())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, static_cast<GLenum>(bufferAttachement), GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDeleteRenderbuffers(1,&m_renderBufferID[bufferAttachement]);

		m_renderBufferID.erase(bufferAttachement);
	}
	else
	{
		bool isTexture = false;
		int layerTexture = -1;
		for (int i = 0; i < m_textureBuffers.size(); i++)
		{
			if (m_textureBuffers[i] == bufferAttachement)
			{
				isTexture = true;
				layerTexture = i;
				break;
			}
		}
		if (isTexture)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
			glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(bufferAttachement), GL_TEXTURE_2D, 0, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glDeleteTextures(1, &m_layerTextureInfos[layerTexture].m_textureID);
			if (m_backBuffer[layerTexture] != 0)
				glDeleteTextures(1, &m_backBuffer[layerTexture]);

			m_layerTextureInfos.erase(m_layerTextureInfos.begin() + layerTexture);
			m_textureBuffers.erase(m_textureBuffers.begin() + layerTexture);
			m_backBuffer.erase(m_backBuffer.begin() + layerTexture);
		}
	}
}

void RenderTexture2D::SwapBuffer()
{
	for (int i = 0; i < m_layerTextureInfos.size(); i++)
	{
		if (m_backBuffer[i] != 0)
		{
			GLuint id = m_layerTextureInfos[i].m_textureID;
			m_layerTextureInfos[i].m_textureID = m_backBuffer[i];
			m_backBuffer[i] = id;

			glFramebufferTexture2D(GL_FRAMEBUFFER, (int)m_textureBuffers[i], GL_TEXTURE_2D, m_backBuffer[i], 0);
		}
		
	}
}



void RenderTexture2D::AttachBuffer(BufferAttachment bufferAttachement, InternalRenderableFormat internalRenderableFormat, Format format, DataType dataTye, bool useRenderBuffer, LayerTextureInfo layerTextureInfo, bool enableDoubleBuffering)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);

	if (!useRenderBuffer)
	{
		GLuint textureID = 0;

		int layerIndex = -1;
		for (int i = 0; i < m_textureBuffers.size(); i++)
		{
			if (m_textureBuffers[i] == bufferAttachement)
			{
				layerIndex = i;
				break;
			}
		}

		if(layerIndex == -1)
		{
			glGenTextures(1, &textureID);
			layerTextureInfo.m_textureID = textureID;
			m_layerTextureInfos.push_back(layerTextureInfo);
			m_textureBuffers.push_back(bufferAttachement);
			layerIndex = m_layerTextureInfos.size() - 1;

			if (enableDoubleBuffering)
			{
				GLuint backBufferID = 0;
				glGenTextures(1, &backBufferID);

				m_backBuffer.push_back(backBufferID);
			}
			else
				m_backBuffer.push_back(0);
		}
		else
		{
			layerTextureInfo.m_textureID = m_layerTextureInfos[layerIndex].m_textureID;
			m_layerTextureInfos[layerIndex] = layerTextureInfo;

			if (enableDoubleBuffering)
			{
				if (m_backBuffer[layerIndex] == 0)
				{
					GLuint backBufferID = 0;
					glGenTextures(1, &backBufferID);

					m_backBuffer[layerIndex] = backBufferID;
				}
			}
			else
			{
				if (m_backBuffer[layerIndex] != 0)
				{
					glDeleteTextures(1, &m_backBuffer[layerIndex]);
					m_backBuffer[layerIndex] = 0;
				}
			}
		}

		if(enableDoubleBuffering)
			textureID = m_backBuffer[layerIndex];
		else
			textureID = layerTextureInfo.m_textureID;

		glBindTexture(GL_TEXTURE_2D, textureID); 
		glTexImage2D(GL_TEXTURE_2D, 0, (int)internalRenderableFormat, m_width, m_height, 0, (int)format, (int)dataTye, NULL);
		glTexParameteri((int)m_textureType, GL_TEXTURE_MIN_FILTER, (GLint)m_layerTextureInfos[layerIndex].m_minificationFilter);
		glTexParameteri((int)m_textureType, GL_TEXTURE_MAG_FILTER, (GLint)m_layerTextureInfos[layerIndex].m_magnificationFilter);
		glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_S, (GLint)m_layerTextureInfos[layerIndex].m_wrapHorizontalParameter);
		glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_T, (GLint)m_layerTextureInfos[layerIndex].m_wrapVerticalParameter);

		glTexParameteri((int)m_textureType, GL_TEXTURE_BASE_LEVEL, (GLint)m_layerTextureInfos[layerIndex].m_mipmapBaseLevel);
		glTexParameteri((int)m_textureType, GL_TEXTURE_MAX_LEVEL, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMaxLevel);
		glTexParameteri((int)m_textureType, GL_TEXTURE_MIN_LOD, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMinLOD);
		glTexParameteri((int)m_textureType, GL_TEXTURE_MAX_LOD, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMaxLOD);
		glTexParameteri((int)m_textureType, GL_TEXTURE_LOD_BIAS, (GLint)m_layerTextureInfos[layerIndex].m_mipmapLODBias);
		glTexParameterf((int)m_textureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_layerTextureInfos[layerIndex].m_anisotropyValue);

		glFramebufferTexture2D(GL_FRAMEBUFFER, (int)bufferAttachement, GL_TEXTURE_2D, textureID, 0);
	
		if(layerTextureInfo.m_generateMimpap)
			glGenerateMipmap((int)m_textureType);

		if (enableDoubleBuffering)
		{
			textureID = m_layerTextureInfos[layerIndex].m_textureID;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, (int)internalRenderableFormat, m_width, m_height, 0, (int)format, (int)dataTye, NULL);
			glTexParameteri((int)m_textureType, GL_TEXTURE_MIN_FILTER, (GLint)m_layerTextureInfos[layerIndex].m_minificationFilter);
			glTexParameteri((int)m_textureType, GL_TEXTURE_MAG_FILTER, (GLint)m_layerTextureInfos[layerIndex].m_magnificationFilter);
			glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_S, (GLint)m_layerTextureInfos[layerIndex].m_wrapHorizontalParameter);
			glTexParameteri((int)m_textureType, GL_TEXTURE_WRAP_T, (GLint)m_layerTextureInfos[layerIndex].m_wrapVerticalParameter);

			glTexParameteri((int)m_textureType, GL_TEXTURE_BASE_LEVEL, (GLint)m_layerTextureInfos[layerIndex].m_mipmapBaseLevel);
			glTexParameteri((int)m_textureType, GL_TEXTURE_MAX_LEVEL, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMaxLevel);
			glTexParameteri((int)m_textureType, GL_TEXTURE_MIN_LOD, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMinLOD);
			glTexParameteri((int)m_textureType, GL_TEXTURE_MAX_LOD, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMaxLOD);
			glTexParameteri((int)m_textureType, GL_TEXTURE_LOD_BIAS, (GLint)m_layerTextureInfos[layerIndex].m_mipmapLODBias);
			glTexParameterf((int)m_textureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_layerTextureInfos[layerIndex].m_anisotropyValue);
			if (layerTextureInfo.m_generateMimpap)
				glGenerateMipmap((int)m_textureType);
		}
	}
	else
	{
		GLuint renderBufferID = 0;
		if (m_renderBufferID.find(bufferAttachement) == m_renderBufferID.end())
		{
			glGenRenderbuffers(1, &renderBufferID);
			m_renderBufferID[bufferAttachement] = renderBufferID;
		}

		renderBufferID = m_renderBufferID[bufferAttachement];
		glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, (int)internalRenderableFormat, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, (int)bufferAttachement, GL_RENDERBUFFER, renderBufferID);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "framebuffer not properly initialized : " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	else
		std::cout << "framebuffer is initialized" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
