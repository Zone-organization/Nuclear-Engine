
#ifdef NE_SHADOWS

#ifdef NE_MAX_DIR_CASTERS
Texture2D NE_ShadowMap_Dir;
SamplerState NE_ShadowMap_Dir_sampler;

float DirlightShadowCalculation(float4 fragPosLightSpace, float3 FragPos, float3 Normal)
{
    // perform perspective divide (re-homogenize position after interpolation)
    float3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w);

    //if position is not visible to the light - dont illuminate it
    //results in hard light frustum
    if (projCoords.x < -1.0f || projCoords.x > 1.0f ||
        projCoords.y < -1.0f || projCoords.y > 1.0f ||
        projCoords.z < 0.0f || projCoords.z > 1.0f)
    {
        return 0.0f;
    }

    //Clipspace [-1,1] to uv space [0,1]
    projCoords.x = projCoords.x / 2 + 0.5;
    projCoords.y = projCoords.y / -2 + 0.5;

    //apply shadow map bias
    //projCoords.z -= shadowMapBias;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float ShadowMapDepth = NE_ShadowMap_Dir.Sample(NE_ShadowMap_Dir_sampler, projCoords.xy).r;

    // get depth of current fragment from light's perspective
    float CurrentDepth = projCoords.z;

    float shadow = 0.0f;

  //  if clip space z value greater than shadow map value then pixel is in shadow ( check whether current frag pos is in shadow )
   // float shadow = projCoords.z <= ShadowMapDepth;

    if (ShadowMapDepth < CurrentDepth)
    {
        shadow = 1.0f;
    }

    return shadow;
}

#endif

#ifdef NE_MAX_SPOT_CASTERS

Texture2D NE_ShadowMap_Spot;
SamplerState NE_ShadowMap_Spot_sampler;

float SpotlightShadowCalculation(float4 fragPosLightSpace, float3 FragPos, float3 Normal)
{
    //projCoords = projCoords * 0.5 + 0.5;
    //float shadow = 0.0f;
    //// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    //float4 closestDepth = NE_ShadowMap_Spot.Sample(NE_ShadowMap_Spot_sampler, projCoords.xy).r;
    //// get depth of current fragment from light's perspective
    //float currentDepth = projCoords.z;
    //// calculate bias (based on depth map resolution and slope)
    //float3 normal = normalize(Normal);
    //float3 lightDir = normalize(lightPos - FragPos);
    //float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    //// check whether current frag pos is in shadow
    //float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    //// keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    //if (projCoords.z > 1.0)
    //    shadow = 0.0;

    // perform perspective divide (re-homogenize position after interpolation)
    float3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w);

    //if position is not visible to the light - dont illuminate it
    //results in hard light frustum
    //if (projCoords.x < -1.0f || projCoords.x > 1.0f ||
    //    projCoords.y < -1.0f || projCoords.y > 1.0f ||
    //    projCoords.z < 0.0f || projCoords.z > 1.0f)
    //{
    //    return 0.0f;
    //}

    //Clipspace [-1,1] to uv space [0,1]
    projCoords.x = projCoords.x / 2 + 0.5;
    projCoords.y = projCoords.y / -2 + 0.5;

    //apply shadow map bias
    //projCoords.z -= shadowMapBias;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float ShadowMapDepth = NE_ShadowMap_Spot.Sample(NE_ShadowMap_Spot_sampler, projCoords.xy).r;

    // get depth of current fragment from light's perspective
    float CurrentDepth = projCoords.z;

    float shadow = 0.0f;

    //  if clip space z value greater than shadow map value then pixel is in shadow ( check whether current frag pos is in shadow )
     // float shadow = projCoords.z <= ShadowMapDepth;

    if (ShadowMapDepth < CurrentDepth)
    {
        shadow = 1.0f;
    }

    return shadow;
}
#endif
 
#endif  //NE_SHADOWS
