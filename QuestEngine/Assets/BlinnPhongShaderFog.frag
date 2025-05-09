#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 pos;
in vec3 viewPos;
out vec4 FdfragColor;

uniform float viewNear;
uniform float viewFar;
uniform vec3 viewDirection;

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

    sampler2D diffuseTexture;
    vec4 diffuseTextureST;
    
    sampler2D specularTexture;
    vec4 specularTextureST;

    float shininess;
}; 
uniform Material material;


vec3 ComputeDirectionalLightColor(DirectionalLight dLight)
{
    vec3 lightDirection = dLight.direction;
    vec4 ambientColor = dLight.ambientColor * material.ambientColor * texture(material.ambiantTexture, uv * material.ambiantTextureST.zw + material.ambiantTextureST.xy);
    vec4 diffuseColor = dLight.diffuseColor * material.diffuseColor * texture(material.diffuseTexture, uv * material.diffuseTextureST.zw + material.diffuseTextureST.xy);
    float NdotL = dot(normal, -lightDirection); 
    diffuseColor*= max(NdotL,0);

    vec3 viewDir = normalize(viewPos - pos);
    vec3 halfwayDir = normalize(-lightDirection + viewDir);

    vec4 specularColor = vec4(0,0,0,1);
    if(NdotL >= 0.001)
    {
        specularColor = dLight.specularColor * material.specularColor * texture(material.specularTexture, uv * material.specularTextureST.zw + material.specularTextureST.xy);
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

    vec4 ambientColor = pLight.ambientColor * material.ambientColor * texture(material.ambiantTexture, uv * material.ambiantTextureST.zw + material.ambiantTextureST.xy);
    vec4 diffuseColor = pLight.diffuseColor * material.diffuseColor * texture(material.diffuseTexture, uv * material.diffuseTextureST.zw + material.diffuseTextureST.xy);
    float NdotL = dot(normal, -lightDirection);
    diffuseColor*= max(NdotL,0);

    vec3 viewDir = normalize(viewPos - pos);
    vec3 halfwayDir = normalize(-lightDirection + viewDir);

    vec4 specularColor = vec4(0,0,0,1);
    if(NdotL >= 0.001)
    {
        specularColor = pLight.specularColor * material.specularColor * texture(material.specularTexture, uv * material.specularTextureST.zw + material.specularTextureST.xy);;
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
    vec4 ambientColor = sLight.ambientColor * material.ambientColor * texture(material.ambiantTexture, uv * material.ambiantTextureST.zw + material.ambiantTextureST.xy);

    if(cosAngle > sLight.spotCosAngle)
    {
        vec4 diffuseColor = sLight.diffuseColor * material.diffuseColor * texture(material.diffuseTexture, uv * material.diffuseTextureST.zw + material.diffuseTextureST.xy);
        vec4 specularColor = vec4(0,0,0,1);

        
        float NdotL = dot(normal, -lightDirection);
        diffuseColor*= max(NdotL,0);

        vec3 viewDir = normalize(viewPos - pos);
        vec3 halfwayDir = normalize(-lightDirection + viewDir);

        if(NdotL >= 0.001)
        {
            specularColor = sLight.specularColor * material.specularColor * texture(material.specularTexture, uv * material.specularTextureST.zw + material.specularTextureST.xy);;
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

vec3 ProjectVec3OnNormal(vec3 vector, vec3 onNormal)
{
    float sqrMag = dot(onNormal, onNormal);
    if (sqrMag < 0.0001)
        return vec3(0.0, 0.0, 0.0);
    else
    {
        float dotValue = dot(vector, onNormal);
        return vec3(onNormal.x * dotValue / sqrMag,
            onNormal.y * dotValue / sqrMag,
            onNormal.z * dotValue / sqrMag);
    }
}

void main()
{    
    vec4 ambientColor = globalAmbiantColor * material.ambientColor * texture(material.ambiantTexture, uv * material.ambiantTextureST.zw + material.ambiantTextureST.xy);
    vec3 color = vec3(ambientColor.r, ambientColor.g, ambientColor.b);

    if(directionalLightCount > 0)
        color += ComputeDirectionalLightColor(directionalLight);

    for(int i = 0; i < pointLightCount; i++)
        color += ComputePointLight(pointLights[i]);

    for(int i = 0; i < spotLightCount; i++)
        color += ComputeSpotLight(spotLights[i]);
    
    float z = gl_FragCoord.z;
    float linearZ = (-viewNear * z) / (z*(viewFar-viewNear)-viewFar);

    float fogDensity =20;

    float fog = 1.0 - exp(-linearZ * fogDensity);
    
    vec3 fogColor = vec3(128.0/255.0,128.0/255.0,128.0/255.0);

    color = mix(color,fogColor, fog*0.8f);

    FdfragColor = vec4(color.r,color.g,color.b,1.0);
};