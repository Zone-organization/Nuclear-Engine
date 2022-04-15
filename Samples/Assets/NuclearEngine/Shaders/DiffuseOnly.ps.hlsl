
struct PixelInputType
{
    float4 Position : SV_POSITION;

    float2 UV : TEXCOORD0;

    float3 Normal : NORMAL0;

};

Texture2D NEMat_Diffuse1 : register(t0);;
SamplerState NEMat_Diffuse1_sampler : register(s0);

Texture2D ShadowMap : register(t0);;
SamplerState ShadowMap_sampler : register(s0);


float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    float3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    float3 normal = normalize(fs_in.Normal);
    float3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
     float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    // PCF
    //float shadow = 0.0;
    //float2 texelSize = 1.0 / textureSize(shadowMap, 0);
    //for (int x = -1; x <= 1; ++x)
    //{
    //    for (int y = -1; y <= 1; ++y)
    //    {
    //        float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
    //        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
    //    }
    //}
    //shadow /= 9.0;

    //// keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    //if (projCoords.z > 1.0)
    //    shadow = 0.0;

    return shadow;
}

float4 main(PixelInputType input) : SV_TARGET
{
    float4 result = float4(1.0f, 1.0f, 1.0f, 1.0f);
    result = NEMat_Diffuse1.Sample(NEMat_Diffuse1_sampler, input.UV);


    return result;
}