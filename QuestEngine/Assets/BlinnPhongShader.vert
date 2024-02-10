#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aNormal;
uniform mat4 model;
uniform mat3 normalMatrix;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 uViewPos;

out vec2 uv;
out vec3 normal;
out vec3 pos;
out vec3 viewPos;
void main()
{
	  // Transform vertex from local space to NDC space
    vec4 vertexInLocalSpace = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vec4 vertexInWorldPos = vertexInLocalSpace * model;
    vec4 vertexInClipSpace = vertexInWorldPos * view * projection;

    gl_Position = vec4(vertexInClipSpace.x, vertexInClipSpace.y, vertexInClipSpace.z, vertexInClipSpace.w);
    uv = aUv;

    pos = vec3(vertexInWorldPos.x,vertexInWorldPos.y,vertexInWorldPos.z);
    normal = normalize(aNormal * normalMatrix);
    viewPos = uViewPos;
}