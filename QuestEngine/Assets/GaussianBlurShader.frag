#version 330 core

uniform sampler2D texture2D;
in vec2 uv;
uniform vec2 pixelSpacement;

#define KERNEL_MAX_SIZE 961
uniform int radius;
uniform int kernelWidth;
uniform float kernel[KERNEL_MAX_SIZE];

out vec4 fragColor;

void main()
{
	vec4 color = vec4(0,0,0,0); 

	for(int j = -radius; j <= radius; j++)
	{
		for(int i = -radius; i <= radius; i++)
		{
			int kernelIndex = (j+radius) * kernelWidth + i+radius;
			color += texture(texture2D, uv + vec2(i, j) * pixelSpacement) * kernel[kernelIndex];	
		}
	}

	fragColor = color;
};


