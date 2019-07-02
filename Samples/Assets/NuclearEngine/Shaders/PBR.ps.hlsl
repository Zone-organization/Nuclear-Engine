//Definitions:
//#define NE_DIR_LIGHTS_NUM 1
//#define NE_POINT_LIGHTS_NUM 1
//#define NE_SPOT_LIGHTS_NUM 1

struct PixelInputType
{
	float4 Position : SV_POSITION;
	float2 TexCoords : TEX_COORD;
	float3 Normal : NORMAL0;
	float3 FragPos : TEXCOORD1;
	float3x3 TBN : TANGENT0;
};


Texture2D NE_Albedo;
Texture2D NE_Normal;
Texture2D NE_Metallic;
Texture2D NE_Roughness;
Texture2D NE_AmbientO;

SamplerState NE_Albedo_sampler;
SamplerState NE_Normal_sampler;
SamplerState NE_Metallic_sampler;
SamplerState NE_Roughness_sampler;
SamplerState NE_AmbientO_sampler;

struct DirLight
{
	float4 Direction;
	float4 Color;
};
struct PointLight
{
	float4 Position;
	float4 Intensity_Attenuation;
	float4 Color;
};
struct SpotLight
{
	float4 Position;
	float4 Direction;
	float4 Intensity_Attenuation;
	float4 InnerCutOf_OuterCutoff;
	float4 Color;
};
cbuffer NEStatic_Lights
{
	float4 ViewPos;
#ifdef NE_DIR_LIGHTS_NUM
	DirLight DirLights[NE_DIR_LIGHTS_NUM];
#endif
#ifdef NE_POINT_LIGHTS_NUM
	PointLight PointLights[NE_POINT_LIGHTS_NUM];
#endif

#ifdef NE_SPOT_LIGHTS_NUM
	SpotLight SpotLights[NE_SPOT_LIGHTS_NUM];
#endif

};

const float PI = 3.14159265359;

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

	return nom / denom;
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
float4 main(PixelInputType input) : SV_TARGET

{
	float3 albedo = pow(NE_Albedo.Sample(NE_Albedo_sampler, input.TexCoords).xyz, float3(2.2f,2.2f,2.2f));
	float metallic = NE_Metallic.Sample(NE_Metallic_sampler, input.TexCoords).x;
	float roughness = NE_Roughness.Sample(NE_Roughness_sampler, input.TexCoords).x;
	float ao = NE_AmbientO.Sample(NE_AmbientO_sampler, input.TexCoords).x;

	float3 N = NE_Normal.Sample(NE_Normal_sampler, input.TexCoords).xyz;
	N = normalize(mul(N, 2.0f) - 1.0f);
	N = normalize(mul(N, input.TBN));

	//float3 V = normalize(ViewPos - WorldPos);

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
	// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
	float3 F0 = float3(0.04f, 0.04f, 0.04f);
	//F0 = mix(F0, albedo, metallic);

	// reflectance equation
	float3 Lo = float3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; ++i)
	{
		// calculate per-light radiance
		float3 L = normalize(lightPositions[i] - WorldPos);
		float3 H = normalize(V + L);
		float distance = length(lightPositions[i] - WorldPos);
		float attenuation = 1.0 / (distance * distance);
		float3 radiance = lightColors[i] * attenuation;

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
		float3 kD = float3(1.0) - kS;
		// multiply kD by the inverse metalness such that only non-metals 
		// have diffuse lighting, or a linear blend if partly metal (pure metals
		// have no diffuse light).
		kD *= 1.0 - metallic;

		// scale light by NdotL
		float NdotL = max(dot(N, L), 0.0);

		// add to outgoing radiance Lo
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
	}

	// ambient lighting (note that the next IBL tutorial will replace 
	// this ambient lighting with environment lighting).
	float3 ambient = float3(0.03f, 0.03f, 0.03f) * albedo * ao;

	float3 color = ambient + Lo;

	return float4(color,1.0f);
}

