struct PixelInputType
{
    float4 Position : SV_POSITION;
    float3 LocalPos : POSITION;
};
Texture2D NEMat_EquirectangularMap : register(t0);;
SamplerState NEMat_EquirectangularMap_sampler : register(s0);

static const float2 invAtan = float2(0.1591, 0.3183);
float2 SampleSphericalMap(float3 v)
{
    float2 uv = float2(atan2(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

//float3(1.0f, 0.0f, 0.0f);
float4 main(PixelInputType input) : SV_TARGET
{
    float2 uv = SampleSphericalMap(normalize(input.LocalPos));
    float3 color = NEMat_EquirectangularMap.Sample(NEMat_EquirectangularMap_sampler, uv).rgb;
    return float4(color, 1.0f);
}
