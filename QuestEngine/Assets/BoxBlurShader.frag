#version 330 core
#include "ColorManagement.glsl"

in vec2 uv;
out vec4 fragColor;
uniform sampler2D texture2D;
uniform int texture2DColorSpace;

uniform vec2 pixelSpacement;

vec3 BoxBlurFilter(sampler2D text,vec2 uv, vec2 pixelSize,float delta) {
    
    vec2 offset = pixelSize * vec2(delta, delta);
    vec3 s =
        texture(text, uv + vec2(-offset.x, -offset.y)).rgb +
        texture(text, uv + vec2(offset.x, -offset.y)).rgb +
        texture(text, uv + vec2(offset.x, offset.y)).rgb +
        texture(text, uv + vec2(-offset.x, offset.y)).rgb;

    return s * 0.25;
}


void main()
{ 
    fragColor = vec4(BoxBlurFilter(texture2D, uv, pixelSpacement, 1.0f), 1.0f);
};