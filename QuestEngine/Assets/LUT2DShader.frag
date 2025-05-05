#version 330 core
#include "ColorManagement.glsl"

in vec2 uv;
out vec4 fragColor;
uniform sampler2D texture2D;
uniform int texture2DColorSpace;
uniform sampler2D lut;
uniform int lutColorSpace;



uniform float lutSize;

vec3 ApplyLUT(vec3 color, sampler2D lut, float size)
{
    color = clamp(color, 0.0, 1.0);
    
    float blueIndex = color.b * (size - 1.0);
    float sliceIndex = floor(blueIndex);
    float sliceInterp = fract(blueIndex);

    float tileSize = 1.0 / size;
    float halfPixel = 0; 
    float range = color.r * (1.0f - halfPixel - halfPixel);
    float range2 = (1.0f - color.g) * (1.0f - halfPixel - halfPixel);

    vec2 uv0;
    uv0.x = ((halfPixel + range) + sliceIndex) * tileSize;
    uv0.y = halfPixel + range2;

    vec2 uv1;
    uv1.x = ((halfPixel + range) + sliceIndex + 1.0) * tileSize;
    uv1.y = halfPixel + range2;


    vec3 c0 = texture(lut, uv0).rgb;
    vec3 c1 = texture(lut, uv1).rgb;

    return mix(c0, c1, sliceInterp);
}

void main()
{
    vec4 color = texture(texture2D, uv);
    color.rgb = ApplyLUT(color.rgb, lut, lutSize);
    fragColor = color;
};