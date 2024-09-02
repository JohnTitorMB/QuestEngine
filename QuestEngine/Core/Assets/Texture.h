#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include <string>
#include <glad/glad.h>
#include "Assets.h"
#include "../../Library/stb_image.h"



#include <vector>

enum class TextureType
{
	DEFAULT = GL_TEXTURE_2D,
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

class Texture : public Assets
{
public:
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
	};

public:

	Texture();
	Texture(std::string filePath);
	virtual ~Texture();

	void Bind(int textureIndex, int layer = 0);
	void Unbind(int textureIndex, int layer = 0);

	void SetWrapHorizontalParameter(Wrap wrapHorizontalParameter, int layer = 0);
	void SetVerticalParameter(Wrap wrapVerticalParameter, int layer = 0);
	void SetDepthParameter(Wrap wrapVerticalParameter, int layer = 0);
	void GenerateMipmap(int layer = 0);
	void GenerateAllMipmap(bool forceGeneration = true);
	Wrap GetHorizontalParameter(int layer = 0)const;
	Wrap GetVerticalParameter(int layer = 0)const;
	Wrap GetDepthParameter(int layer = 0)const;

	void SetMinification(MinificationFilter minificationFilter, int layer = 0);
	void SetMagnification(MagnificationFilter magnificationFilter, int layer = 0);

	MinificationFilter GetMinification(int layer = 0)const;
	MagnificationFilter GetMagnification(int layer = 0)const;

	void SetMipmapBaseLevel(int mipmapBaseLevel, int layer = 0);
	void SetMipmapMaxLevel(int mipmapMaxLevel, int layer = 0);
	void SetMipmapMinLOD(int mipmapMinLOD, int layer = 0);
	void SetMipmapMaxLOD(int mipmapMaxLOD, int layer = 0);
	void SetMipmapLODBias(int mipmapLODBias, int layer = 0);

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
	std::string m_path;

protected:
	unsigned char* LoadTexture(std::string filePath);
	
	int m_width = 0;
	int m_height = 0;

	std::vector<LayerTextureInfo> m_layerTextureInfos;
	TextureType m_textureType = TextureType::DEFAULT;

	static std::vector<unsigned char> GetSubData(const unsigned char* data, int srcWidth, int srcHeight, int x, int y, int subWidth, int subHeight, bool inversedX = false, bool inversedY = false);


};

#endif // !_TEXTURE_H_

