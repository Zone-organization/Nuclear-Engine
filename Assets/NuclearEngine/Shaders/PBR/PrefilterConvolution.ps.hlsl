#include "PBR/Common.hlsl"

struct PixelInputType
{
    float4 Position : SV_POSITION;
    float3 LocalPos : POSITION;
};

TextureCube NEMat_EnvironmentMap : register(t0);
SamplerState NEMat_EnvironmentMap_sampler : register(s0);

cbuffer NEStatic_Roughness : register(b0)
{
    float4 RoughnessF4;
}

float4 main(PixelInputType input) : SV_TARGET
{
    float3 N = normalize(input.LocalPos);
    float3 R = N;
    float3 V = R;

    const uint SAMPLE_COUNT = 1024u;
    float3 prefilteredColor = float3(0.0f, 0.0f, 0.0f);
    float totalWeight = 0.0f;

    for (uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        float2 Xi = Hammersley(i, SAMPLE_COUNT);
        float3 H = ImportanceSampleGGX(Xi, RoughnessF4.x, N);
        float3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if (NdotL > 0.0f)
        {
            // sample from the environment's mip level based on roughness/pdf
            float D = DistributionGGX(N, H, RoughnessF4.x);
            float NdotH = max(dot(N, H), 0.0f);
            float HdotV = max(dot(H, V), 0.0f);
            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001f;

            float resolution = 512.0; // resolution of source cubemap (per face)
            float saTexel = 4.0 * PI / (6.0 * resolution * resolution);
            float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

            float mipLevel = RoughnessF4.x == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

            prefilteredColor += NEMat_EnvironmentMap.SampleLevel(NEMat_EnvironmentMap_sampler, L, mipLevel).rgb * NdotL;
            totalWeight += NdotL;
        }
    }


    prefilteredColor = prefilteredColor / totalWeight;

    return float4(prefilteredColor, 1.0);
}
