#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aNormal;
uniform mat4 model, view, projection;
uniform mat3 normalMatrix;
uniform vec3 uViewPos;

#define SPOT_DIRECTIONNAL_LIGHT_COUNT 5  
uniform mat4 lightsVP[SPOT_DIRECTIONNAL_LIGHT_COUNT];
uniform int spotLightCount = 0, directionalLightCount = 0;

out vec2 uv;
out vec3 normal, pos, viewPos;
out float zView;

out vec4 posLightSpaceArray[SPOT_DIRECTIONNAL_LIGHT_COUNT];

void main()
{
    // Transform vertex from local space to NDC space
    vec4 vertexInLocalSpace = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vec4 vertexInWorldPos = vertexInLocalSpace * model;
    gl_Position = vertexInWorldPos * view * projection;

    uv = aUv;

    pos = vec3(vertexInWorldPos.x,vertexInWorldPos.y,vertexInWorldPos.z);
    normal = normalize(aNormal * normalMatrix);
    viewPos = uViewPos;
    zView = (vertexInWorldPos * view).z;

    vec4 posH = vec4(pos.x, pos.y, pos.z, 1.0);
    
    if(directionalLightCount > 0)
        posLightSpaceArray[0] = (posH * lightsVP[0]);

    for(int i = 1; i < spotLightCount + 1; i++)
        posLightSpaceArray[i] = (posH * lightsVP[i]); 
}