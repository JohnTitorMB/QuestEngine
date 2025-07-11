#include "RenderCubeMap.h"
#include <iostream>

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF



RenderCubeMap::RenderCubeMap(int width, int height) : RenderTexture(width, height)
{
	m_width = width;
	m_height = height;
	glGenFramebuffers(1, &m_framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "framebuffer not properly initialized : " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	else
		std::cout << "framebuffer is initialized" << std::endl;

	m_textureType = TextureType::CUBEMAP;
}

RenderCubeMap::~RenderCubeMap()
{
	glDeleteFramebuffers(1, &m_framebufferID);

	for (const GLuint& id : m_backBuffer)
	{
		if (id != 0)
			glDeleteTextures(1, &id);
	}

	m_backBuffer.clear();
}

void RenderCubeMap::DetachBuffer(BufferAttachment bufferAttachement)
{
	RenderTexture::DetachBuffer(bufferAttachement);
	int layerTexture = -1;
	for (int i = 0; i < m_textureBuffers.size(); i++) {
		if (m_textureBuffers[i] == bufferAttachement) {
			layerTexture = i;
			break;
		}
	}
	if (layerTexture != -1) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(bufferAttachement),
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + m_currentFaceIndex,
			0, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDeleteTextures(1, &m_layerTextureInfos[layerTexture].m_textureID);
		if (m_backBuffer[layerTexture] != 0)
			glDeleteTextures(1, &m_backBuffer[layerTexture]);

		m_layerTextureInfos.erase(m_layerTextureInfos.begin() + layerTexture);
		m_textureBuffers.erase(m_textureBuffers.begin() + layerTexture);
		m_backBuffer.erase(m_backBuffer.begin() + layerTexture);
	}
}


void RenderCubeMap::SwapBuffer()
{
	RenderTexture::SwapBuffer();

	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
	for (int i = 0; i < m_layerTextureInfos.size(); i++) {
		if (m_backBuffer[i] != 0) {
			GLuint id = m_layerTextureInfos[i].m_textureID;
			m_layerTextureInfos[i].m_textureID = m_backBuffer[i];
			m_backBuffer[i] = id;

			glFramebufferTexture2D(GL_FRAMEBUFFER, (int)m_textureBuffers[i],
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + m_currentFaceIndex,
				m_backBuffer[i], 0);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void RenderCubeMap::Blit(RenderCubeMap* rtRead, RenderCubeMap* rtDraw,
	int srcX0, int srcY0, int srcX1, int srcY1,
	int dstX0, int dstY0, int dstX1, int dstY1,
	BlitBitField mask, BlitFilter filter)
{
	if (rtRead != nullptr)
		glBindFramebuffer(GL_READ_FRAMEBUFFER, rtRead->GetFrameBufferID());
	else
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	if (rtDraw != nullptr)
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rtDraw->GetFrameBufferID());
	else
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1,
		dstX0, dstY0, dstX1, dstY1,
		(int)mask, (int)filter);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderCubeMap::Blit(RenderCubeMap* rtA, RenderCubeMap* rtB, bool blitAllFaces,
	int srcX0, int srcY0, int srcX1, int srcY1,
	int dstX0, int dstY0, int dstX1, int dstY1,
	BlitBitField mask, BlitFilter filter)
{
	if (!blitAllFaces) {
		RenderCubeMap::Blit(rtA, rtB, srcX0, srcY0, srcX1, srcY1,
			dstX0, dstY0, dstX1, dstY1,
			mask, filter);
		return;
	}

	int originalFaceRead = rtA ? rtA->m_currentFaceIndex : -1;
	int originalFaceDraw = rtB ? rtB->m_currentFaceIndex : -1;

	for (int face = 0; face < 6; ++face) {
		if (rtA) rtA->AttachFaceToFramebuffer(BufferAttachment::ColorAttachment0, face);
		if (rtB) rtB->AttachFaceToFramebuffer(BufferAttachment::ColorAttachment0, face);

		RenderCubeMap::Blit(rtA, rtB, srcX0, srcY0, srcX1, srcY1,
			dstX0, dstY0, dstX1, dstY1,
			mask, filter);
	}

	if (rtA && originalFaceRead >= 0)
		rtA->AttachFaceToFramebuffer(BufferAttachment::ColorAttachment0, originalFaceRead);
	if (rtB && originalFaceDraw >= 0)
		rtB->AttachFaceToFramebuffer(BufferAttachment::ColorAttachment0, originalFaceDraw);
}



void RenderCubeMap::Resize(int newWidth, int newHeight)
{
	RenderTexture::Resize(newWidth, newHeight);

	if (m_width == newWidth && m_height == newHeight)
		return;

	m_width = newWidth;
	m_height = newHeight;

	for (int i = 0; i < m_layerTextureInfos.size(); ++i)
	{
		GLuint textureID = m_layerTextureInfos[i].m_textureID;
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		for (int face = 0; face < 6; ++face)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0,
				(int)m_layerTextureInfos[i].m_internalFormat,
				m_width, m_height, 0,
				(int)m_layerTextureInfos[i].m_format,
				(int)m_layerTextureInfos[i].m_dataType,
				nullptr);
		}

		if (m_backBuffer[i] != 0)
		{
			GLuint backBufferID = m_backBuffer[i];
			glBindTexture(GL_TEXTURE_CUBE_MAP, backBufferID);
			for (int face = 0; face < 6; ++face)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0,
					(int)m_layerTextureInfos[i].m_internalFormat,
					m_width, m_height, 0,
					(int)m_layerTextureInfos[i].m_format,
					(int)m_layerTextureInfos[i].m_dataType,
					nullptr);
			}
		}
	}
}

void RenderCubeMap::AttachTextureBuffer(BufferAttachment bufferAttachement, InternalFormat internalRenderableFormat, Format format, DataType dataTye, LayerTextureInfo layerTextureInfo)
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

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, (int)internalRenderableFormat,
			m_width, m_height, 0, (int)format, (int)dataTye, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, (GLint)m_layerTextureInfos[layerIndex].m_minificationFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, (GLint)m_layerTextureInfos[layerIndex].m_magnificationFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, (GLint)m_layerTextureInfos[layerIndex].m_wrapHorizontalParameter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, (GLint)m_layerTextureInfos[layerIndex].m_wrapVerticalParameter);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, (GLint)m_layerTextureInfos[layerIndex].m_mipmapBaseLevel);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMaxLevel);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_LOD, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMinLOD);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LOD, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMaxLOD);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_LOD_BIAS, (GLint)m_layerTextureInfos[layerIndex].m_mipmapLODBias);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_layerTextureInfos[layerIndex].m_anisotropyValue);

	float borderColor[] = { m_layerTextureInfos[layerIndex].m_borderColor.m_x,
							m_layerTextureInfos[layerIndex].m_borderColor.m_y,
							m_layerTextureInfos[layerIndex].m_borderColor.m_z,
							m_layerTextureInfos[layerIndex].m_borderColor.m_w };
	glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);

	glFramebufferTexture2D(GL_FRAMEBUFFER, (int)bufferAttachement,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X + m_currentFaceIndex,
		textureID, 0);

	if (layerTextureInfo.m_generateMimpap)
		glGenerateMipmap((int)m_textureType);

	if (m_layerTextureInfos[layerIndex].m_enableDoubleBuffering)
	{
		textureID = m_layerTextureInfos[layerIndex].m_textureID;

		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		for (int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, (int)internalRenderableFormat,
				m_width, m_height, 0, (int)format, (int)dataTye, nullptr);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, (GLint)m_layerTextureInfos[layerIndex].m_minificationFilter);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, (GLint)m_layerTextureInfos[layerIndex].m_magnificationFilter);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, (GLint)m_layerTextureInfos[layerIndex].m_wrapHorizontalParameter);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, (GLint)m_layerTextureInfos[layerIndex].m_wrapVerticalParameter);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, (GLint)m_layerTextureInfos[layerIndex].m_mipmapBaseLevel);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMaxLevel);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_LOD, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMinLOD);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LOD, (GLint)m_layerTextureInfos[layerIndex].m_mipmapMaxLOD);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_LOD_BIAS, (GLint)m_layerTextureInfos[layerIndex].m_mipmapLODBias);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_layerTextureInfos[layerIndex].m_anisotropyValue);

		float borderColor[] = { m_layerTextureInfos[layerIndex].m_borderColor.m_x,
					m_layerTextureInfos[layerIndex].m_borderColor.m_y,
					m_layerTextureInfos[layerIndex].m_borderColor.m_z,
					m_layerTextureInfos[layerIndex].m_borderColor.m_w };
		glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);

		if (layerTextureInfo.m_generateMimpap)
			glGenerateMipmap((int)m_textureType);
	}


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "framebuffer not properly initialized : " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	else
		std::cout << "framebuffer is initialized" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderCubeMap::AttachFaceToFramebuffer(BufferAttachment attachment, int faceIndex)
{
	if (faceIndex < 0 || faceIndex >= 6) {
		std::cerr << "Invalid cubemap face index" << std::endl;
		return;
	}

	for (int i = 0; i < m_textureBuffers.size(); ++i) {
		if (m_textureBuffers[i] == attachment) {
			GLuint textureID = m_layerTextureInfos[i].m_textureID;
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
			glFramebufferTexture2D(GL_FRAMEBUFFER, (int)attachment,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex,
				textureID, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			m_currentFaceIndex = faceIndex;
			return;
		}
	}

	std::cerr << "Attachment not found in RenderCubeMap" << std::endl;
}
