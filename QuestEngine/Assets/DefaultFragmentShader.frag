#version 330 core
in vec2 uv;
out vec4 FdfragColor;
uniform sampler2D text;
void main()
{
//	FdfragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);

	FdfragColor = texture(text, uv);
};