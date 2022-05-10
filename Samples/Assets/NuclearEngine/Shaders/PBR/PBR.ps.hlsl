//Definitions:
//#define NE_DIR_LIGHTS_NUM 1
//#define NE_POINT_LIGHTS_NUM 1
//#define NE_SPOT_LIGHTS_NUM 1
#include "..\LightTypes.hlsli"
#include "Common.hlsl"

struct PixelInputType
{
	float4 Position : SV_POSITION;
	float2 TexCoords : TEXCOORD0;
#ifndef NE_DEFFERED
	float3 Normal : NORMAL0;
	float3 FragPos : TEXCOORD1;
	float3x3 TBN : TANGENT0;
#endif
};

#ifdef NE_DEFFERED

Texture2D NE_RT_GBuffer_Position : register(t0);
Texture2D NE_RT_GBuffer_Normal : register(t1);
Texture2D NE_RT_GBuffer_AlbedoMatallic : register(t2);
Texture2D NE_RT_GBuffer_RoughnessAO : register(t3);

SamplerState NE_RT_GBuffer_Position_sampler : register(s0);
SamplerState NE_RT_GBuffer_Normal_sampler : register(s1);
SamplerState NE_RT_GBuffer_AlbedoMatallic_sampler : register(s2);
SamplerState NE_RT_GBuffer_RoughnessAO_sampler : register(s3);  //1 Component

#else
Texture2D NEMat_Albedo : register(t0);
Texture2D NEMat_Metallic : register(t1);
Texture2D NEMat_Normal : register(t2); 
Texture2D NEMat_Roughness : register(t3);
Texture2D NEMat_AO : register(t4);

#ifdef IBL_ENABLED
TextureCube NEMat_LC_IrradianceMap : register(t5);
TextureCube NEMat_LC_PrefilterMap : register(t6);
Texture2D NEMat_LC_BRDF_LUT : register(t7);
#endif

SamplerState NEMat_Albedo_sampler : register(s0);
SamplerState NEMat_Metallic_sampler : register(s1);
SamplerState NEMat_Normal_sampler : register(s2);
SamplerState NEMat_Roughness_sampler : register(s3);
SamplerState NEMat_AO_sampler : register(s4);

#ifdef IBL_ENABLED
SamplerState NEMat_LC_IrradianceMap_sampler : register(s5);
SamplerState NEMat_LC_PrefilterMap_sampler : register(s6);
SamplerState NEMat_LC_BRDF_LUT_sampler : register(s7);
#endif

#endif

float3 CalcPointLight(PointLight light, float3 N, float3 WorldPos, float3 V, float3 F0, float3 albedo, float metallic, float roughness)
{
	// calculate per-light radiance
	float3 L = normalize(light.Position.xyz - WorldPos);
	float3 H = normalize(V + L);
	float attenuation = DoQuadraticAttenuation(light.Intensity_Attenuation, light.Position.xyz, WorldPos);
	float3 radiance = light.Color.xyz * attenuation;

	// Cook-Torrance BRDF
	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

	float3 nominator = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // 0.0001 to prevent divide by zero.
	float3 specular = nominator / denominator;

	// kS is equal to Fresnel
	float3 kS = F;
	// for energy conservation, the diffuse and specular light can't
	// be above 1.0 (unless the surface emits light); to preserve this
	// relationship the diffuse component (kD) should equal 1.0 - kS.
	float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
	// multiply kD by the inverse metalness such that only non-metals 
	// have diffuse lighting, or a linear blend if partly metal (pure metals
	// have no diffuse light).
	kD *= 1.0 - metallic;

	// scale light by NdotL
	float NdotL = max(dot(N, L), 0.0);

	// add to outgoing radiance Lo
	return float3( (kD * albedo / PI + specular) * radiance * NdotL);

}
// ----------------------------------------------------------------------------
float3 CalcDirLight(DirLight light, float3 N, float3 WorldPos, float3 V, float3 F0, float3 albedo, float metallic, float roughness)
{
	// calculate per-light radiance
	float3 L = normalize(-light.Direction.xyz);
	float3 H = normalize(V + L);
	float3 radiance = light.Color.xyz;

	// Cook-Torrance BRDF
	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	float3 F = fresnelSchlick(max(dot(H, V), 0.0f), F0);

	float3 nominator = NDF * G * F;
	float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
	float3 specular = nominator / denominator;

	// kS is equal to Fresnel
	float3 kS = F;
	// for energy conservation, the diffuse and specular light can't
	// be above 1.0 (unless the surface emits light); to preserve this
	// relationship the diffuse component (kD) should equal 1.0 - kS.
	float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
	// multiply kD by the inverse metalness such that only non-metals 
	// have diffuse lighting, or a linear blend if partly metal (pure metals
	// have no diffuse light).
	kD *= 1.0 - metallic;

	// scale light by NdotL
	float NdotL = max(dot(N, L), 0.0);

	// add to outgoing radiance Lo
	return float3((kD * albedo / PI + specular) * radiance * NdotL);

}
// ----------------------------------------------------------------------------
float3 CalcSpotLight(SpotLight light, float3 N, float3 WorldPos, float3 V, float3 F0, float3 albedo, float metallic, float roughness)
{
	// calculate per-light radiance
	float3 L = normalize(light.Position.xyz - WorldPos);
	float3 H = normalize(V + L);
	float attenuation = DoQuadraticAttenuation(light.Intensity_Attenuation, light.Position.xyz, WorldPos);

	// spotlight intensity
	float theta = dot(L, normalize(-light.Direction.xyz));
	float epsilon = light.InnerCutOf_OuterCutoff.x - light.InnerCutOf_OuterCutoff.y;
	float intensity = saturate((theta - light.InnerCutOf_OuterCutoff.y) / epsilon);

	float3 radiance = light.Color.xyz * attenuation * intensity;

	// Cook-Torrance BRDF
	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

	float3 nominator = NDF * G * F;
	float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
	float3 specular = nominator / denominator;

	// kS is equal to Fresnel
	float3 kS = F;
	// for energy conservation, the diffuse and specular light can't
	// be above 1.0 (unless the surface emits light); to preserve this
	// relationship the diffuse component (kD) should equal 1.0 - kS.
	float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
	// multiply kD by the inverse metalness such that only non-metals 
	// have diffuse lighting, or a linear blend if partly metal (pure metals
	// have no diffuse light).
	kD *= 1.0 - metallic;

	// scale light by NdotL
	float NdotL = max(dot(N, L), 0.0);

	// add to outgoing radiance Lo
	return float3((kD * albedo / PI + specular) * radiance * NdotL);

}
struct PS_OUTPUT
{
	float4 Color: SV_Target0;
#ifdef BLOOM
	float4 Bloom: SV_Target1;
#endif
};

// ----------------------------------------------------------------------------
PS_OUTPUT main(PixelInputType input) : SV_TARGET
{
#ifdef NE_DEFFERED
	float3 FragPos = NE_RT_GBuffer_Position.Sample(NE_RT_GBuffer_Position_sampler, input.TexCoords).xyz;
	float3 albedo = pow(NE_RT_GBuffer_AlbedoMatallic.Sample(NE_RT_GBuffer_AlbedoMatallic_sampler, input.TexCoords).rgb, float3(2.2f,2.2f,2.2f));
	float metallic = NE_RT_GBuffer_AlbedoMatallic.Sample(NE_RT_GBuffer_AlbedoMatallic_sampler, input.TexCoords).a;
	float roughness = NE_RT_GBuffer_RoughnessAO.Sample(NE_RT_GBuffer_RoughnessAO_sampler, input.TexCoords).r;
	float ao = NE_RT_GBuffer_RoughnessAO.Sample(NE_RT_GBuffer_RoughnessAO_sampler, input.TexCoords).g;
	float3 N = normalize(NE_RT_GBuffer_Normal.Sample(NE_RT_GBuffer_Normal_sampler, input.TexCoords).rgb);
#else
	float3 FragPos = input.FragPos;
	float3 albedo = pow(NEMat_Albedo.Sample(NEMat_Albedo_sampler, input.TexCoords).xyz, float3(2.2f,2.2f,2.2f));
	float metallic = NEMat_Metallic.Sample(NEMat_Metallic_sampler, input.TexCoords).x;
	float roughness = NEMat_Roughness.Sample(NEMat_Roughness_sampler, input.TexCoords).x;
	float ao = NEMat_AO.Sample(NEMat_AO_sampler, input.TexCoords).x;
	float3 N = NEMat_Normal.Sample(NEMat_Normal_sampler, input.TexCoords).xyz;
	N = normalize(mul(N, 2.0f) - 1.0f);
	N = normalize(mul(N, input.TBN));
#endif

	float3 V = normalize(ViewPos.xyz - FragPos);

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
	// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
	float3 F0 = float3(0.04f, 0.04f, 0.04f);
	F0 = lerp(F0, albedo, metallic);

	// reflectance equation
	float3 Lo = float3(0.0f, 0.0f, 0.0f);
#ifdef NE_DIR_LIGHTS_NUM
	// phase 1: directional lighting
	for (int i0 = 0; i0 < NE_DIR_LIGHTS_NUM; i0++)
	{
		Lo += CalcDirLight(DirLights[i0], N, FragPos, V, F0, albedo, metallic, roughness);
	}
#endif
#ifdef NE_POINT_LIGHTS_NUM  
	// phase 2: point lights
	for (int i1 = 0; i1 < NE_POINT_LIGHTS_NUM; i1++)
	{
		Lo += CalcPointLight(PointLights[i1], N, FragPos, V, F0, albedo, metallic, roughness);
	}
#endif
#ifdef NE_SPOT_LIGHTS_NUM
	// phase 3: spot light
	for (int i2 = 0; i2 < NE_SPOT_LIGHTS_NUM; i2++)
	{
		Lo += CalcSpotLight(SpotLights[i2], N, FragPos, V, F0, albedo, metallic, roughness);
	}
#endif

	PS_OUTPUT output;

///////////////////////////////////////Image based Lighting///////////////////////////////////////
#ifdef IBL_ENABLED
	float3 R = reflect(-V, N);

	// ambient lighting (we now use IBL as the ambient term)
	float3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0f), F0, roughness);

	float3 kS = F;
	float3 kD = 1.0f - kS;
	kD *= 1.0f - metallic;

	float3 irradiance = NEMat_LC_IrradianceMap.Sample(NEMat_LC_IrradianceMap_sampler, N).rgb;
	float3 diffuse = irradiance * albedo;
	// sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
	const float MAX_REFLECTION_LOD = 4.0;
	float3 prefilteredColor = NEMat_LC_PrefilterMap.SampleLevel(NEMat_LC_PrefilterMap_sampler, R, roughness * MAX_REFLECTION_LOD).rgb;
	float2 brdf = NEMat_LC_BRDF_LUT.Sample(NEMat_LC_BRDF_LUT_sampler, float2(max(dot(N, V), 0.0), roughness)).rg;
	float3 specular = prefilteredColor * (F * brdf.x + brdf.y);

	float3 ambient = (kD * diffuse + specular) * ao;

	float3 color = ambient + Lo;
#else
	float3 ambient = float3(0.03f, 0.03f, 0.03f) * albedo * ao;

	float3 color = ambient + Lo;

#endif

	output.Color = float4(color, 1.0f);

#ifdef BLOOM
	float brightness = dot(color, float3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0)
		output.Bloom = float4(color, 1.0);
	else
		output.Bloom = float4(0.0, 0.0, 0.0, 1.0);
#endif
	return output;
}

