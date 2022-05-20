Texture2D NE_ShadowMap_Spot;
SamplerState NE_ShadowMap_Spot_sampler;

float SpotlightShadowCalculation(float4 fragPosLightSpace, float3 FragPos, float3 Normal, float3 lightPos)
{
    // perform perspective divide
    float3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // Transform to [0,1] range
    projCoords.x = projCoords.x / 2 + 0.5;
    projCoords.y = projCoords.y / -2 + 0.5;

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

    return shadow;
}
