#version 330

out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;

void main()
{
	FragColor = texture(skybox, texCoords);

	FragColor = vec4(1,1,1,1);
}