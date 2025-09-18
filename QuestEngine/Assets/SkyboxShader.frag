#version 330 core
#include "ColorManagement.glsl"
in vec3 cubeMapCoord;
layout(location=0) out vec4 FdfragColor;    
layout(location=1) out vec4 outBehind;   // BehindRefractor1
uniform samplerCube cubemap;
uniform int cubemapColorSpace;

uniform sampler2D depthTexture;
uniform vec2 invViewport;


void main()
{
	vec4 color = textureCubemapCs(cubemap, cubeMapCoord, cubemapColorSpace);
	color.rgb = ConvertColor(color.rgb, colorSpaceIn, colorSpaceOut);
	FdfragColor = color;

	vec2 uv =  gl_FragCoord.xy * invViewport;
    float refractedDepth = texture(depthTexture, uv).r;

    
  const float epsilon = 1e-4;

   if (refractedDepth < 0.0) {
        outBehind = vec4(0.0);
    } else {
        float fragDepth = gl_FragCoord.z; 
        bool behind = (fragDepth - refractedDepth) > -epsilon;
        outBehind = behind ? FdfragColor : vec4(0.0);
    }
};