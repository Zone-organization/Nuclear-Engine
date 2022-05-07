struct PixelInputType
{
    float4 Position : SV_POSITION;
};
Texture2D NEMat_EquirectangularMap : register(t0);;
SamplerState NEMat_EquirectangularMap_sampler : register(s0);

#define PI 3.1415926535897932384626433832795

float2 SampleSphericalMap(float3 v)
{
    float2 v_uv = float2(0.0, 0.0);
    v_uv.x = 0.5 * atan2(v.x, v.z) / PI + 0.5;
    v_uv.y = asin(v.y) / PI + 0.5;

    return v_uv;
}

float4 main(PixelInputType input) : SV_TARGET
{
    float2 uv = SampleSphericalMap(normalize(input.Position.xyz));

    return float4(NEMat_EquirectangularMap.Sample(NEMat_EquirectangularMap_sampler, uv).rgb, 1.0f);
}
