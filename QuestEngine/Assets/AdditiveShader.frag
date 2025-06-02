#version 330 core
#include "ColorManagement.glsl"
in vec2 uv;

uniform sampler2D lowTexture;
uniform int lowTextureColorSpace;

uniform sampler2D highTexture;
uniform int highTextureColorSpace;

out vec4 fragColor;

void main()
{
	 vec3 low = texture(lowTexture, uv).rgb;
	 vec3 high = texture(highTexture, uv).rgb;

	 fragColor = vec4(low + high, 1.0f);
};

