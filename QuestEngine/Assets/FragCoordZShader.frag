#version 330 core
in vec2 uv;
in vec3 normal;
in vec3 pos;
in vec3 viewPos;
in vec4 glPos;

out vec4 FdfragColor;

void main()
{    
    FdfragColor = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z,1.0);
};