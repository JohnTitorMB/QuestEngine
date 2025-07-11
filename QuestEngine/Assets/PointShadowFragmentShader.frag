#version 330 core

in vec4 pos;              
uniform vec3 lightPos;    
uniform float far;        

void main()
{
    float lightDistance = length(pos.xyz - lightPos);
    gl_FragDepth = lightDistance / far;
}
