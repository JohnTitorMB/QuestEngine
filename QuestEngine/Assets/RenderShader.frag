#version 330 core
in vec2 uv;
out vec4 fragColor;

struct Material 
{
    vec4 color;

    sampler2D texture;
    vec4 textureST;
}; 
uniform Material material;

void main()
{    
/*
    //DepthHere
    float depthStencil = texture(material.texture, uv * material.textureST.zw + material.textureST.xy).r;
    
    uint depthStencilUInt = uint(depthStencil * 4294967295.0); //2 puissance 32 -1
    uint depthUInt = depthStencilUInt >> 8;

    float depth = float(depthUInt) / 16777215.0; 

    
    float viewNear = 0.01f;
    float viewFar = 1000;

    float linearDepth = (-viewNear * depth)/(depth*(viewFar - viewNear)- viewFar);


    float partialDistance = 10.0f;
    float dist = partialDistance / (viewFar-viewNear);

    float partialDepth = clamp(linearDepth/dist, 0.0f, 1.0f);
    
    fragColor =  material.color * vec4(partialDepth,partialDepth,partialDepth,1);
   */
    //Color here
    vec4 color = material.color * texture(material.texture, uv * material.textureST.zw + material.textureST.xy);
    fragColor = color;
};