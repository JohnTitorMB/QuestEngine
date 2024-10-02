#version 330 core
in vec2 uv;

uniform float pixelSpacement;
uniform sampler2D texture2D;
out vec4 fragColor;

#define KERNEL_MAX_SIZE 256
uniform float kernel[KERNEL_MAX_SIZE];
uniform int kernelHalfWidth = 1;
uniform int kernelWidth = 3;


void main()
{
	vec4 color = vec4(0,0,0,0); 

	for(int i = -kernelHalfWidth; i <= kernelHalfWidth; i++)
	{
		vec2 sampledUV = uv + vec2(0, i * pixelSpacement);
	//	sampledUV = clamp(sampledUV, vec2(0.0), vec2(1.0));
		color += texture(texture2D, uv + vec2(0, i * pixelSpacement)) * kernel[i + kernelHalfWidth];	
	}

	fragColor = color;
};

