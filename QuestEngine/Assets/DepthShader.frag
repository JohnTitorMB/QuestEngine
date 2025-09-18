#version 330 core

layout(location = 0) out float outRefractDepth;


void main()
{
    outRefractDepth = gl_FragCoord.z;
}
