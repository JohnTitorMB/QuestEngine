#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec4 aTangent;

uniform mat4 model, view, projection;
uniform mat3 normalMatrix;
uniform vec3 uViewPos;

#define SPOT_DIRECTIONNAL_LIGHT_COUNT 5  
uniform mat4 lightsVP[SPOT_DIRECTIONNAL_LIGHT_COUNT];
uniform int spotLightCount = 0, directionalLightCount = 0;
uniform int pointLightCount = 0;

out vec2 uv;
out vec3 normal, pos, viewPos;
out float zView;

out vec4 posLightSpaceArray[SPOT_DIRECTIONNAL_LIGHT_COUNT];


#define POINT_LIGHT_COUNT 4  
#define SPOT_LIGHT_COUNT 4  

out vec3 TangentSpotLightDir[SPOT_LIGHT_COUNT];
out vec3 TangentPointLightDir[POINT_LIGHT_COUNT];
out vec3 TangentDirectionalLightDir;

out vec3 TangentViewPos;
out vec3 TangentFragPos;

struct DirectionalLight 
{
    vec4 color;
    float intensity;
    vec3 direction;

    sampler2D lightDepthMap;
    vec4 shadowParam;
    int blurResolution;
};  
uniform DirectionalLight directionalLight;

struct PointLight  
{
    vec4 color;

    vec3 position;
    float constant;
    float linear;
    float quadratic; 
    float intensity;
    samplerCube lightDepthMap;
    vec4 shadowParam;
    int blurResolution;
    vec2 blurOffset;
};  
uniform PointLight pointLights[POINT_LIGHT_COUNT];

struct SpotLight  
{
    vec4 color;

    vec3 position;
    vec3 direction;
    float constant;
    float linear;
    float quadratic; 
    float intensity;

    float spotCosAngle;
	float spotCosSmoothAngle;
    sampler2D lightDepthMap;
    vec4 shadowParam;
    int blurResolution;
};
uniform SpotLight spotLights[SPOT_LIGHT_COUNT];

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

    vec3 bitangent = cross(aNormal, aTangent.xyz) * aTangent.w;
    bitangent = normalize(bitangent * normalMatrix);
    vec3 tangent = normalize(aTangent.xyz * normalMatrix);

    mat3 TBN = transpose(mat3(tangent, bitangent, normal));
    for(int i = 0; i < SPOT_LIGHT_COUNT; i++)
    {
        TangentSpotLightDir[i] = TBN * spotLights[i].position;
    }

    for(int i = 0; i < POINT_LIGHT_COUNT; i++)
    {
        TangentPointLightDir[i] = TBN * pointLights[i].position;
    }

    TangentDirectionalLightDir = TBN * directionalLight.direction;
    TangentViewPos = TBN * viewPos;
    TangentFragPos = TBN * pos;
}