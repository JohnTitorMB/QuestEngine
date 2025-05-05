#version 330 core
#include "ColorManagement.glsl"
in vec3 cubeMapCoord;
out vec4 FdfragColor;
uniform samplerCube cubemap;
uniform int cubemapColorSpace;

void main()
{
	vec4 color = textureCubemapCs(cubemap, cubeMapCoord, cubemapColorSpace);
	color.rgb = ConvertColor(color.rgb, colorSpaceIn, colorSpaceOut);
	FdfragColor = color;
};