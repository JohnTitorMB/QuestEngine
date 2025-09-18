#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aNormal;
uniform mat4 model;
uniform mat4 mvp; 

out vec4 pos;

void main()
{
    // Transform vertex from local space to NDC space
    vec4 vertexInLocalSpace = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vec4 vertexInClipSpace = vertexInLocalSpace * mvp;
    pos = vertexInLocalSpace * model;

    gl_Position = vec4(vertexInClipSpace.x, vertexInClipSpace.y, vertexInClipSpace.z, vertexInClipSpace.w);
}