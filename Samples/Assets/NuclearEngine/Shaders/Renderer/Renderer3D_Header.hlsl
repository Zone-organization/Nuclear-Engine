struct VertexInputType
{

float4 Position : POSITION0;
float3 Normal : NORMAL0;
float2 TexCoord : TEXCOORD0;
};

struct PixelInputType
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
    float3 FragPos : TEXCOORD1;
};

Texture2D NE_Tex_Diffuse1 : register(t0);
SamplerState NE_Diffuse1_Sampler : register(s0);

Texture2D NE_Tex_Specular1 : register(t1);
SamplerState NE_Specular1_Sampler : register(s1);



struct DirLight
{
    float4 Direction;
    float4 Color;
};
struct PointLight
{
    float4 Position;
    float4 Intensity_Attenuation;
    float4 Color;
};
struct SpotLight
{
    float4 Position;
    float4 Direction;
    float4 Intensity_Attenuation;
    float4 InnerCutOf_OuterCutoff;
    float4 Color;
};
#ifdef NE_LIGHT_SHADING_TECH
cbuffer NE_Light_CB
{  
    float4 ViewPos;
#ifdef NE_DIR_LIGHTS_NUM
    DirLight DirLights[NE_DIR_LIGHTS_NUM];
#endif
#ifdef NE_POINT_LIGHTS_NUM
    PointLight PointLights[NE_POINT_LIGHTS_NUM];
#endif

#ifdef NE_SPOT_LIGHTS_NUM
    SpotLight SpotLights[NE_SPOT_LIGHTS_NUM];
#endif

};
#endif