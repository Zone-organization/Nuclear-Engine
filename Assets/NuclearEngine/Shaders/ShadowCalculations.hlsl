
#ifdef NE_SHADOWS

#ifdef NE_MAX_DIR_CASTERS

Texture2DArray NE_ShadowMap_DirPos;
SamplerState NE_ShadowMap_DirPos_sampler;

float DirPosShadowCalculation(uint arrayslice, float4 fragPosLightSpace)
{
    // perform perspective divide (re-homogenize position after interpolation)
    float3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w);


    //TODO: Optimize -> A lot of branching cause performance issues
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
    float ShadowMapDepth = NE_ShadowMap_DirPos.Sample(NE_ShadowMap_DirPos_sampler, float3(projCoords.xy, arrayslice)).r;

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

Texture2DArray NE_ShadowMap_Spot;
SamplerState NE_ShadowMap_Spot_sampler;

float SpotShadowCalculation(uint arrayslice, float4 fragPosLightSpace)
{
    // perform perspective divide (re-homogenize position after interpolation)
    float3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w);


    //TODO: Optimize -> A lot of branching cause performance issues
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
    float ShadowMapDepth = NE_ShadowMap_Spot.Sample(NE_ShadowMap_Spot_sampler, float3(projCoords.xy, arrayslice)).r;

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


#ifdef NE_MAX_OMNIDIR_CASTERS

TextureCubeArray NE_ShadowMap_OmniDir;
SamplerState NE_ShadowMap_OmniDir_sampler;

float OmniDirShadowCalculation(uint arrayslice, float3 FragPos, float3 lightPos, float3 normal, float farplane)
{
    // get vector between fragment position and light position
    float3 fragToLight = FragPos - lightPos;

    // ise the fragment to light vector to sample from the depth map    
    float ShadowMapDepth = NE_ShadowMap_OmniDir.Sample(NE_ShadowMap_OmniDir_sampler, float4(fragToLight.xyz, arrayslice)).r;

    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    ShadowMapDepth *= farplane;

    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);

    float3 lightDir = normalize(lightPos.xyz - FragPos);
    float bias = max(0.5 * (1.0 - dot(normal, lightDir)), 0.05);

   // float bias = 0.05; // we use a much larger bias since depth is now in [near_plane, far_plane] range
    float shadow = currentDepth - bias > ShadowMapDepth ? 1.0 : 0.0;

    return shadow;
}
#endif

#endif  //NE_SHADOWS
