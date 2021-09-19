//Definitions:
//#define NE_USE_NORMAL_MAPS
//#define NE_DIR_LIGHTS_NUM 1
//#define NE_POINT_LIGHTS_NUM 1
//#define NE_SPOT_LIGHTS_NUM 1
//#define NE_SHADOWS_ENABLED
#include "LightTypes.hlsli"

struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 TexCoords : TEXCOORD0;
    float3 Normal : NORMAL0;
    float3 FragPos : TEXCOORD1;

#ifdef NE_USE_NORMAL_MAPS
    float3x3 TBN : TANGENT0;
#endif
};

Texture2D NEMat_Diffuse1 : register(t0);
Texture2D NEMat_Specular1 : register(t1);
#ifdef NE_USE_NORMAL_MAPS
Texture2D NEMat_Normal1 : register(t2);
#endif

SamplerState NEMat_Diffuse1_sampler : register(s0);
SamplerState NEMat_Specular1_sampler : register(s1);
#ifdef NE_USE_NORMAL_MAPS
SamplerState NEMat_Normal1_sampler : register(s2);
#endif

//cbuffer NEDynamic_Material
//{
//	float3 ModelColor;
//	float Shininess;
//};

float4 DoLighting(PixelInputType input);

float4 main(PixelInputType input) : SV_TARGET
{
    return DoLighting(input);
}

//TODO: move to a global light modifier or some shit
#define Shininess 64.0f

float DoBlinnSpecular(float3 normal, float3 lightDir, float3 viewDir)
{
    float3 halfwayDir = normalize(lightDir + viewDir);
    return pow(max(dot(normal, halfwayDir), 0.0f), Shininess);
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
float3 CalcDirLight(DirLight light, float3 normal, float3 viewDir, float2 TexCoords)
{
    float3 lightDir = normalize(-light.Direction.xyz);

    float3 ambient = 0.05f * float3(light.Color.xyz * NEMat_Diffuse1.Sample(NEMat_Diffuse1_sampler, TexCoords).xyz);
    float3 diffuse = light.Color.xyz * DoDiffuse(lightDir, normal) * NEMat_Diffuse1.Sample(NEMat_Diffuse1_sampler, TexCoords).xyz;
    float3 specular = light.Color.xyz * DoBlinnSpecular(normal, lightDir, viewDir) * NEMat_Specular1.Sample(NEMat_Specular1_sampler, TexCoords).xyz;
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
float3 CalcPointLight(PointLight light, float3 normal, float3 fragPos, float3 viewDir, float2 TexCoords)
{
    float3 lightDir = normalize(light.Position.xyz - fragPos);
    // attenuation
    float attenuation = DoQuadraticAttenuation(light.Intensity_Attenuation, light.Position.xyz, fragPos);
    // combine results
    float3 ambient = 0.05f * float3(light.Color.xyz * NEMat_Diffuse1.Sample(NEMat_Diffuse1_sampler, TexCoords).xyz);
    float3 diffuse = light.Color.xyz * DoDiffuse(lightDir, normal) * NEMat_Diffuse1.Sample(NEMat_Diffuse1_sampler, TexCoords).xyz;
    float3 specular = light.Color.xyz * DoBlinnSpecular(normal, lightDir, viewDir) * NEMat_Specular1.Sample(NEMat_Specular1_sampler, TexCoords).xyz;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
	return (ambient + diffuse + specular);// *ModelColor;
}

// calculates the color when using a spot light.
float3 CalcSpotLight(SpotLight light, float3 normal, float3 fragPos, float3 viewDir, float2 TexCoords)
{
    float3 lightDir = normalize(light.Position.xyz - fragPos);

    // attenuation
    float attenuation = DoQuadraticAttenuation(light.Intensity_Attenuation, light.Position.xyz, fragPos);

    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.Direction.xyz));
    float epsilon = light.InnerCutOf_OuterCutoff.x - light.InnerCutOf_OuterCutoff.y;
    float intensity = clamp((theta - light.InnerCutOf_OuterCutoff.y) / epsilon, 0.0, 1.0);

    // combine results
    float3 ambient = 0.05f * float3(light.Color.xyz * NEMat_Diffuse1.Sample(NEMat_Diffuse1_sampler, TexCoords).xyz);
    float3 diffuse = light.Color.xyz * DoDiffuse(lightDir, normal) * NEMat_Diffuse1.Sample(NEMat_Diffuse1_sampler, TexCoords).xyz;
    float3 specular = light.Color.xyz * DoBlinnSpecular(normal, lightDir, viewDir) * NEMat_Specular1.Sample(NEMat_Specular1_sampler, TexCoords).xyz;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

float4 DoLighting(PixelInputType input)
{
    // properties
    float3 norm = normalize(input.Normal);
    float3 viewDir = normalize(ViewPos.xyz - input.FragPos);
    float3 result = float3(0.0f, 0.0f, 0.0f);


#ifdef NE_USE_NORMAL_MAPS
    norm = NEMat_Normal1.Sample(NEMat_Normal1_sampler, input.TexCoords).xyz;
    norm = normalize(mul(norm, 2.0f) - 1.0f);
    norm = normalize(mul(norm, input.TBN));
#endif

#ifdef NE_DIR_LIGHTS_NUM
  // phase 1: directional lighting
    for (int i0 = 0; i0 < NE_DIR_LIGHTS_NUM; i0++)
    {
        result += CalcDirLight(DirLights[i0], norm, viewDir, input.TexCoords);
    }
#endif
#ifdef NE_POINT_LIGHTS_NUM  
    // phase 2: point lights
    for (int i1 = 0; i1 < NE_POINT_LIGHTS_NUM; i1++)
    {
        result += CalcPointLight(PointLights[i1], norm, input.FragPos, viewDir, input.TexCoords);
    }
#endif
#ifdef NE_SPOT_LIGHTS_NUM
    // phase 3: spot light
    for (int i2 = 0; i2 < NE_SPOT_LIGHTS_NUM; i2++)
    {
    
        result += CalcSpotLight(SpotLights[i2], norm, input.FragPos, viewDir, input.TexCoords);
    }
#endif

    return float4(result, 1.0f);
}
