#version 330 core
#include "ColorManagement.glsl"
in vec2 uv;
out vec4 fragColor;

struct Material 
{
    vec4 color;

    sampler2D texture;
    vec4 textureST;
    int textureColorSpace;
    float intensity;
}; 
uniform Material material;
void main()
{    
    vec4 color = material.color * textureCs(material.texture, uv * material.textureST.zw + material.textureST.xy, material.textureColorSpace);
    color.rgb *=material.intensity;
    color.rgb = ConvertColor(color.rgb, colorSpaceIn, colorSpaceOut);
    fragColor = color;
};