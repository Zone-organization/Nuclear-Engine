struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 TexCoords : TEXCOORD0;
    float3 Normal : NORMAL0;
    float3 FragPos : TEXCOORD1;
    float3x3 TBN : TANGENT0;
};

Texture2D NEMat_Diffuse1 : register(t0);
Texture2D NEMat_Specular1 : register(t1);
Texture2D NEMat_Normal1 : register(t2);

SamplerState NEMat_Diffuse1_sampler : register(s0);
SamplerState NEMat_Specular1_sampler : register(s1);
SamplerState NEMat_Normal1_sampler : register(s2);

struct PS_OUTPUT
{
    float4 Position: SV_Target0;
    float4 Normal : SV_Target1;
    float4 AlbedoSpec: SV_Target2;
};


PS_OUTPUT main(PixelInputType input)
{
    PS_OUTPUT output;

    float3 norm = normalize(NEMat_Normal1.Sample(NEMat_Normal1_sampler, input.TexCoords).xyz);
    norm = normalize(mul(norm, 2.0f) - 1.0f);
    norm = normalize(mul(norm, input.TBN));

    // store the fragment position vector in the first gbuffer texture
    output.Position = float4(input.FragPos, 1.0f);
    // also store the per-fragment normals into the gbuffer
    output.Normal = float4(norm, 1.0f);
    // and the diffuse per-fragment color
    output.AlbedoSpec.rgb = NEMat_Diffuse1.Sample(NEMat_Diffuse1_sampler, input.TexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    output.AlbedoSpec.a = NEMat_Specular1.Sample(NEMat_Specular1_sampler, input.TexCoords).r;

    return output;
}