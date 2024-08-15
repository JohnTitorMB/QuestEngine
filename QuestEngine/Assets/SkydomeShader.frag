#version 330 core
in vec2 uv;
in vec3 pos;

uniform vec4 colorA;
uniform vec4 colorB;
uniform vec4 sunColor;
uniform vec3 lightDirection;
uniform float horizonOffset = 0.53;
uniform float exposure = 8;
uniform float sunSize = 2;
uniform float sunSmoothThreshold = 0.2;

out vec4 fragColor;

void main()
{
	float interpolation = clamp(uv.y/horizonOffset, 0, 1);
	interpolation = pow(interpolation, exposure);
	fragColor = mix(colorA, colorB, interpolation);

	vec3 viewDirection = normalize(pos);
	float dProduct = dot(-lightDirection, viewDirection);

	float angle = acos(dProduct);
	float value = smoothstep(0.0, 1.0, (angle - sunSize) / sunSmoothThreshold); 

	fragColor = mix(sunColor, fragColor, value);
};

