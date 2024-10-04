#version 330 core
in vec2 uv;

uniform sampler2D texture2D;
uniform vec4 tintColor;

out vec4 fragColor;

void main()
{
	vec4 color = texture(texture2D, uv) * tintColor;
	fragColor = color;
};

