#version 330 core
#include "ColorManagement.glsl"
in vec2 uv;

uniform sampler2D source;
uniform int sourceColorSpace;

uniform sampler2D HDR;
uniform int HDRColorSpace;

uniform float intensity;

out vec4 fragColor;

void main()
{
	vec3 color = textureCs(source, uv,sourceColorSpace).rgb;
	color += intensity * texture2D(HDR, uv).rgb;

    fragColor = vec4(color, 1.0);

    fragColor.rgb = ConvertColor(fragColor.rgb, colorSpaceIn, colorSpaceOut).rgb;
};

