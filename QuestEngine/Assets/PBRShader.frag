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

in vec3 TangentSpotLightPositions[SPOT_LIGHT_COUNT];
in vec3 TangentPointLightPositions[POINT_LIGHT_COUNT];
in vec3 TangentDirectionalLightDir;

in vec3 TangentViewPos;
in vec3 TangentFragPos;

struct PBRInfo
{
    vec3 albedo;
    float metallic;

    float roughness;
    float alpha;
    float squaredAlpha;

    vec3 F0;
    float reflectance90;

    vec3 N;
    vec3 V;
    float NdotV;

    float G_V;
    float oneMinusNdotV5;
};

PBRInfo ComputePBRInfo(vec3 normal, vec3 viewDirection)
{
    PBRInfo info;

    // -------------------------
    // Textures & material
    // -------------------------
    vec4 albedoSample = textureCs(material.albedoTexture, uv * material.albedoTextureST.zw + material.albedoTextureST.xy, material.albedoTextureColorSpace);
    info.albedo = albedoSample.rgb * material.albedoColor.rgb;

    info.metallic = material.metallicFactor * texture2D(material.metallicTexture, uv * material.metallicTextureST.zw + material.metallicTextureST.xy).r;

    float roughnessInput = material.roughnessFactor * texture2D(material.roughnessTexture, uv * material.roughnessTextureST.zw + material.roughnessTextureST.xy).r;
    info.roughness = clamp(roughnessInput, 0.04, 1.0);

    // -------------------------
    // Roughness
    // -------------------------
    info.alpha = info.roughness * info.roughness;
    info.squaredAlpha = info.alpha * info.alpha;

    // -------------------------
    // Fresnel
    // -------------------------
    info.F0 = mix(vec3(0.04), info.albedo, info.metallic);

    float reflectance = max(max(info.F0.r, info.F0.g), info.F0.b);
    info.reflectance90 = clamp(reflectance * 25.0, 0.0, 1.0);

    // -------------------------
    // Directions
    // -------------------------
    info.N = normalize(normal);
    info.V = normalize(viewDirection);
    info.NdotV = max(dot(info.N, info.V), 0.0);

    // -------------------------
    // Geometry (Smith GGX)
    // -------------------------
    float denomGV = info.NdotV + sqrt(info.squaredAlpha + (1.0 - info.squaredAlpha) * info.NdotV * info.NdotV);
    info.G_V = (2.0 * info.NdotV) / max(denomGV, 0.001);

    // -------------------------
    // Disney Burley
    // -------------------------
    float oneMinusNdotV = 1.0 - info.NdotV;
    float oneMinusNdotV2 = oneMinusNdotV * oneMinusNdotV;
    info.oneMinusNdotV5 = oneMinusNdotV2 * oneMinusNdotV2 * oneMinusNdotV;

    return info;
}

vec3 EvaluatePBR(PBRInfo pbrInfo, vec3 _lightColor, vec3 _lightDirection)
{
    // -------------------------
    // Directions
    // -------------------------
    vec3 L = _lightDirection;
    vec3 H = normalize(pbrInfo.V + L);

    // -------------------------
    // Dot products
    // -------------------------
    float NdotL = max(dot(pbrInfo.N, L), 0.0);

    // Early out
    if (NdotL <= 0.0 || pbrInfo.NdotV <= 0.0)
        return vec3(0.0);

    float NdotH = max(dot(pbrInfo.N, H), 0.0);
    float VdotH = max(dot(pbrInfo.V, H), 0.0);
    float LdotH = max(dot(L, H), 0.0);

    // -------------------------
    // Fresnel (Schlick)
    // -------------------------
    float oneMinusVdotH = clamp(1.0 - VdotH, 0.0, 1.0);
    float oneMinusVdotH2 = oneMinusVdotH * oneMinusVdotH;
    float oneMinusVdotH5 = oneMinusVdotH2 * oneMinusVdotH2 * oneMinusVdotH;

    vec3 F = pbrInfo.F0 + (vec3(pbrInfo.reflectance90) - pbrInfo.F0) * oneMinusVdotH5;

    // -------------------------
    // GGX Distribution
    // -------------------------
    float denomD = NdotH * NdotH * (pbrInfo.squaredAlpha - 1.0) + 1.0;
    float D = pbrInfo.squaredAlpha / max(PI * denomD * denomD, 0.001);

    // -------------------------
    // Geometry (Smith GGX)
    // -------------------------
    float G_L = (2.0 * NdotL) / max(NdotL + sqrt(pbrInfo.squaredAlpha + (1.0 - pbrInfo.squaredAlpha) * NdotL * NdotL), 0.001);
    float G = G_L * pbrInfo.G_V;

    // -------------------------
    // Specular (Cook-Torrance GGX)
    // -------------------------
    float denomSpec = max(4.0 * NdotL * pbrInfo.NdotV, 0.001);
    vec3 specular = (D * G * F) / denomSpec;

    // -------------------------
    // Diffuse (Disney Burley)
    // -------------------------
    vec3 kS = F;
    vec3 kD = (1.0 - kS) * (1.0 - pbrInfo.metallic);

    float diffuseF90 = 2.0 * LdotH * LdotH * pbrInfo.roughness - 0.5;

    float oneMinusNdotL = 1.0 - NdotL;
    float oneMinusNdotL2 = oneMinusNdotL * oneMinusNdotL;
    float oneMinusNdotL5 = oneMinusNdotL2 * oneMinusNdotL2 * oneMinusNdotL;

    float lightScatter = 1.0 + diffuseF90 * oneMinusNdotL5;
    float viewScatter = 1.0 + diffuseF90 * pbrInfo.oneMinusNdotV5;

    vec3 diffuse = kD * pbrInfo.albedo * (1.0 / PI) * lightScatter * viewScatter;

    // -------------------------
    // Final lighting
    // -------------------------
    vec3 result = (diffuse + specular) * _lightColor * NdotL;

    return max(result, vec3(0.0));
}

vec3 ComputeDirectionalLightColor(DirectionalLight dLight, PBRInfo pbrInfo, float shadow)
{
    vec3 L = normalize(-TangentDirectionalLightDir);
    vec3 lightColor = dLight.color.rgb * dLight.intensity;

    vec3 pbrResult = EvaluatePBR(pbrInfo, lightColor, L);
    return pbrResult * shadow;
}

vec3 ComputePointLight(PointLight pLight, PBRInfo pbrInfo, vec3 tangentPointLightPos, float shadow)
{
    vec3 lightDiff = TangentFragPos - tangentPointLightPos;

    float lightDistance = length(lightDiff);
    vec3 lightDirection = lightDiff / lightDistance;

    float attenuation = 1.0 / (pLight.constant + pLight.linear * lightDistance + pLight.quadratic * (lightDistance * lightDistance));

    vec3 L = -lightDirection;
    vec3 lightColor = pLight.color.rgb * pLight.intensity;

    vec3 pbrResult = EvaluatePBR(pbrInfo, lightColor, L);
    return pbrResult * attenuation * shadow;
}

vec3 ComputeSpotLight(SpotLight sLight, PBRInfo pbrInfo, vec3 tangentSpotLightPos, float shadow)
{
    vec3 lightDiff = TangentFragPos - tangentSpotLightPos;

    float lightDistance = length(lightDiff);
    vec3 lightDirection = lightDiff / lightDistance;

    float cosAngle = dot(lightDirection, normalize(sLight.direction));

    if (cosAngle <= sLight.spotCosAngle)
        return vec3(0.0);

    float attenuation = 1.0 / (sLight.constant + sLight.linear * lightDistance + sLight.quadratic * (lightDistance * lightDistance));
    float epsilon = sLight.spotCosSmoothAngle - sLight.spotCosAngle;
    float intensity = smoothstep(0.0, 1.0, (cosAngle - sLight.spotCosAngle) / epsilon);

    vec3 L = -lightDirection;
    vec3 lightColor = sLight.color.rgb * sLight.intensity;

    vec3 pbrResult = EvaluatePBR(pbrInfo, lightColor, L);
    return pbrResult * attenuation * intensity * shadow;
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

float ComputeSpotShadow(vec4 posLightSpace, vec3 _normal, SpotLight sLight)
{   
    int blurResolution = sLight.blurResolution;
    if(blurResolution == -1)
        return 1.0f;
    
    vec3 projCoords = posLightSpace.xyz / posLightSpace.w;

    if(projCoords.z > 1.0f)
        return 1.0f;

    projCoords = projCoords * 0.5 + 0.5; 
    
    float near = sLight.shadowParam.z;
    float far = sLight.shadowParam.w;
    float minBias = sLight.shadowParam.x;
    float maxBias = sLight.shadowParam.y; 
    vec3 lightDir = sLight.direction;
    
    vec2 depthUv = projCoords.xy;
    float currentDepth = (-near * projCoords.z) / (projCoords.z*(far-near)-far);
    float bias = max(maxBias * (1.0 - dot(_normal, -lightDir)), minBias);  
    if(blurResolution == 0)
    {
        float depthN = texture(sLight.lightDepthMap, depthUv).r;
        float depth = (-near * depthN) / (depthN*(far-near)-far);

        if(currentDepth - bias <= depth)
            return 1.0f;
        else
            return 0.0f;
    }
    else
    {
        vec2 texelSize = 1.0 / textureSize(sLight.lightDepthMap, 0);
        float value = 0.0f;
        for(int x = -blurResolution; x <= blurResolution; ++x)
        {
            for(int y = -blurResolution; y <= blurResolution; ++y)
            {        
                vec2 relativeUv = depthUv + vec2(x, y) * texelSize;
                float depthN = texture(sLight.lightDepthMap, relativeUv).r;

                float depth = (-near * depthN) / (depthN*(far-near)-far);
                if(currentDepth - bias <= depth)
                    value += 1.0f;
            }    
        }   
        
        float sampleCount = float((2 * blurResolution + 1) * (2 * blurResolution + 1));
        value /= sampleCount;
    
        return value;
    }
}

float ComputePointShadow(vec3 worldPos, vec3 _normal, PointLight pLight)
{
    int blurResolution = pLight.blurResolution;
    if(blurResolution == -1)
        return 1.0f;
    
    vec3 lightDiff = worldPos - pLight.position;
    vec3 lightDir = normalize(lightDiff);

    float near = pLight.shadowParam.z;
    float far = pLight.shadowParam.w;
    float minBias = pLight.shadowParam.x;
    float maxBias = pLight.shadowParam.y;

    vec3 absDir = abs(lightDiff);
    vec3 axis;
    if (absDir.x > absDir.y && absDir.x > absDir.z)
        axis = vec3(sign(lightDiff.x), 0.0, 0.0);
    else if (absDir.y > absDir.z)
        axis = vec3(0.0, sign(lightDiff.y), 0.0);
    else
        axis = vec3(0.0, 0.0, sign(lightDiff.z));

    float z = dot(lightDiff, axis);
    if (z < near || z > far)
        return 1.0;
    float currentDepth = length(lightDiff);
    float bias = max(maxBias * (1.0 - dot(_normal, -lightDir)), minBias);  
    if(blurResolution == 0)
    {
        float depthN = texture(pLight.lightDepthMap, vec3(lightDiff.x, -lightDiff.y, lightDiff.z)).r;
        float depth = depthN * far;
        
        if(currentDepth - bias <= depth)
            return 1.0f;
        else
            return 0.0f;
    }
    else
    {
        float value = 0.0f;
        float startOffset = pLight.blurOffset.x;
        float offset = pLight.blurOffset.y;
        vec3 start = lightDiff + vec3(-startOffset, -startOffset, -startOffset);
        for(int x = 0; x <= blurResolution; ++x)
        {
            for(int y = 0; y <= blurResolution; ++y)
            {        
                for(int z = 0; z <= blurResolution; ++z)
                {    
                    vec3 relativeDir = start + vec3(x*offset, y*offset, z*offset);
                    float depthN = texture(pLight.lightDepthMap, vec3(relativeDir.x, -relativeDir.y, relativeDir.z)).r;
                    float depth = depthN * far;
        
                    if(currentDepth - bias <= depth)
                        value += 1.0f;
                }    
            }    
        }          

        float sampleCount = float((blurResolution + 1) * (blurResolution + 1) * (blurResolution + 1));
        value /= sampleCount;
    
        return value;
    }
}

void main()
{   
    vec4 mAmbiant = material.ambientColor * textureCs(material.ambiantTexture, uv * material.ambiantTextureST.zw + material.ambiantTextureST.xy, material.ambiantTextureColorSpace);
    vec4 ambientColor = globalAmbiantColor * mAmbiant;

    vec3 color = ambientColor.rgb;

    vec3 normalFrag = textureCs(material.normalTexture, uv * material.normalTextureST.zw + material.normalTextureST.xy, material.normalTextureColorSpace).rgb;
    normalFrag = normalize(normalFrag * 2.0 - 1.0);   
    
    vec3 V = TangentViewPos - TangentFragPos;

    PBRInfo pbrInfo = ComputePBRInfo(normalFrag, V);

    if(directionalLightCount > 0)
    {
        float shadow = ComputeDirectionalShadow(posLightSpaceArray[0], normal, directionalLight);    
        color += ComputeDirectionalLightColor(directionalLight, pbrInfo, shadow);
    }

    for(int i = 0; i < pointLightCount; i++)
    {
        float shadow = ComputePointShadow(pos, normal, pointLights[i]);
        color += ComputePointLight(pointLights[i], pbrInfo, TangentPointLightPositions[i], shadow);
    }

    for(int i = 0; i < spotLightCount; i++)
    {
        float shadow = ComputeSpotShadow(posLightSpaceArray[i + 1], normal, spotLights[i]);                 
        color += ComputeSpotLight(spotLights[i], pbrInfo, TangentSpotLightPositions[i], shadow);
    }
    
    float alpha = material.alpha * texture(material.alphaTexture, uv * material.alphaTextureST.zw + material.alphaTextureST.xy).r;

    color += material.emissiveColor.rgb * textureCs(material.emissiveTexture, uv * material.emissiveTextureST.zw + material.emissiveTextureST.xy, material.emissiveTextureColorSpace).rgb;

    color = ConvertColor(color, colorSpaceIn, colorSpaceOut);

    FdfragColor = vec4(color, alpha);
}
