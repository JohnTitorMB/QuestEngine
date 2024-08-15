#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 uViewPos;

out vec2 uv;
out vec3 viewPos;
out vec3 pos;

void main()
{
	  // Transform vertex from local space to NDC space
    vec4 vertexInLocalSpace = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vec4 vertexInWorldPos = vertexInLocalSpace * model;
    vec4 vertexInClipSpace = vertexInWorldPos * view * projection;

    gl_Position = vec4(vertexInClipSpace.x, vertexInClipSpace.y, vertexInClipSpace.w, vertexInClipSpace.w);
    uv = aUv;
    viewPos = uViewPos;
    pos = vec3(vertexInWorldPos.x,vertexInWorldPos.y,vertexInWorldPos.z);
}