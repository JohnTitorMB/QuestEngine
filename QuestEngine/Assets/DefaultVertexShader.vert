#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat3 model;
uniform mat3 view;
uniform mat3 toNDCSpace;
	
void main()
{
	/*Transform vertice from localSpace into NDCSpace*/
	vec3 vertexInlocalSpace = vec3(aPos.x,aPos.y,1.0f);
	vec3 vertexInNDCSpace = toNDCSpace * view * model * vertexInlocalSpace;

	gl_Position = vec4(vertexInNDCSpace.x, vertexInNDCSpace.y, aPos.z, 1.0);
}