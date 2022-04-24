struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 TexCoords : TEXCOORD0;
    float3 Normal : NORMAL0;
    float3 FragPos : TEXCOORD1;
    float3x3 TBN : TANGENT0;
};

Texture2D NEMat_Albedo : register(t0);
Texture2D NEMat_Metallic : register(t1);
Texture2D NEMat_Normal : register(t2);
#ifdef PBR
Texture2D NEMat_Roughness : register(t3);
Texture2D NEMat_AO : register(t4);
#endif

SamplerState NEMat_Albedo_sampler : register(s0);
SamplerState NEMat_Metallic_sampler : register(s1);
SamplerState NEMat_Normal_sampler : register(s2);
#ifdef PBR
SamplerState NEMat_Roughness_sampler : register(s3);
SamplerState NEMat_AO_sampler : register(s4);
#endif

struct PS_OUTPUT
{
    float4 Position: SV_Target0;
    float4 Normal : SV_Target1;
    float4 AlbedoMatallic : SV_Target2;
#ifdef PBR
    float4 RoughnessAO : SV_Target3;
#endif

};


PS_OUTPUT main(PixelInputType input)
{
    PS_OUTPUT output;

    float3 norm = NEMat_Normal.Sample(NEMat_Normal_sampler, input.TexCoords).xyz;
    norm = normalize(mul(2.0f ,norm) - 1.0f);
    norm = normalize(mul(norm, input.TBN));

    // store the fragment position vector in the first gbuffer texture
    output.Position = float4(input.FragPos, 1.0f);
    // also store the per-fragment normals into the gbuffer
    output.Normal = float4(normalize(norm), 1.0f);

#ifdef PBR
    //output.NormalRoughness = float4(norm, NEMat_Roughness.Sample(NEMat_Roughness_sampler, input.TexCoords).r);
    output.RoughnessAO.r = NEMat_Roughness.Sample(NEMat_Roughness_sampler, input.TexCoords).r;
    output.RoughnessAO.g = NEMat_AO.Sample(NEMat_AO_sampler, input.TexCoords).r;
#endif

    // and the diffuse per-fragment color
    output.AlbedoMatallic.rgb = NEMat_Albedo.Sample(NEMat_Albedo_sampler, input.TexCoords).rgb;
    // store specular intensity in AlbedoMatallic's alpha component
    output.AlbedoMatallic.a = NEMat_Metallic.Sample(NEMat_Metallic_sampler, input.TexCoords).r;


    return output;
}