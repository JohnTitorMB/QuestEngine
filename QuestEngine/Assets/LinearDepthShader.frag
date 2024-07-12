#version 330 core
out vec4 FdfragColor;

uniform float viewNear;
uniform float viewFar;

void main()
{        
    float z = gl_FragCoord.z;
    float linearZ = (-viewNear * z) / (z*(viewFar-viewNear)-viewFar);

    FdfragColor = vec4(linearZ, linearZ, linearZ,1.0);
};