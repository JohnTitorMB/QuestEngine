#version 330 core
in vec2 uv;

uniform sampler2D texture2D;
uniform vec3 lightColor;

out vec4 fragColor;

void main()
{
	float gamma = 2.2f;
	vec4 color = texture(texture2D, uv);
	
	// 1. Gamma Space to linear Space
	color.rgb = pow(color.rgb, vec3(gamma));
	
	// 2. Data Processing
	color.rgb *= lightColor;
	
	// 3. Linear Space to Gamma Space (Gamma correction)
	color.rgb = pow(color.rgb, vec3(1.0f/gamma));

	fragColor = color;
};

