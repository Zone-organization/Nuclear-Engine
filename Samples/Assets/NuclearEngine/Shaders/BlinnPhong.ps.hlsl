//Definitions:
//#define NE_USE_NORMAL_MAPS
//#define NE_DIR_LIGHTS_NUM 1
//#define NE_POINT_LIGHTS_NUM 1
//#define NE_SPOT_LIGHTS_NUM 1
//#define NE_SHADOWS_ENABLED
#include "LightTypes.hlsli"
#include <CommonInput.ps.hlsl>

#ifdef NE_SHADOWS
#include <ShadowCalculations.hlsl>
#endif


#ifdef NE_DEFFERED

Texture2D NE_RT_GBuffer_Position : register(t0);
Texture2D NE_RT_GBuffer_Normal : register(t1);
Texture2D NE_RT_GBuffer_AlbedoSpec : register(t2);

SamplerState NE_RT_GBuffer_Position_sampler : register(s0);
SamplerState NE_RT_GBuffer_Normal_sampler : register(s1);
SamplerState NE_RT_GBuffer_AlbedoSpec_sampler : register(s2);

#else

Texture2D NEMat_Diffuse1 : register(t0);
Texture2D NEMat_Specular1 : register(t1);
Texture2D NEMat_Normal1 : register(t2);

SamplerState NEMat_Diffuse1_sampler : register(s0);
SamplerState NEMat_Specular1_sampler : register(s1);
SamplerState NEMat_Normal1_sampler : register(s2);

#endif

struct PS_OUTPUT
{
    float4 Color: SV_Target0;
#ifdef BLOOM
    float4 Bloom: SV_Target1;
#endif
};

PS_OUTPUT DoLighting(PixelInputType input);

PS_OUTPUT main(PixelInputType input) : SV_TARGET
{
    return DoLighting(input);
}

//TODO: move to a global light modifier or some shit
#define Shininess 64.0f

float DoBlinnSpecular(float3 normal, float3 lightDir, float3 viewDir)
{
    float3 reflectDir = reflect(-lightDir, normal);

    return pow(max(dot(viewDir, reflectDir), 0.0f), Shininess);

    //float3 halfwayDir = normalize(lightDir + viewDir);
    //return pow(max(dot(normal, halfwayDir), 0.0f), Shininess);
}

float DoDiffuse(float3 LightDir, float3 normal)
{
    return max(dot(normal, LightDir), 0.0f);
}
float DoQuadraticAttenuation(float4 Intensity_Attenuation, float3 lightposition, float3 fragPos)
{
    float distance = length(lightposition - fragPos);
    return Intensity_Attenuation.x / (Intensity_Attenuation.y + Intensity_Attenuation.z * distance + Intensity_Attenuation.w * (distance * distance));
}
// calculates the color when using a directional light.
float3 CalcDirLight(DirLight light, float3 normal, float3 viewDir, float4 albedo)
{
    float3 lightDir = normalize(-light.Direction.xyz);

    float3 diffuse = light.Color_Intensity.xyz * DoDiffuse(lightDir, normal) * albedo.xyz;
    float3 specular = light.Color_Intensity.xyz * DoBlinnSpecular(normal, lightDir, viewDir) * albedo.w;

    diffuse *= light.Color_Intensity.w;
    specular *= light.Color_Intensity.w;

    return (diffuse + specular);
}

// calculates the color when using a point light.
float3 CalcPointLight(PointLight light, float3 normal, float3 fragPos, float3 viewDir, float4 albedo)
{
    float3 lightDir = normalize(light.Position.xyz - fragPos);

    // attenuation
    float attenuation = DoQuadraticAttenuation(light.Intensity_Attenuation, light.Position.xyz, fragPos);

    // combine results
    float3 diffuse = light.Color.xyz * DoDiffuse(lightDir, normal) * albedo.xyz;
    float3 specular = light.Color.xyz * DoBlinnSpecular(normal, lightDir, viewDir) * albedo.w;
    diffuse *= attenuation;
    specular *= attenuation;

	return (diffuse + specular);// *ModelColor;
}

// calculates the color when using a spot light.
float3 CalcSpotLight(SpotLight light, float3 normal, float3 fragPos, float3 viewDir, float4 albedo)
{
    float3 lightDir = normalize(light.Position.xyz - fragPos);

    // attenuation
    float attenuation = DoQuadraticAttenuation(light.Intensity_Attenuation, light.Position.xyz, fragPos);

    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.Direction.xyz));
    float epsilon = light.InnerCutOf_OuterCutoff.x - light.InnerCutOf_OuterCutoff.y;
    float intensity = clamp((theta - light.InnerCutOf_OuterCutoff.y) / epsilon, 0.0, 1.0);

    // combine results
    float3 diffuse = light.Color.xyz * DoDiffuse(lightDir, normal) * albedo.xyz;
    float3 specular = light.Color.xyz * DoBlinnSpecular(normal, lightDir, viewDir) * albedo.w;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (diffuse + specular);
}

PS_OUTPUT DoLighting(PixelInputType input)
{
    float3 result = float3(0.0f, 0.0f, 0.0f);
#ifdef NE_DEFFERED
    float3 FragPos = NE_RT_GBuffer_Position.Sample(NE_RT_GBuffer_Position_sampler, input.TexCoord).xyz;
    float3 norm = NE_RT_GBuffer_Normal.Sample(NE_RT_GBuffer_Normal_sampler, input.TexCoord).xyz;
    float4 albedo = NE_RT_GBuffer_AlbedoSpec.Sample(NE_RT_GBuffer_AlbedoSpec_sampler, input.TexCoord);
#else
    float3 FragPos = input.FragPos;

    float3 norm = NEMat_Normal1.Sample(NEMat_Normal1_sampler, input.TexCoord).xyz;
    norm = normalize(mul(norm, 2.0f) - 1.0f);
    norm = normalize(mul(norm, input.TBN));
    float4 albedo = float4(NEMat_Diffuse1.Sample(NEMat_Diffuse1_sampler, input.TexCoord).xyz, NEMat_Specular1.Sample(NEMat_Specular1_sampler, input.TexCoord).x);
#endif

    float3 viewDir = normalize(ViewPos.xyz - FragPos);


    float dir_Shadow = 1.0f, spot_shadow = 1.0f;  //No shadow == 1.0f

#ifdef NE_SHADOWS


#ifdef NE_MAX_DIR_CASTERS
    dir_Shadow = (1.0f - DirlightShadowCalculation(input.DirLight_FragPos[0], FragPos, norm));
#endif

#ifdef NE_MAX_SPOT_CASTERS
    spot_shadow = (1.0f - SpotlightShadowCalculation(input.SpotLight_FragPos[0], FragPos, norm));
#endif

#endif

    // phase 0: ambient "diffuse" lighting
    //should be done once and not incremented...?
    float3 ambient = 0.05f * albedo.xyz;
    result += ambient;

#ifdef NE_DIR_LIGHTS_NUM
  // phase 1: directional lighting
    for (int i0 = 0; i0 < NE_DIR_LIGHTS_NUM; i0++)
    {
        result += dir_Shadow * CalcDirLight(DirLights[i0], norm, viewDir, albedo);
    }
#endif
#ifdef NE_POINT_LIGHTS_NUM  
    // phase 2: point lights
    for (int i1 = 0; i1 < NE_POINT_LIGHTS_NUM; i1++)
    {
        result += CalcPointLight(PointLights[i1], norm, FragPos, viewDir, albedo);
    }
#endif
#ifdef NE_SPOT_LIGHTS_NUM
    // phase 3: spot light
    for (int i2 = 0; i2 < NE_SPOT_LIGHTS_NUM; i2++)
    {
        result += spot_shadow * CalcSpotLight(SpotLights[i2], norm, FragPos, viewDir, albedo);
    }
#endif

    PS_OUTPUT output;
    output.Color =  float4(result, 1.0f);

#ifdef BLOOM
    float brightness = dot(result, float3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0)
        output.Bloom = float4(result, 1.0);
    else
        output.Bloom = float4(0.0, 0.0, 0.0, 1.0);
#endif
   return output;
}
