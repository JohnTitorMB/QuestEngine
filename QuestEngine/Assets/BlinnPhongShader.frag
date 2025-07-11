#version 330 core
#include "ColorManagement.glsl"
in vec2 uv;
in vec3 normal;
in vec3 pos;
in vec3 viewPos;
 
out vec4 FdfragColor;

uniform vec2 textureTilling = vec2(1,1);
uniform vec2 textureOffset = vec2(0,0);
uniform vec4 globalAmbiantColor;
struct DirectionalLight 
{
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;
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
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;

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
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;

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
    vec4 diffuseColor;
    vec4 specularColor;
    vec4 emissiveColor;

    sampler2D ambiantTexture;
    vec4 ambiantTextureST;
    int ambiantTextureColorSpace;

    sampler2D diffuseTexture;
    vec4 diffuseTextureST;
    int diffuseTextureColorSpace;

    sampler2D specularTexture;
    vec4 specularTextureST;
    int specularTextureColorSpace;

    sampler2D emissiveTexture;
    vec4 emissiveTextureST;
    int emissiveTextureColorSpace;

    sampler2D alphaTexture;
    vec4 alphaTextureST;

    float shininess;
    float alpha;
}; 
uniform Material material;



vec3 ComputeDirectionalLightColor(DirectionalLight dLight, float shadow)
{
    vec3 lightDirection = dLight.direction;
    vec4 ambientColor = dLight.ambientColor * material.ambientColor * textureCs(material.ambiantTexture, uv * material.ambiantTextureST.zw + material.ambiantTextureST.xy, material.ambiantTextureColorSpace);
    vec4 diffuseColor = dLight.diffuseColor * material.diffuseColor * textureCs(material.diffuseTexture, uv * material.diffuseTextureST.zw + material.diffuseTextureST.xy, material.diffuseTextureColorSpace);
    float NdotL = dot(normal, -lightDirection); 
    diffuseColor*= max(NdotL,0);

    vec3 viewDir = normalize(viewPos - pos);
    vec3 halfwayDir = normalize(-lightDirection + viewDir);

    vec4 specularColor = vec4(0,0,0,1);
    if(NdotL >= 0.001)
    {
        specularColor = dLight.specularColor * material.specularColor * textureCs(material.specularTexture, uv * material.specularTextureST.zw + material.specularTextureST.xy, material.specularTextureColorSpace);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
        spec = smoothstep(0.0, 1.0, spec);
        specularColor *= spec;
    }
  
    vec4 color = ambientColor+(diffuseColor + specularColor) * shadow;

    return vec3(color.r, color.g, color.b) * dLight.intensity;
}

vec3 ComputePointLight(PointLight pLight, float shadow)
{
    vec3 lightDiff = pos - pLight.position;


    float lightDistance = length(lightDiff);
    vec3 lightDirection = lightDiff/lightDistance;

    vec4 ambientColor = pLight.ambientColor * material.ambientColor * textureCs(material.ambiantTexture, uv * material.ambiantTextureST.zw + material.ambiantTextureST.xy, material.ambiantTextureColorSpace);
    vec4 diffuseColor = pLight.diffuseColor * material.diffuseColor * textureCs(material.diffuseTexture, uv * material.diffuseTextureST.zw + material.diffuseTextureST.xy, material.diffuseTextureColorSpace);
    float NdotL = dot(normal, -lightDirection);
    diffuseColor*= max(NdotL,0);

    vec3 viewDir = normalize(viewPos - pos);
    vec3 halfwayDir = normalize(-lightDirection + viewDir);

    vec4 specularColor = vec4(0,0,0,1);
    if(NdotL >= 0.001)
    {
        specularColor = pLight.specularColor * material.specularColor * textureCs(material.specularTexture, uv * material.specularTextureST.zw + material.specularTextureST.xy, material.specularTextureColorSpace);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
        spec = smoothstep(0.0, 1.0, spec);
        specularColor *= spec;
    }
  
    float attenuation = 1.0 / (pLight.constant + pLight.linear * lightDistance +  pLight.quadratic * (lightDistance * lightDistance));    

    vec4 color = ambientColor + (diffuseColor + specularColor) * attenuation * shadow;
    
    return vec3(color.r, color.g, color.b)* pLight.intensity;
}

vec3 ComputeSpotLight(SpotLight sLight, float shadow)
{
    vec3 lightDiff = pos - sLight.position;

    float lightDistance = length(lightDiff);
    vec3 lightDirection = lightDiff/lightDistance;

    float cosAngle    = dot(lightDirection, normalize(sLight.direction));
    vec4 ambientColor = sLight.ambientColor * material.ambientColor * textureCs(material.ambiantTexture, uv * material.ambiantTextureST.zw + material.ambiantTextureST.xy, material.ambiantTextureColorSpace);

    if(cosAngle > sLight.spotCosAngle)
    {
        vec4 diffuseColor = sLight.diffuseColor * material.diffuseColor * textureCs(material.diffuseTexture, uv * material.diffuseTextureST.zw + material.diffuseTextureST.xy, material.diffuseTextureColorSpace);
       
        float NdotL = dot(normal, -lightDirection);
        diffuseColor*= max(NdotL,0);

        vec3 viewDir = normalize(viewPos - pos);
        vec3 halfwayDir = normalize(-lightDirection + viewDir);

        vec4 specularColor = vec4(0,0,0,1);
        if(NdotL >= 0.001)
        {
            specularColor = sLight.specularColor * material.specularColor * textureCs(material.specularTexture, uv * material.specularTextureST.zw + material.specularTextureST.xy, material.specularTextureColorSpace);
            float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
            spec = smoothstep(0.0, 1.0, spec);
            specularColor *= spec;
        }
  
        float attenuation = 1.0 / (sLight.constant + sLight.linear * lightDistance +  sLight.quadratic * (lightDistance * lightDistance));    
        float epsilon   = sLight.spotCosSmoothAngle - sLight.spotCosAngle;
        float intensity = smoothstep(0.0, 1.0, (cosAngle - sLight.spotCosAngle) / epsilon); 
        vec4 color = ambientColor+(diffuseColor + specularColor) * attenuation * intensity * shadow;

        return vec3(color.r, color.g, color.b)* sLight.intensity;
    }
    else
        return ambientColor.rgb;
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

float ComputeSpotShadow(vec4 posLightSpace, SpotLight sLight)
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
    float bias = max(maxBias * (1.0 - dot(normal, -lightDir)), minBias);  
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

float ComputePointShadow(vec3 worldPos, PointLight pLight)
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
    float bias = max(maxBias * (1.0 - dot(normal, -lightDir)), minBias);  
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

    vec3 color = vec3(ambientColor.r, ambientColor.g, ambientColor.b);

    if(directionalLightCount > 0)
    {
        float shadow = ComputeDirectionalShadow(posLightSpaceArray[0], directionalLight);    
        color += ComputeDirectionalLightColor(directionalLight, shadow);
    }

    for(int i = 0; i < pointLightCount; i++)
    {
        float shadow = ComputePointShadow(pos, pointLights[i]);
        color += ComputePointLight(pointLights[i], shadow);
    }

    for(int i = 0; i < spotLightCount; i++)
    {
        float shadow = ComputeSpotShadow(posLightSpaceArray[i+1], spotLights[i]);                 
        color += ComputeSpotLight(spotLights[i], shadow);
    }
    
    float alpha = material.alpha * texture(material.alphaTexture, uv * material.alphaTextureST.zw + material.alphaTextureST.xy).r;

    color.rgb += material.emissiveColor.rgb * textureCs(material.emissiveTexture, uv * material.emissiveTextureST.zw + material.emissiveTextureST.xy, material.emissiveTextureColorSpace).rgb;

    color.rgb = ConvertColor(color.rgb, colorSpaceIn, colorSpaceOut);

    FdfragColor = vec4(color.r,color.g,color.b,alpha);

 //   float shadow = ComputeSpotShadow(posLightSpaceArray[1], spotLights[0]);               
 //   FdfragColor = vec4(shadow,shadow,shadow,1);
};
