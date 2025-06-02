#version 330 core
#include "ColorManagement.glsl"
in vec2 uv;

uniform sampler2D texture2D;
uniform int texture2DColorSpace;

uniform float threshold = 1.0;
uniform vec3 curve; 
const float EPSILON = 1e-5;

out vec4 fragColor;




// Quadratic color thresholding
// Adapted from Unity Post-Processing Stack v2
// Source: https://github.com/Unity-Technologies/Graphics
// License: MIT
// Original function: QuadraticThreshold (Colors.hlsl)
// curve = (threshold - knee, knee * 2, 0.25 / (knee + episilon))
vec3 QuadraticThreshold(vec3 color, float _threshold, vec3 _curve)
{
    // Pixel brightness
    float maxValue = max(color.r, max(color.g, color.b));

    // Under-threshold part: quadratic curve
    float rq = clamp(maxValue - _curve.x, 0.0, _curve.y);
    rq = _curve.z * rq * rq;

    // Combine and apply the brightness response curve.
    color *= max(rq, maxValue - _threshold) / max(maxValue, EPSILON);

    return color;
}





void main()
{
     vec3 extracted = QuadraticThreshold(textureCs(texture2D, uv, texture2DColorSpace).rgb, threshold, curve);
    
    //Return color in linear space
    fragColor = vec4(extracted, 1.0);

};

