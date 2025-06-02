#version 330 core
#include "ColorManagement.glsl"

in vec2 uv;
out vec4 fragColor;

uniform sampler2D lowTexture;  
uniform int lowTextureColorSpace;

uniform sampler2D highTexture; 
uniform int highTextureColorSpace;

uniform vec2 pixelSpacement; 

vec3 BoxBlurFilter(sampler2D text,vec2 uv, vec2 pixelSize,float delta) {
    
    vec2 offset = pixelSize * vec2(delta);
    vec3 s =
        texture(text, uv + vec2(-offset.x, -offset.y)).rgb +
        texture(text, uv + vec2(offset.x, -offset.y)).rgb +
        texture(text, uv + vec2(offset.x, offset.y)).rgb +
        texture(text, uv + vec2(-offset.x, offset.y)).rgb;

    return s * 0.25;
}

void main() {
    
    vec3 low = BoxBlurFilter(lowTexture, uv,pixelSpacement, 0.5f);
    vec3 high = texture(highTexture, uv).rgb;

    fragColor = vec4(low + high, 1.0); 

}