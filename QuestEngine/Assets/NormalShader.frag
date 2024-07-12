#version 330 core
in vec3 normal;
out vec4 FdfragColor;



void main()
{    
    vec3 normalizedNormal = normalize(normal);
    normalizedNormal = normalizedNormal * 0.5 + 0.5;

    FdfragColor = vec4(normalizedNormal.r,normalizedNormal.g,normalizedNormal.b,1.0);
};