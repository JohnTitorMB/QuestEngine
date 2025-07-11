#include "RenderTexture2D.h"
#include <iostream>

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF



RenderTexture2D::RenderTexture2D(int width, int height) : RenderTexture(width, height)
{
	
}

RenderTexture2D::~RenderTexture2D()
{
	for (const auto& info : m_renderBuffersInfo)
		glDeleteRenderbuffers(1, &info.second.m_renderBufferID);

	m_renderBuffersInfo.clear();
}



void RenderTexture2D::DetachBuffer(BufferAttachment bufferAttachement)
{
	RenderTexture::DetachBuffer(bufferAttachement);

	if (m_renderBuffersInfo.find(bufferAttachement) != m_renderBuffersInfo.end())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, static_cast<GLenum>(bufferAttachement), GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDeleteRenderbuffers(1,&m_renderBuffersInfo[bufferAttachement].m_renderBufferID);

		m_renderBuffersInfo.erase(bufferAttachement);
	}
	else
	{
		int layerTexture = -1;
		for (int i = 0; i < m_textureBuffers.size(); i++)
		{
			if (m_textureBuffers[i] == bufferAttachement)
			{
				layerTexture = i;
				break;
			}
		}
		if (layerTexture != -1)
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
	RenderTexture::SwapBuffer();

	for (int i = 0; i < m_layerTextureInfos.size(); i++)
	{
		if (m_backBuffer[i] != 0)
		{
			GLuint id = m_layerTextureInfos[i].m_textureID;
			m_layerTextureInfos[i].m_textureID = m_backBuffer[i];
			m_backBuffer[i] = id;

			if(m_layerTextureInfos[i].m_useMultisampledTexture)
				glFramebufferTexture2D(GL_FRAMEBUFFER, (int)m_textureBuffers[i], GL_TEXTURE_2D_MULTISAMPLE, m_backBuffer[i], 0);
			else
				glFramebufferTexture2D(GL_FRAMEBUFFER, (int)m_textureBuffers[i], GL_TEXTURE_2D, m_backBuffer[i], 0);
		}
		
	}
}


void RenderTexture2D::Blit(RenderTexture2D* rtRead, RenderTexture2D* rtDraw, int srcX0, int srcY0, int srcX1, int srcY1, int dstX0, int dstY0, int dstX1, int dstY1, BlitBitField mask, BlitFilter filter)
{
	if(rtRead == nullptr)
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	else
		glBindFramebuffer(GL_READ_FRAMEBUFFER, rtRead->GetFrameBufferID());

	if(rtDraw == nullptr)
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	else
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rtDraw->GetFrameBufferID());

	glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, (int)mask, (int)filter);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void RenderTexture2D::Resize(int newWidth, int newHeight)
{
	RenderTexture::Resize(newWidth, newHeight);
	if (m_width == newWidth && m_height == newHeight)
		return;

	m_width = newWidth;
	m_height = newHeight;

	for (int i = 0; i < m_layerTextureInfos.size(); ++i)
	{
		GLuint textureID = m_layerTextureInfos[i].m_textureID;
		if (!m_layerTextureInfos[i].m_useMultisampledTexture)
		{
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, (int)m_layerTextureInfos[i].m_internalFormat, m_width, m_height, 0, (int)m_layerTextureInfos[i].m_format, (int)m_layerTextureInfos[i].m_dataType, nullptr);

			if (m_backBuffer[i] != 0)
			{
				GLuint backBufferID = m_backBuffer[i];
				glBindTexture(GL_TEXTURE_2D, backBufferID);
				glTexImage2D(GL_TEXTURE_2D, 0, (int)m_layerTextureInfos[i].m_internalFormat, m_width, m_height, 0, (int)m_layerTextureInfos[i].m_format, (int)m_layerTextureInfos[i].m_dataType, nullptr);
			}
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureID);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, (GLint)m_layerTextureInfos[i].m_samples, (int)m_layerTextureInfos[i].m_internalFormat, m_width, m_height, m_layerTextureInfos[i].m_useFixedsamplelocations);

			if (m_backBuffer[i] != 0)
			{
				GLuint backBufferID = m_backBuffer[i];
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, backBufferID);
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, (GLint)m_layerTextureInfos[i].m_samples, (int)m_layerTextureInfos[i].m_internalFormat, m_width, m_height, m_layerTextureInfos[i].m_useFixedsamplelocations);
			}
		}
	}

	for (auto& entry : m_renderBuffersInfo)
	{
		GLuint renderBufferID = entry.second.m_renderBufferID;
		glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);

		if(!entry.second.m_useMultisampledRenderBuffer)
			glRenderbufferStorage(GL_RENDERBUFFER, (int)entry.second.m_interRerableFormat, m_width, m_height);
		else
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, entry.second.m_samples, (int)(int)entry.second.m_interRerableFormat, m_width, m_height);
	}

}



void RenderTexture2D::AttachColorTextureBufferMS(ColorRenderableFormat colorRenderableFormat, unsigned int colorAttachementIndex, LayerTextureInfo layerTextureInfo)
{
	AttachColorTextureBuffer(colorRenderableFormat, (ColorFormat)0, (DataType)0, colorAttachementIndex, layerTextureInfo);
}

void RenderTexture2D::AttachColorRenderBuffer(ColorRenderableFormat colorRenderableFormat, unsigned int colorAttachementIndex, RenderBufferInfo renderBufferInfo)
{
	int bufferAttachmentIndex = (int)BufferAttachment::ColorAttachment0 + colorAttachementIndex;
	if (bufferAttachmentIndex > (int)BufferAttachment::ColorAttachment31)
	{
		std::cout << "RenderTexture cannot attach a buffer to a color index greater than 31" << std::endl;
		return;
	}

	AttachRenderBuffer((BufferAttachment)bufferAttachmentIndex, (InternalFormat)colorRenderableFormat, renderBufferInfo);
}



void RenderTexture2D::AttachDepthTextureBufferMS(DepthRenderableFormat depthRenderableFormat, LayerTextureInfo layerTextureInfo)
{
	layerTextureInfo.m_useMultisampledTexture = true;
	AttachTextureBuffer(BufferAttachment::DepthAttachment, (InternalFormat)depthRenderableFormat, Format::DEPTH_COMPONENT, (DataType)0, layerTextureInfo);
}

void RenderTexture2D::AttachDepthRenderBuffer(DepthRenderableFormat depthRenderableFormat, RenderBufferInfo renderBufferInfo)
{
	AttachRenderBuffer(BufferAttachment::DepthAttachment, (InternalFormat)depthRenderableFormat, renderBufferInfo);
}



void RenderTexture2D::AttachStencilTextureBufferMS(StencilRenderableFormat stencilRenderableFormat, LayerTextureInfo layerTextureInfo)
{
	layerTextureInfo.m_useMultisampledTexture = true;
	AttachTextureBuffer(BufferAttachment::StencilAttachment, (InternalFormat)stencilRenderableFormat, Format::STENCIL_INDEX, (DataType)0, layerTextureInfo);
}

void RenderTexture2D::AttachStencilRenderBuffer(StencilRenderableFormat stencilRenderableFormat, RenderBufferInfo renderBufferInfo)
{
	AttachRenderBuffer(BufferAttachment::StencilAttachment, (InternalFormat)stencilRenderableFormat, renderBufferInfo);
}



void RenderTexture2D::AttachDepthStencilTextureBufferMS(DepthStencilRenderableFormat depthStencilRenderableFormat, LayerTextureInfo layerTextureInfo)
{
	layerTextureInfo.m_useMultisampledTexture = true;
	AttachTextureBuffer(BufferAttachment::Depth_Stencil_Attachment, (InternalFormat)depthStencilRenderableFormat, Format::DEPTH_STENCIL,(DataType)0, layerTextureInfo);
}

void RenderTexture2D::AttachDepthStencilRenderBuffer(DepthStencilRenderableFormat depthStencilRenderableFormat, RenderBufferInfo renderBufferInfo)
{
	AttachRenderBuffer(BufferAttachment::Depth_Stencil_Attachment, (InternalFormat)depthStencilRenderableFormat, renderBufferInfo);
}


void RenderTexture2D::AttachRenderBuffer(BufferAttachment bufferAttachement, InternalFormat internalRenderableFormat, RenderBufferInfo renderBufferInfo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);

	GLuint renderBufferID = 0;
	if (m_renderBuffersInfo.find(bufferAttachement) == m_renderBuffersInfo.end())
	{
		glGenRenderbuffers(1, &renderBufferID);
		renderBufferInfo.m_renderBufferID = renderBufferID;
		m_renderBuffersInfo[bufferAttachement] = renderBufferInfo;
	}

	renderBufferID = renderBufferInfo.m_renderBufferID;
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
	if (renderBufferInfo.m_useMultisampledRenderBuffer)
	{
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, renderBufferInfo.m_samples, (int)internalRenderableFormat, m_width, m_height);
	}
	else
		glRenderbufferStorage(GL_RENDERBUFFER, (int)internalRenderableFormat, m_width, m_height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, (int)bufferAttachement, GL_RENDERBUFFER, renderBufferID);

	m_renderBuffersInfo[bufferAttachement].m_interRerableFormat = internalRenderableFormat;


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "framebuffer not properly initialized : " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	else
		std::cout << "framebuffer is initialized" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture2D::AttachTextureBuffer(BufferAttachment bufferAttachement, InternalFormat internalRenderableFormat, Format format, DataType dataTye, LayerTextureInfo layerTextureInfo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);

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

	layerTextureInfo.m_dataType = dataTye;
	layerTextureInfo.m_format = format;
	layerTextureInfo.m_internalFormat = internalRenderableFormat;

	if (layerIndex == -1)
	{
		glGenTextures(1, &textureID);
		layerTextureInfo.m_textureID = textureID;
		m_layerTextureInfos.push_back(layerTextureInfo);
		m_textureBuffers.push_back(bufferAttachement);
		layerIndex = m_layerTextureInfos.size() - 1;

		if (layerTextureInfo.m_enableDoubleBuffering)
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

		if (layerTextureInfo.m_enableDoubleBuffering)
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

	if (layerTextureInfo.m_enableDoubleBuffering)
		textureID = m_backBuffer[layerIndex];
	else
		textureID = layerTextureInfo.m_textureID;

	if (layerTextureInfo.m_useMultisampledTexture)
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureID);
		int fixedsamplelocations = m_layerTextureInfos[layerIndex].m_useFixedsamplelocations == false ? 0 : 1;
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, (GLint)m_layerTextureInfos[layerIndex].m_samples, (int)internalRenderableFormat, m_width, m_height, fixedsamplelocations);

		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, (GLint)m_layerTextureInfos[layerIndex].m_wrapHorizontalParameter);
		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, (GLint)m_layerTextureInfos[layerIndex].m_wrapVerticalParameter);
		glFramebufferTexture2D(GL_FRAMEBUFFER, (int)bufferAttachement, GL_TEXTURE_2D_MULTISAMPLE, textureID, 0);
	
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, (int)internalRenderableFormat, m_width, m_height, 0, (int)format, (int)dataTye, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)m_layerTextureInfos[layerIndex].m_minificationFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)m_layerTextureInfos[layerIndex].m_magnificationFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)m_layerTextureInfos[layerIndex].m_wrapHorizontalParameter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)m_layerTextureInfos[layerIndex].m_wrapVerticalParameter);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, (GLint)m_layerTextureInfos[layerIndex].m_mipmapBaseLevel);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMaxLevel);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMinLOD);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMaxLOD);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, (GLint)m_layerTextureInfos[layerIndex].m_mipmapLODBias);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_layerTextureInfos[layerIndex].m_anisotropyValue);

		float borderColor[] = { m_layerTextureInfos[layerIndex].m_borderColor.m_x, 
								m_layerTextureInfos[layerIndex].m_borderColor.m_y, 
								m_layerTextureInfos[layerIndex].m_borderColor.m_z, 
								m_layerTextureInfos[layerIndex].m_borderColor.m_w };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glFramebufferTexture2D(GL_FRAMEBUFFER, (int)bufferAttachement, GL_TEXTURE_2D, textureID, 0);

		if (layerTextureInfo.m_generateMimpap)
			glGenerateMipmap((int)m_textureType);
	}

	if (m_layerTextureInfos[layerIndex].m_enableDoubleBuffering)
	{
		textureID = m_layerTextureInfos[layerIndex].m_textureID;

		if (m_layerTextureInfos[layerIndex].m_useMultisampledTexture)
		{
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureID);
			int fixedsamplelocations = m_layerTextureInfos[layerIndex].m_useFixedsamplelocations == false ? 0 : 1;
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, (GLint)m_layerTextureInfos[layerIndex].m_samples, (int)internalRenderableFormat, m_width, m_height, fixedsamplelocations);

			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, (GLint)m_layerTextureInfos[layerIndex].m_wrapHorizontalParameter);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, (GLint)m_layerTextureInfos[layerIndex].m_wrapVerticalParameter);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, (int)internalRenderableFormat, m_width, m_height, 0, (int)format, (int)dataTye, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)m_layerTextureInfos[layerIndex].m_minificationFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)m_layerTextureInfos[layerIndex].m_magnificationFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)m_layerTextureInfos[layerIndex].m_wrapHorizontalParameter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)m_layerTextureInfos[layerIndex].m_wrapVerticalParameter);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, (GLint)m_layerTextureInfos[layerIndex].m_mipmapBaseLevel);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMaxLevel);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMinLOD);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMaxLOD);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, (GLint)m_layerTextureInfos[layerIndex].m_mipmapLODBias);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_layerTextureInfos[layerIndex].m_anisotropyValue);

			float borderColor[] = { m_layerTextureInfos[layerIndex].m_borderColor.m_x,
						m_layerTextureInfos[layerIndex].m_borderColor.m_y,
						m_layerTextureInfos[layerIndex].m_borderColor.m_z,
						m_layerTextureInfos[layerIndex].m_borderColor.m_w };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			if (layerTextureInfo.m_generateMimpap)
				glGenerateMipmap((int)m_textureType);
		}

	}


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "framebuffer not properly initialized : " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	else
		std::cout << "framebuffer is initialized" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture2D::SetTexureMSSample(BufferAttachment bufferAttachement, int sample)
{
	if (sample < 1)
		return;

	int layerIndex = -1;
	for (int i = 0; i < m_textureBuffers.size(); i++)
	{
		if (m_textureBuffers[i] == bufferAttachement)
		{
			layerIndex = i;
			break;
		}
	}

	if (layerIndex != -1)
	{
		if (m_layerTextureInfos[layerIndex].m_useMultisampledTexture)
		{
			m_layerTextureInfos[layerIndex].m_samples = sample;
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_layerTextureInfos[layerIndex].m_textureID);
			int fixedsamplelocations = m_layerTextureInfos[layerIndex].m_useFixedsamplelocations == false ? 0 : 1;
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, (GLint)m_layerTextureInfos[layerIndex].m_samples, (int)m_layerTextureInfos[layerIndex].m_internalFormat, m_width, m_height, fixedsamplelocations);

			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, (GLint)m_layerTextureInfos[layerIndex].m_wrapHorizontalParameter);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, (GLint)m_layerTextureInfos[layerIndex].m_wrapVerticalParameter);
			
			glFramebufferTexture2D(GL_FRAMEBUFFER, (int)bufferAttachement, GL_TEXTURE_2D_MULTISAMPLE, m_layerTextureInfos[layerIndex].m_textureID, 0);
		
			std::cout << "RefreshSample  RT" << m_layerTextureInfos[layerIndex].m_samples << std::endl;

		}
	}
}

void RenderTexture2D::SetRenderBufferMSSample(BufferAttachment bufferAttachement, int sample)
{
	if (sample < 1)
		return;

	if (m_renderBuffersInfo.find(bufferAttachement) != m_renderBuffersInfo.end())
	{
		m_renderBuffersInfo[bufferAttachement].m_samples = sample;
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffersInfo[bufferAttachement].m_renderBufferID);

		glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_renderBuffersInfo[bufferAttachement].m_samples, (int)m_renderBuffersInfo[bufferAttachement].m_interRerableFormat, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, (int)bufferAttachement, GL_RENDERBUFFER, m_renderBuffersInfo[bufferAttachement].m_renderBufferID);
	}
}

std::string GetGLErrorString(GLenum error)
{
	switch (error)
	{
	case GL_NO_ERROR: return "GL_NO_ERROR";
	case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
	case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
	default: return "UNKNOWN_ERROR";
	}
}


void RenderTexture2D::DrawBuffer(DrawBufferType drawBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "FBO not complete! Status: 0x" << std::hex << status << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebufferID);
	glDrawBuffer(GL_NONE);


	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cerr << "Error in glDrawBuffer with buffer " << static_cast<int>(drawBuffer)
			<< ": " << GetGLErrorString(error) << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture2D::ReadBuffer(ReadBufferMode mode)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
	glReadBuffer(static_cast<GLenum>(mode));

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cerr << "Error in glReadBuffer with mode " << static_cast<int>(mode)
			<< ": " << GetGLErrorString(error) << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}