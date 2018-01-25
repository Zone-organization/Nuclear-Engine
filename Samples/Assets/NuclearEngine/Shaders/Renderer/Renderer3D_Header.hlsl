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

Texture2D NE_Specular1_Tex : register(t1);
SamplerState NE_Specular1_Sampler : register(s1);