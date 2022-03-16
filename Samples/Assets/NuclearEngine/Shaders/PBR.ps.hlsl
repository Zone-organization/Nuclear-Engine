//Definitions:
//#define NE_DIR_LIGHTS_NUM 1
//#define NE_POINT_LIGHTS_NUM 1
//#define NE_SPOT_LIGHTS_NUM 1
#include "LightTypes.hlsli"
struct PixelInputType
{
	float4 Position : SV_POSITION;
	float2 TexCoords : TEXCOORD0;
	float3 Normal : NORMAL0;
	float3 FragPos : TEXCOORD1;
	float3x3 TBN : TANGENT0;
};

Texture2D NEMat_Albedo : register(t0);
Texture2D NEMat_Metallic : register(t1);
Texture2D NEMat_Normal : register(t2); 
Texture2D NEMat_Roughness : register(t3);
Texture2D NEMat_AO : register(t4);

SamplerState NEMat_Albedo_sampler : register(s0);
SamplerState NEMat_Metallic_sampler : register(s1);
SamplerState NEMat_Normal_sampler : register(s2);
SamplerState NEMat_Roughness_sampler : register(s3);
SamplerState NEMat_AO_sampler : register(s4);


#define PI 3.14159265359f

// ----------------------------------------------------------------------------
float DistributionGGX(float3 N, float3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / max(denom, 0.001);
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
float3 fresnelSchlick(float cosTheta, float3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
float DoQuadraticAttenuation(float4 Intensity_Attenuation, float3 lightposition, float3 fragPos)
{
	float distance = length(lightposition - fragPos);
	return Intensity_Attenuation.x / (Intensity_Attenuation.y + Intensity_Attenuation.z * distance + Intensity_Attenuation.w * (distance * distance));
}
// ----------------------------------------------------------------------------
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
	float intensity = clamp((theta - light.InnerCutOf_OuterCutoff.y) / epsilon, 0.0, 1.0);

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
// ----------------------------------------------------------------------------
float4 main(PixelInputType input) : SV_TARGET
{
	float3 albedo = pow(NEMat_Albedo.Sample(NEMat_Albedo_sampler, input.TexCoords).xyz, float3(2.2f,2.2f,2.2f));
	float metallic = NEMat_Metallic.Sample(NEMat_Metallic_sampler, input.TexCoords).x;
	float roughness = NEMat_Roughness.Sample(NEMat_Roughness_sampler, input.TexCoords).x;
	float ao = NEMat_AO.Sample(NEMat_AO_sampler, input.TexCoords).x;

	float3 N = NEMat_Normal.Sample(NEMat_Normal_sampler, input.TexCoords).xyz;
	N = normalize(mul(N, 2.0f) - 1.0f);
	N = normalize(mul(N, input.TBN));

	float3 V = normalize(ViewPos - input.FragPos);

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
	// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
	float3 F0 = float3(0.04f, 0.04f, 0.04f);
	lerp(F0, albedo, metallic);

	// reflectance equation
	float3 Lo = float3(0.0f, 0.0f, 0.0f);
#ifdef NE_DIR_LIGHTS_NUM
	// phase 1: directional lighting
	for (int i0 = 0; i0 < NE_DIR_LIGHTS_NUM; i0++)
	{
		Lo += CalcDirLight(DirLights[i0], N, input.FragPos, V, F0, albedo, metallic, roughness);
	}
#endif
#ifdef NE_POINT_LIGHTS_NUM  
	// phase 2: point lights
	for (int i1 = 0; i1 < NE_POINT_LIGHTS_NUM; i1++)
	{
		Lo += CalcPointLight(PointLights[i1], N, input.FragPos, V, F0, albedo, metallic, roughness);
	}
#endif
#ifdef NE_SPOT_LIGHTS_NUM
	// phase 3: spot light
	for (int i2 = 0; i2 < NE_SPOT_LIGHTS_NUM; i2++)
	{
		Lo += CalcSpotLight(SpotLights[i2], N, input.FragPos, V, F0, albedo, metallic, roughness);
	}
#endif

	float3 ambient = float3(0.03f, 0.03f, 0.03f) * albedo * ao;

	float3 color = ambient + Lo;

	return float4(color,1.0f);
}

