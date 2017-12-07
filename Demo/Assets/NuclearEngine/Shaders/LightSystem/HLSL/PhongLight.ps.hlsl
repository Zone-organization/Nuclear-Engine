struct PixelInputType
{
    float4 FragPos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 TexCoords : TEXCOORD;
};

Texture2D NE_Tex_Diffuse : register(t0);
SamplerState NE_Tex_Diffuse_Sample : register(s0);

Texture2D NE_Tex_Specular : register(t1);
SamplerState NE_Tex_Specular_Sample : register(s1);

static float4 ambientvalue = float4(0.05f, 0.05f, 0.05f,1.0f);

struct DirLight {
	float4 direction;
	float4 color;
};

struct PointLight {
	float4 position;
	float4 Intensity_attenuation;
	float4 color;
};

struct SpotLight {
	float4 position;
	float4 direction;
	float4 Intensity_attenuation;
	float4 SpotlightCutOf;
	float4 color;
};

cbuffer NE_LightUBO
{
	float4 viewPosandMaterialshininess;
#ifdef NR_DIR_LIGHTS
	DirLight dirLights[NR_DIR_LIGHTS];
#endif
#ifdef NR_POINT_LIGHTS
	PointLight pointLights[NR_POINT_LIGHTS];
#endif
#ifdef NR_SPOT_LIGHTS
	SpotLight spotLights[NR_SPOT_LIGHTS];
#endif
};

// function prototypes
float3 CalcDirLight(DirLight light, float3 normal, float3 viewDir, float2 tex);
float4 CalcPointLight(PointLight light, float4 normal, float4 fragPos, float4 viewDir,float2 tex);
float4 CalcSpotLight(SpotLight light, float4 normal, float4 fragPos, float4 viewDir,float2 tex);

float4 main(PixelInputType input) : SV_TARGET
{
	// properties
	float3 norm = normalize(input.Normal);
	float3 viewDir = normalize(viewPosandMaterialshininess.xyz - input.FragPos.xyz);
	float3 FragColor;
#ifdef NR_DIR_LIGHTS
	for (int i0 = 0; i0 < NR_DIR_LIGHTS; i0++)
		FragColor = CalcDirLight(dirLights[i0], norm, viewDir, input.TexCoords);
#endif

#ifdef NR_POINT_LIGHTS
	//for (int i1 = 0; i1 < NR_POINT_LIGHTS; i1++)
	//	FragColor += CalcPointLight(pointLights[i1], norm, input.FragPos, viewDir, input.TexCoords);
#endif

#ifdef NR_SPOT_LIGHTS
	//for (int i2 = 0; i2 < NR_SPOT_LIGHTS; i2++)
	//	FragColor += CalcSpotLight(spotLights[i2], norm, input.FragPos, viewDir, input.TexCoords);
#endif
	return float4(FragColor, 1.0f);
}
// calculates the color when using a directional light.
float3 CalcDirLight(DirLight light, float3 normal, float3 viewDir, float2 tex)
{
	float3 lightDir = normalize(-light.direction.xyz);

	// diffuse shading
	float diff = max(0, dot(normal, lightDir));
	// specular shading
	//float3 reflectDir = reflect(-lightDir, normal);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), viewPosandMaterialshininess.w);
	// combine results
	float3 ambient = ambientvalue * NE_Tex_Diffuse.Sample(NE_Tex_Diffuse_Sample, tex);
	float3 diffuse = light.color * diff * NE_Tex_Diffuse.Sample(NE_Tex_Diffuse_Sample, tex);
	//float3 specular = light.color * spec * NE_Specular_Map.Sample(specularmapsample, tex);
	return (ambient + diffuse);
}

//float4 CalcDirLight(DirLight light, float4 normal, float4 viewDir,float2 tex)
//{
//	float4 lightDir = normalize(-light.direction);
//	// diffuse shading
//	float diff = max(dot(normal, lightDir), 0.0);
//	// specular shading
//	float4 reflectDir = reflect(-lightDir, normal);
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), viewPosandMaterialshininess.w);
//	// combine results
//	float4 ambient = ambientvalue * NE_Diffuse_Map.Sample(diffusemapsample, tex);
//	float4 diffuse = light.color * diff * NE_Diffuse_Map.Sample(diffusemapsample, tex);
//	float4 specular = light.color * spec * 	NE_Specular_Map.Sample(specularmapsample, tex);
//	return (ambient + diffuse + specular);
//}

// calculates the color when using a point light.
float4 CalcPointLight(PointLight light, float4 normal, float4 fragPos, float4 viewDir,float2 tex)
{
	float4 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	float4 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), viewPosandMaterialshininess.w);
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = light.Intensity_attenuation.x / (light.Intensity_attenuation.y + light.Intensity_attenuation.z * distance + light.Intensity_attenuation.w * (distance * distance));
	
	// combine results
	float3 ambient = ambientvalue * NE_Tex_Diffuse.Sample(NE_Tex_Diffuse_Sample, tex);
	float3 diffuse = light.color * diff * NE_Tex_Diffuse.Sample(NE_Tex_Diffuse_Sample, tex);
	//float4 specular = light.color * spec * 	NE_Specular_Map.Sample(specularmapsample, tex);
	ambient = ambient * attenuation;
	diffuse = diffuse * attenuation;
	specular = specular * attenuation;
	return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
float4 CalcSpotLight(SpotLight light, float4 normal, float4 fragPos, float4 viewDir,float2 tex)
{
	float4 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	float4 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), viewPosandMaterialshininess.w);
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = light.Intensity_attenuation.x / (light.Intensity_attenuation.y + light.Intensity_attenuation.z * distance + light.Intensity_attenuation.w * (distance * distance));

	// spotlight intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.SpotlightCutOf.x - light.SpotlightCutOf.y;
	float intensity = clamp((theta - light.SpotlightCutOf.y) / epsilon, 0.0, 1.0);
	// combine results
	float3 ambient = ambientvalue * NE_Tex_Diffuse.Sample(NE_Tex_Diffuse_Sample, tex);
	float3 diffuse = light.color * diff * NE_Tex_Diffuse.Sample(NE_Tex_Diffuse_Sample, tex);
	//float4 specular = light.color * spec * 	NE_Specular_Map.Sample(specularmapsample, tex);
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}