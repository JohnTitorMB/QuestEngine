#version 330 core

uniform sampler2D texture2D;
in vec2 uv;
uniform vec2 pixelSpacement;

#define KERNEL_MAX_SIZE 1021
uniform int radius;
uniform float kernel[KERNEL_MAX_SIZE];

out vec4 fragColor;

void main()
{
	vec4 color = texture(texture2D, uv -radius * pixelSpacement) * kernel[0]; 

	for(int i = -radius + 1; i <= radius; i += 2)
	{
		int kernelIndex = radius + i;
		float w0 = kernel[kernelIndex];
		float w1 = kernel[kernelIndex+1];
		float wSum = w0 + w1;
		float t = w1 / wSum;
		color += texture(texture2D, uv + (i+t) * pixelSpacement) * wSum;	
	}

	fragColor = color;
};


