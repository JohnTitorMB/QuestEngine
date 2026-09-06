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

    sampler2D normalTexture;
    vec4  normalTextureST;
    int  normalTextureColorSpace;

    sampler2D alphaTexture;
    vec4 alphaTextureST;
    int  alphaTextureColorSpace;
}; 
uniform Material material;

uniform sampler2D depthTexture;
uniform vec2 invViewport;

in vec3 TangentSpotLightDir[SPOT_LIGHT_COUNT];
in vec3 TangentPointLightDir[POINT_LIGHT_COUNT];
in vec3 TangentDirectionalLightDir;

in vec3 TangentViewPos;
in vec3 TangentFragPos;


vec3 ComputeDirectionalLightColor(DirectionalLight dLight, vec3 _normal, float shadow)
{
    // -------------------------
    // Textures & material
    // -------------------------
    vec4 albedoSample = textureCs(material.albedoTexture,
        uv * material.albedoTextureST.zw + material.albedoTextureST.xy,
        material.albedoTextureColorSpace);

    vec3 albedo = albedoSample.rgb * material.albedoColor.rgb;

    float metallic = material.metallicFactor *
        texture2D(material.metallicTexture,
        uv * material.metallicTextureST.zw + material.metallicTextureST.xy).r;

    float roughnessInput = material.roughnessFactor *
        texture2D(material.roughnessTexture,
        uv * material.roughnessTextureST.zw + material.roughnessTextureST.xy).r;

    // Clamp safe
    float roughness = clamp(roughnessInput, 0.04, 1.0);
    float alpha = roughness * roughness;

    // -------------------------
    // Base colors
    // -------------------------
    vec3 f0 = vec3(0.04);
    vec3 F0 = mix(f0, albedo, metallic);

    // -------------------------
    // Directions
    // -------------------------
    vec3 N = normalize(_normal);
    vec3 V = normalize(TangentViewPos - TangentFragPos);
    vec3 L = normalize(-TangentDirectionalLightDir);
    vec3 H = normalize(V + L);

    // -------------------------
    // Dot products (SAFE)
    // -------------------------
    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotH = max(dot(N, H), 0.0);
    float VdotH = max(dot(V, H), 0.0);

    // Early out
    if (NdotL <= 0.0 || NdotV <= 0.0)
        return vec3(0.0);

    // -------------------------
    // Fresnel (Schlick)
    // -------------------------
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);

    // -------------------------
    // GGX Distribution
    // -------------------------
    float a2 = alpha * alpha;
    float denomD = (NdotH * NdotH * (a2 - 1.0) + 1.0);
    float D = a2 / max(PI * denomD * denomD, 0.001);

    // -------------------------
    // Geometry (Smith GGX)
    // -------------------------
    float k = (roughness + 1.0);
    k = (k * k) / 8.0;

    float G_L = NdotL / (NdotL * (1.0 - k) + k);
    float G_V = NdotV / (NdotV * (1.0 - k) + k);
    float G = G_L * G_V;

    // -------------------------
    // Specular
    // -------------------------
    float denomSpec = max(4.0 * NdotL * NdotV, 0.001);
    vec3 specular = (D * G * F) / denomSpec;

    // -------------------------
    // Diffuse
    // -------------------------

    vec3 kS = F;
    vec3 kD = (1.0 - kS) * (1.0 - metallic);

    vec3 diffuse = kD * albedo * (1.0 / PI);

    // -------------------------
    // Final lighting
    // -------------------------
    vec3 lightColor = vec3(dLight.color.r, dLight.color.g, dLight.color.b) * dLight.intensity;

    vec3 result = (diffuse + specular) * lightColor * NdotL;

    // Safety anti-NaN
    result = max(result, vec3(0.0));

    return result * shadow;
}

float ComputeDirectionalShadow(vec4 _posLightSpace, vec3 _normal, DirectionalLight dLight)
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
    float bias = max(maxBias * (1.0 - dot(_normal, -lightDir)), minBias);  
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
   // vec3 normalFrag = texture(material.normalTexture, uv).rgb;

    vec3 normalFrag = textureCs(material.normalTexture, uv * material.normalTextureST.zw + material.normalTextureST.xy,material.normalTextureColorSpace).rgb;
    normalFrag = normalize(normalFrag * 2.0 - 1.0);   

    if(directionalLightCount > 0)
    {
        float shadow = ComputeDirectionalShadow(posLightSpaceArray[0], normal, directionalLight);    
        color += ComputeDirectionalLightColor(directionalLight, normalFrag, shadow);
    }
    
    float alpha = material.alpha * texture(material.alphaTexture, uv * material.alphaTextureST.zw + material.alphaTextureST.xy).r;

    color.rgb += material.emissiveColor.rgb * textureCs(material.emissiveTexture, uv * material.emissiveTextureST.zw + material.emissiveTextureST.xy, material.emissiveTextureColorSpace).rgb;

    color.rgb = ConvertColor(color.rgb, colorSpaceIn, colorSpaceOut);

    FdfragColor = vec4(color.r,color.g,color.b,alpha);

  //  FdfragColor = vec4(normalFrag,1.0);
};
