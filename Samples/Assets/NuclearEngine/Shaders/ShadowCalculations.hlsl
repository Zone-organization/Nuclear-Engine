
Texture2D NE_ShadowMap_Dir;
SamplerState NE_ShadowMap_Dir_sampler;

Texture2D NE_ShadowMap_Spot;
SamplerState NE_ShadowMap_Spot_sampler;

float DirlightShadowCalculation(float4 fragPosLightSpace, float3 FragPos, float3 Normal, float3 direction)
{
    // perform perspective divide
   // float3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    float3 projCoords;
    float bias = 0.001f;
    //Remap the -1 to 1 NDC to the range of 0 to 1
  //  projCoords.xy = projCoords.xy * 0.5f + 0.5f;
    projCoords.x = fragPosLightSpace.x / fragPosLightSpace.w / 2.0f + 0.5f;
    projCoords.y = -fragPosLightSpace.y / fragPosLightSpace.w / 2.0f + 0.5f;
    projCoords.z = fragPosLightSpace.z;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float4 closestDepth = NE_ShadowMap_Dir.Sample(NE_ShadowMap_Dir_sampler, projCoords.xy).r;
    // get depth of current fragment from light's perspective
   // float currentDepth = projCoords.z;

    float currentDepth = NormalizedDeviceZToDepth(projCoords.z);

    // calculate bias (based on depth map resolution and slope)
    float3 normal = normalize(Normal);
    float3 lightDir = normalize(direction);


    //Fixes the shadow acne
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // check whether current frag pos is in shadow
   // float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    // PCF
    float shadow = 0.0;
    uint width, height, levels;
    NE_ShadowMap_Dir.GetDimensions(0, width, height, levels);
    float2 texelSize = 1.0 / float2(width, height);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = NE_ShadowMap_Dir.Sample(NE_ShadowMap_Dir_sampler, projCoords.xy + float2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}


float SpotlightShadowCalculation(float4 fragPosLightSpace, float3 FragPos, float3 Normal, float3 lightPos)
{
    // perform perspective divide
    float3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // Transform to [0,1] range
    //projCoords.x = projCoords.x / 2 + 0.5;
    //projCoords.y = projCoords.y / -2 + 0.5;
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float4 closestDepth = NE_ShadowMap_Spot.Sample(NE_ShadowMap_Spot_sampler, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    float3 normal = normalize(Normal);
    float3 lightDir = normalize(lightPos - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}
