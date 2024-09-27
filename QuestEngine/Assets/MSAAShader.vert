#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 uViewPos;


out vec2 uv;

void main()
{
	  // Transform vertex from local space to NDC space
    vec4 vertexInLocalSpace = vec4(aPos.x, aPos.y, aPos.z, 1.0);

    gl_Position = vec4(vertexInLocalSpace.x, vertexInLocalSpace.y, vertexInLocalSpace.z, vertexInLocalSpace.w);
    uv = aUv;
}