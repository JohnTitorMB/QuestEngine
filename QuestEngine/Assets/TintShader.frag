#version 330 core
in vec2 uv;

uniform float pixelSpacement;
uniform sampler2D texture2D;
out vec4 fragColor;

void main()
{
	vec4 color = vec4(1,0,0,1); 
	fragColor = texture(texture2D, uv) * color;
};

