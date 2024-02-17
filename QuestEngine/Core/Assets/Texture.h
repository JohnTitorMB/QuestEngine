#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include <string>
#include <glad/glad.h>
#include "Assets.h"

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

class Texture : public Assets
{
public:

	Texture(std::string filePath);
	~Texture();

	void Bind(int textureIndex);
	void Unbind(int textureIndex);

	void SetWrapHorizontalParameter(Wrap wrapHorizontalParameter);
	void SetVerticalParameter(Wrap wrapVerticalParameter);
	Wrap GetHorizontalParameter()const;
	Wrap GetVerticalParameter()const;

	void SetMinification(MinificationFilter minificationFilter);
	void SetMagnification(MagnificationFilter magnificationFilter);
	void SetMipmapTexture(int level, std::string filePath);

	MinificationFilter SetMinification()const;
	MagnificationFilter SetMagnification()const;

	void SetMipmapBaseLevel(int mipmapBaseLevel);
	void SetMipmapMaxLevel(int mipmapMaxLevel);
	void SetMipmapMinLOD(int mipmapMinLOD);
	void SetMipmapMaxLOD(int mipmapMaxLOD);
	void SetMipmapLODBias(int mipmapLODBias);

	int GetMipmapBaseLevel()const;
	int GetMipmapMaxLevel()const;
	int GetMipmapMinLOD()const;
	int GetMipmapMaxLOD()const;
	int GetMipmapLODBIAS()const;

	void SetAnisotropy(float anisotropyValue);
	float GetAnisotropy()const;
	float GetMaxGPUAnisotropy()const;


private:
	unsigned char* LoadTexture(std::string filePath);
	void GenereTextureID();
	void UpdateTextureData(const unsigned char* data);

	GLuint m_textureID;
	int m_width = 0;
	int m_height = 0;

	int m_mipmapBaseLevel = 0;
	int m_mipmapMaxLevel = 1000;
	int m_mipmapMinLOD = -1000;
	int m_mipmapMaxLOD = 1000;
	int m_mipmapLODBias = 0;
	float m_anisotropyValue = 16;


	Wrap m_wrapHorizontalParameter = Wrap::Repeat;
	Wrap m_wrapVerticalParameter = Wrap::Repeat;
	MagnificationFilter m_magnificationFilter = MagnificationFilter::Bilinear;
	MinificationFilter m_minificationFilter = MinificationFilter::Trilinear;
};

#endif // !_TEXTURE_H_

