#version 330 core
in vec3 cubeMapCoord;
out vec4 FdfragColor;
uniform samplerCube cubemap;

void main()
{
	FdfragColor = texture(cubemap, cubeMapCoord);
};