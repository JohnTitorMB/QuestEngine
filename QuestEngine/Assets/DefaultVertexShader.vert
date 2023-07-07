#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat3 model;
uniform mat3 view;
uniform mat3 toNDCSpace;
	
void main()
{
	  // Transform vertex from local space to NDC space
    vec3 vertexInLocalSpace = vec3(aPos.x, aPos.y, 1.0);
    vec3 vertexInNDCSpace = (vec4(vertexInLocalSpace, 1.0) * mat4(model) * mat4(view) * mat4(toNDCSpace)).xyz;

    gl_Position = vec4(vertexInNDCSpace.x, vertexInNDCSpace.y, aPos.z, 1.0);
}