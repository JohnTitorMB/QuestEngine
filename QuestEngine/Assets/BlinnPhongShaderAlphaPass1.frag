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
};
#define SPOT_LIGHT_COUNT 4  
uniform SpotLight spotLights[SPOT_LIGHT_COUNT];
uniform int spotLightCount = 0;

struct Material 
{
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;
  
    sampler2D ambiantTexture;
    vec4 ambiantTextureST;
    int ambiantTextureColorSpace;

    sampler2D diffuseTexture;
    vec4 diffuseTextureST;
    int diffuseTextureColorSpace;

    sampler2D specularTexture;
    vec4 specularTextureST;
    int specularTextureColorSpace;

    sampler2D alphaTexture;
    vec4 alphaTextureST;

    float shininess;
    float alpha;

  
}; 
uniform Material material;


vec3 ComputeDirectionalLightColor(DirectionalLight dLight)
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
        specularColor*= pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    }
  
    vec4 color = ambientColor+diffuseColor + specularColor;

    return vec3(color.r, color.g, color.b) * dLight.intensity;
}


vec3 ComputePointLight(PointLight pLight)
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
        specularColor*= pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    }
  
    float attenuation = 1.0 / (pLight.constant + pLight.linear * lightDistance +  pLight.quadratic * (lightDistance * lightDistance));    

    vec4 color = ambientColor+diffuseColor + specularColor;
    color*=attenuation;
  
    return vec3(color.r, color.g, color.b)* pLight.intensity;
}

vec3 ComputeSpotLight(SpotLight sLight)
{
    vec3 lightDiff = pos - sLight.position;

    float lightDistance = length(lightDiff);
    vec3 lightDirection = lightDiff/lightDistance;

    float cosAngle    = dot(lightDirection, normalize(sLight.direction));
    vec4 ambientColor = sLight.ambientColor * material.ambientColor * textureCs(material.ambiantTexture, uv * material.ambiantTextureST.zw + material.ambiantTextureST.xy, material.ambiantTextureColorSpace);

    if(cosAngle > sLight.spotCosAngle)
    {
        vec4 diffuseColor = sLight.diffuseColor * material.diffuseColor * textureCs(material.diffuseTexture, uv * material.diffuseTextureST.zw + material.diffuseTextureST.xy, material.diffuseTextureColorSpace);
        vec4 specularColor = vec4(0,0,0,1);

        
        float NdotL = dot(normal, -lightDirection);
        diffuseColor*= max(NdotL,0);

        vec3 viewDir = normalize(viewPos - pos);
        vec3 halfwayDir = normalize(-lightDirection + viewDir);

        if(NdotL >= 0.001)
        {
            specularColor = sLight.specularColor * material.specularColor * textureCs(material.specularTexture, uv * material.specularTextureST.zw + material.specularTextureST.xy, material.specularTextureColorSpace);
            specularColor*= pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
        }
  
        float attenuation = 1.0 / (sLight.constant + sLight.linear * lightDistance +  sLight.quadratic * (lightDistance * lightDistance));    
        float epsilon   = sLight.spotCosSmoothAngle - sLight.spotCosAngle;
        float intensity = smoothstep(0.0, 1.0, (cosAngle - sLight.spotCosAngle) / epsilon); 
        vec4 color = ambientColor+diffuseColor + specularColor;
        color*=attenuation * intensity;

        return vec3(color.r, color.g, color.b)* sLight.intensity;
    }
    else
        return vec3(0, 0, 0);
}

void main()
{   
    vec4 mAmbiant = material.ambientColor * textureCs(material.ambiantTexture, uv * material.ambiantTextureST.zw + material.ambiantTextureST.xy, material.ambiantTextureColorSpace);
    vec4 ambientColor = globalAmbiantColor * mAmbiant;
    vec3 color = vec3(ambientColor.r, ambientColor.g, ambientColor.b);

    if(directionalLightCount > 0)
        color += ComputeDirectionalLightColor(directionalLight);

    for(int i = 0; i < pointLightCount; i++)
        color += ComputePointLight(pointLights[i]);

    for(int i = 0; i < spotLightCount; i++)
        color += ComputeSpotLight(spotLights[i]);
    
    float alpha = material.alpha * texture(material.alphaTexture, uv * material.alphaTextureST.zw + material.alphaTextureST.xy).r;
    if(alpha < 0.99f)
        discard;

    FdfragColor = vec4(color.r,color.g,color.b,alpha);
};
