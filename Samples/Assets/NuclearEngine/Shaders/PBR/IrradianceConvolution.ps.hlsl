struct PixelInputType
{
    float4 Position : SV_POSITION;
    float3 LocalPos : POSITION;
};

TextureCube NEMat_EnvironmentMap : register(t0);
SamplerState NEMat_EnvironmentMap_sampler : register(s0);

static const float PI = 3.14159265359;
float4 main(PixelInputType input) : SV_TARGET
{
    float3 N = normalize(input.LocalPos);
    float3 irradiance = 0.0;

    // Tangent space calculation from origin point
    float3 up = float3(0.0, 1.0, 0.0);
    float3 right = normalize(cross(up, N));
    up = normalize(cross(N, right));

    float sampleDelta = 0.025;
    float nrSamples = 0.0;
    for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            // Spherical to cartesian (in tangent space)
            float3 tangentSample = float3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            // Tangent space to world
            float3 samplefloat = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;
            irradiance += NEMat_EnvironmentMap.Sample(NEMat_EnvironmentMap_sampler, samplefloat).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));

    return float4(irradiance, 1.0);
}