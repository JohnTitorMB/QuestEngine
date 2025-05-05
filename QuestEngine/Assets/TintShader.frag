#version 330 core
#include "ColorManagement.glsl"
in vec2 uv;

uniform sampler2D texture2D;
uniform vec4 tintColor;
uniform int texture2DColorSpace;

out vec4 fragColor;

void main()
{
	vec4 color = textureCs(texture2D, uv, texture2DColorSpace) * tintColor;
	color.rgb = ConvertColor(color.rgb, colorSpaceIn, colorSpaceOut);
	fragColor = color;
};

