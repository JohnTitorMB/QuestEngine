#version 330 core
#include "ColorManagement.glsl"

const float PI = 3.14159265359;

in vec2 uv;
in vec3 normal;
in vec3 pos;
in vec3 viewPos;
 
layout(location=0) out vec4 FdfragColor;    // SceneColor 
layout(location=1) out vec4 outBehind;   // BehindRefracted1

uniform vec2 textureTilling = vec2(1,1);
uniform vec2 textureOffset = vec2(0,0);
uniform vec4 globalAmbiantColor;
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
uniform int directionalLightCount = 0;



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
#define POINT_LIGHT_COUNT 4  
uniform PointLight pointLights[POINT_LIGHT_COUNT];
uniform int pointLightCount = 0;


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
#define SPOT_LIGHT_COUNT 4  
uniform SpotLight spotLights[SPOT_LIGHT_COUNT];
uniform int spotLightCount = 0;

#define SPOT_DIRECTIONNAL_LIGHT_COUNT 5  
in vec4 posLightSpaceArray[SPOT_DIRECTIONNAL_LIGHT_COUNT];


struct Material 
{
    vec4 ambientColor;  
    vec4 albedoColor;  
    vec4 emissiveColor;
    float roughnessFactor;
    float metallicFactor;
    float alpha;

    sampler2D ambiantTexture;
    vec4 ambiantTextureST;
    int ambiantTextureColorSpace;

    sampler2D albedoTexture;
    vec4 albedoTextureST;
    int albedoTextureColorSpace;

    sampler2D emissiveTexture;
    vec4 emissiveTextureST;
    int emissiveTextureColorSpace;

    sampler2D roughnessTexture;
    vec4 roughnessTextureST;
    int roughnessTextureColorSpace;

    sampler2D metallicTexture;
    vec4  metallicTextureST;
    int  metallicTextureColorSpace;

    sampler2D alphaTexture;
    vec4 alphaTextureST;
    int  alphaTextureColorSpace;
}; 
uniform Material material;

uniform sampler2D depthTexture;
uniform vec2 invViewport;


vec3 ComputeDirectionalLightColor(DirectionalLight dLight, float shadow)
{
    vec4 albedoColor = material.albedoColor * textureCs(material.albedoTexture, uv * material.albedoTextureST.zw + material.albedoTextureST.xy, material.albedoTextureColorSpace);
    float metallicFactor = material.metallicFactor * texture2D(material.metallicTexture, uv * material.metallicTextureST.zw + material.metallicTextureST.xy, material.metallicTextureColorSpace).r;
    vec3 baseDiffuseColor = mix(albedoColor.rgb, vec3(0.0f), metallicFactor);

    vec3 f0 = vec3(0.04f);
    vec3 baseSpecularColor = mix(f0, albedoColor.rgb, metallicFactor);

    float roughnessFactor = material.roughnessFactor * texture2D(material.roughnessTexture, uv * material.roughnessTextureST.zw + material.roughnessTextureST.xy, material.roughnessTextureColorSpace).r;
    float minRoughness = 0.04f;
    float roughness = pow(clamp(roughnessFactor,minRoughness, 1.0f),2.0f);

    float reflectance = max(max(baseSpecularColor.r, baseSpecularColor.g), baseSpecularColor.b);
    
    vec3 Reflectance = baseSpecularColor;
    vec3 Refletance90 = vec3(clamp(reflectance * 25.0f, 0.0f, 1.0f));


    vec3 viewDirection = normalize(viewPos - pos);
    vec3 halfwayDirection = normalize(-dLight.direction + viewDirection);

    vec3 F = Reflectance + (Refletance90 - Reflectance) * pow(clamp(1.0-(dot(viewDirection, -dLight.direction+ viewDirection)),0.0f,1.0f),5.0f);
    
    float F90 = 2.0f * pow(dot(-dLight.direction, halfwayDirection),2.0f)*pow(roughness,2)-0.5f;
    vec3 diffuseBurley = baseDiffuseColor * (1.0f / PI) * (1.0f + F90 * pow(1.0f - dot(normal, -dLight.direction),5.0f))*
                                                          (1.0f + F90 * pow(1.0f - dot(normal, viewDirection),5.0f));
    vec3 lightDiffuse = (1.0f - F)*diffuseBurley;

    float attenuationL = 2.0f * dot(normal,-dLight.direction) / (dot(normal, -dLight.direction) + sqrt(pow(roughness,2.0f) + (1.0 - pow(roughness,2.0f)) * pow(dot(normal, -dLight.direction),2.0f)));
    float attenuationV = 2.0f * dot(normal,viewDirection) / (dot(normal, viewDirection) + sqrt(pow(roughness,2.0f) + (1.0 - pow(roughness,2.0f)) * pow(dot(normal, viewDirection),2.0f)));
    
    float G = attenuationL * attenuationV; 
    float NdotH = dot(normal, halfwayDirection);
    float D = pow(roughness,2.0f) * 1.0f/(PI * pow((NdotH * pow(roughness,2.0f)  - NdotH) * NdotH + 1.0f,2.0f));
    vec3 lightSpecular = F*G*D* 1.0F/ (4.0f * dot(normal, -dLight.direction)*dot(normal,viewDirection));

    vec3 lightColor = max(dot(normal, -dLight.direction),0) * vec3(dLight.color.r, dLight.color.g, dLight.color.b) *  dLight.intensity * (lightDiffuse + lightSpecular);
    return lightColor;
}

float ComputeDirectionalShadow(vec4 _posLightSpace, DirectionalLight dLight)
{   

    int blurResolution = dLight.blurResolution;
    if(blurResolution == -1)
        return 1.0f;
    vec3 projCoords = _posLightSpace.xyz / _posLightSpace.w;

    if(projCoords.z > 1.0f)
        return 1.0f;

    float near = dLight.shadowParam.z;
    float far = dLight.shadowParam.w;
    float minBias = dLight.shadowParam.x;
    float maxBias = dLight.shadowParam.y;
    vec3 lightDir = dLight.direction;

    projCoords = projCoords * 0.5 + 0.5; 
    vec2 depthUv = projCoords.xy;
    float currentDepth = projCoords.z * (far - near) + near;
    float bias = max(maxBias * (1.0 - dot(normal, -lightDir)), minBias);  
    if(blurResolution == 0)
    {
        float depthN = texture(dLight.lightDepthMap, depthUv).r;
        float depth = depthN * (far - near) + near;

        if(currentDepth - bias > depth)
            return 0.0f;

        return 1.0f;
    }
    else
    {
        vec2 texelSize = 1.0 / textureSize(dLight.lightDepthMap, 0);
        float value = 0.0f;
        for(int x = -blurResolution; x <= blurResolution; ++x)
        {
            for(int y = -blurResolution; y <= blurResolution; ++y)
            {        
                vec2 relativeUv = depthUv + vec2(x, y) * texelSize;
                float depthN = texture(dLight.lightDepthMap, relativeUv).r;
                float depth = depthN * (far - near) + near;
                if(currentDepth - bias <= depth)
                    value += 1.0f;
            }    
        }   

        float sampleCount = float((2 * blurResolution + 1) * (2 * blurResolution + 1));
        value /= sampleCount;
    
        return value;
    }   
}

void main()
{   
    vec4 mAmbiant = material.ambientColor * textureCs(material.ambiantTexture, uv * material.ambiantTextureST.zw + material.ambiantTextureST.xy, material.ambiantTextureColorSpace);
    vec4 ambientColor = globalAmbiantColor * mAmbiant;

    vec3 color = vec3(ambientColor.r, ambientColor.g, ambientColor.b);

    if(directionalLightCount > 0)
    {
        float shadow = ComputeDirectionalShadow(posLightSpaceArray[0], directionalLight);    
        color += ComputeDirectionalLightColor(directionalLight, shadow);
    }
    
    float alpha = material.alpha * texture(material.alphaTexture, uv * material.alphaTextureST.zw + material.alphaTextureST.xy).r;

    color.rgb += material.emissiveColor.rgb * textureCs(material.emissiveTexture, uv * material.emissiveTextureST.zw + material.emissiveTextureST.xy, material.emissiveTextureColorSpace).rgb;

    color.rgb = ConvertColor(color.rgb, colorSpaceIn, colorSpaceOut);

    FdfragColor = vec4(color.r,color.g,color.b,alpha);

    vec2 uv =  gl_FragCoord.xy * invViewport;
    float refractedDepth = texture(depthTexture, uv).r;
  
    const float epsilon = 1e-4;

    if (refractedDepth < 0.0) {
        outBehind = vec4(0.0);
    } else {
        float fragDepth = gl_FragCoord.z; 
        bool behind = (fragDepth - refractedDepth) > -epsilon;
        outBehind = behind ? FdfragColor : vec4(0.0);
    }

    //outBehind = vec4(1.0f);
};
