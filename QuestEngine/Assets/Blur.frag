#version 330 core
in vec2 uv;

uniform vec2 pixelSpacement;
uniform sampler2D texture2D;
out vec4 fragColor;

#define KERNEL_HalfSize 16
#define KERNEL_Size 33
#define KERNEL_SquaredSize 1089
#define SIGMA 5.0

float kernel[KERNEL_SquaredSize];


float GaussianAtCoord(vec2 coord)
{
    float sigma = SIGMA;

	sigma = float(KERNEL_Size - 1) / 6.0;
    float gaussian = exp(-(coord.x * coord.x + coord.y * coord.y) / (2.0 * sigma * sigma)) / (2.0 * 3.14159265359 * sigma * sigma);
    
    return gaussian;
}

void main()
{
	vec4 color = vec4(0,0,0,0); 

	for(int j = -KERNEL_HalfSize; j <= KERNEL_HalfSize; j++)
	{
		for(int i = -KERNEL_HalfSize; i <= KERNEL_HalfSize; i++)
		{
			kernel[(j+KERNEL_HalfSize) * KERNEL_Size + i + KERNEL_HalfSize] = GaussianAtCoord(vec2(i,j));
		}
	}

	float sum = 0;
	for(int i = 0; i < KERNEL_SquaredSize; i++)
	{
	    sum += kernel[i];
	}

	float divideCoef = 1.0f/sum;

	for(int j = -KERNEL_HalfSize; j <= KERNEL_HalfSize; j++)
	{
		for(int i = -KERNEL_HalfSize; i <= KERNEL_HalfSize; i++)
		{
			color += texture(texture2D, uv + vec2(i, j) * pixelSpacement) * kernel[(j+KERNEL_HalfSize) * KERNEL_Size + i+KERNEL_HalfSize]* divideCoef;	
		}
	}

	fragColor = color;
};

