#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 cubeMapCoord;

void main()
{
	  // Transform vertex from local space to NDC space
    vec4 vertexInLocalSpace = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vec4 vertexInNDCSpace = vertexInLocalSpace * model * view * projection;

    gl_Position = vec4(vertexInNDCSpace.x, vertexInNDCSpace.y, vertexInNDCSpace.z, vertexInNDCSpace.w);
    cubeMapCoord = vec3(aPos.x, aPos.y, aPos.z);
}