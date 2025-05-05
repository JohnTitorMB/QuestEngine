#version 330 core
in vec2 uv;
out vec4 FdfragColor;



void main()
{    
    FdfragColor = vec4(uv.r,uv.g,0.0f,1.0);
};