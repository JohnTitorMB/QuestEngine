#version 330 core
in vec2 uv;

uniform sampler2D texture2D;
uniform vec4 tintColor;

out vec4 fragColor;

void main()
{
discard;
	fragColor = vec4(uv.x, uv.x, uv.x, 1.0f);
	float gamma = 2.2f;
	fragColor.rgb = pow(fragColor.rgb,vec3(1.0f/gamma));
};

