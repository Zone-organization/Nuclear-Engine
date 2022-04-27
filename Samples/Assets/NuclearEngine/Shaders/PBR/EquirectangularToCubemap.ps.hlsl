struct PixelInputType
{
    float4 Position : SV_POSITION;
};
TextureCube NE_EquirectangularMap : register(t0);;
SamplerState NE_EquirectangularMap_sampler : register(s0);

const float2 invAtan = float2(0.1591, 0.3183);

float2 SampleSphericalMap(float3 v)
{
    float2 uv = float2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

float4 main(PixelInputType input) : SV_TARGET
{
    float2 uv = SampleSphericalMap(normalize(input.Position.xyz));

    return float4(NE_EquirectangularMap.Sample(NE_EquirectangularMap_sampler, uv).rgb, 1.0);
}
