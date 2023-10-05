#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uv;

void main()
{
	  // Transform vertex from local space to NDC space
    vec4 vertexInLocalSpace = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vec4 vertexInNDCSpace =vertexInLocalSpace * model * view * projection;

    gl_Position = vec4(vertexInNDCSpace.x, vertexInNDCSpace.y, vertexInNDCSpace.z, vertexInNDCSpace.w);
    uv = aUv;
}