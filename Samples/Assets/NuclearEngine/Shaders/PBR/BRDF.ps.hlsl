#include "Common.hlsl"

struct FullScreenTriangleVSOutput
{
    float4 f4PixelPos     : SV_Position;
    float2 f2NormalizedXY : NORMALIZED_XY; // Normalized device XY coordinates [-1,1]x[-1,1]
};

float2 IntegrateBRDF(float NdotV, float roughness)
{
    float3 V;
    V.x = sqrt(1.0 - NdotV * NdotV);
    V.y = 0.0;
    V.z = NdotV;

    float A = 0.0;
    float B = 0.0;

    float3 N = float3(0.0, 0.0, 1.0);

    const uint SAMPLE_COUNT = 1024u;
    for (uint i = 0u; i < SAMPLE_COUNT; i++)
    {
        // generates a sample vector that's biased towards the
        // preferred alignment direction (importance sampling).
        float2 Xi = Hammersley(i, SAMPLE_COUNT);
        float3 H = ImportanceSampleGGX(Xi, roughness, N);
       // float3 L = normalize(2.0 * dot(V, H) * H - V);
        float3 L = normalize(reflect(-V, H));

        float NdotL = saturate(L.z);
        float NdotH = saturate(H.z);
        float VdotH = saturate(dot(V, H));

        if (NdotL > 0.0f)
        {
            float G = GeometrySmith(N, V, L, roughness);
            float G_Vis = max((G * VdotH) / (NdotH * NdotV), 0.0001);
            float Fc = pow(1.0 - VdotH, 5.0f);

            A += (1.0f- Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }

    return float2(A, B) / float(SAMPLE_COUNT);
}
// ----------------------------------------------------------------------------
float2 main(FullScreenTriangleVSOutput input) : SV_TARGET
{ 
    float2 UV = NormalizedDeviceXYToTexUV(input.f2NormalizedXY);
    float NdotV = UV.x;
    float linearRoughness = UV.y;

    return IntegrateBRDF(NdotV, linearRoughness);
}