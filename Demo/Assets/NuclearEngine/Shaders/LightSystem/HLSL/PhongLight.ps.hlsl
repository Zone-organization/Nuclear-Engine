struct PixelInputType
{
    float4 PixelPos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 TexCoords : TEXCOORD;
};

Texture2D NE_Tex_Diffuse : register(t0);
SamplerState NE_Tex_Diffuse_Sample : register(s0);

Texture2D NE_Tex_Specular : register(t1);
SamplerState NE_Tex_Specular_Sample : register(s1);

static float4 ambientvalue = float4(0.05f, 0.05f, 0.05f,1.0f);

// Light types.
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

struct Material_t
{
	float4  Emissive;       // 16 bytes
	float4  Ambient;        // 16 bytes
	float4  Diffuse;        // 16 bytes
	float4  Specular;       // 16 bytes
	//X:Specular power (exponent)
	//Y:Use Texture Instead of solid colors
	float4  SpecularPower_UseTexture;  // 16 bytes
}; // 80 bytes ( 5 * 16 )

struct Light {
	//W component in Position indicate
	//0: DirLight
	//1: PointLight
	//2: SpotLight
	float4 Position;                   // 16 bytes
	float4 Direction;                  // 16 bytes
	float4 Attenuation_Intensity;      // 16 bytes
	float4 InnerCutOf_OuterCutoff;     // 16 bytes
	float4 Color;                      // 16 bytes
}; // 80 bytes ( 5 * 16 )
cbuffer NE_Material_CB
{
	Material_t Material;
};

cbuffer NE_Light_CB
{
	float4 EyePosition;
	float4 GlobalAmbient;
	Material_t Material;
#ifdef NR_LIGHTS
	Light Lights[NR_LIGHTS];
#endif
};

struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};


//Function Declarations
LightingResult DoDirectionalLight(Light light, float3 V, float4 P, float3 N);
float4 DoDiffuse(Light light, float3 L, float3 N);
float4 DoSpecular(Light light, float3 V, float3 L, float3 N);

LightingResult ComputePhongLighting(float4 P, float3 N)
{
	float3 View_Direction = normalize(EyePosition - P).xyz;

	LightingResult totalResult = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };

#ifdef NR_LIGHTS
	[unroll]
	for (int i = 0; i < NR_LIGHTS; ++i)
	{
		LightingResult result = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };

		//if (!Lights[i].Enabled) continue;

		switch (Lights[i].Position)
		{
		case DIRECTIONAL_LIGHT:
		{
			result = DoDirectionalLight(Lights[i], V, P, N);
		}
		break;
		}
		totalResult.Diffuse += result.Diffuse;
		totalResult.Specular += result.Specular;
	}
#endif

	totalResult.Diffuse = saturate(totalResult.Diffuse);
	totalResult.Specular = saturate(totalResult.Specular);

	return totalResult;
}

float4 main(PixelInputType input) : SV_TARGET
{
	LightingResult lit = ComputePhongLighting(input.PixelPos, normalize(input.Normal));

	float4 emissive = Material.Emissive;
	float4 ambient = Material.Ambient * GlobalAmbient;
	float4 diffuse = Material.Diffuse * lit.Diffuse;
	float4 specular = Material.Specular * lit.Specular;

	float4 texColor = { 1, 1, 1, 1 };

	if (Material.SpecularPower_UseTexture.y = 1.0f)
	{
		texColor = NE_Tex_Diffuse.Sample(NE_Tex_Diffuse_Sampler, input.TexCoords);
	}

	float4 finalColor = (emissive + ambient + diffuse + specular) * texColor;

	return finalColor;
}

float4 DoDiffuse(Light light, float3 L, float3 N)
{
	float NdotL = max(0, dot(N, L));
	return light.Color * NdotL;
}

float4 DoSpecular(Light light, float3 V, float3 L, float3 N)
{
	// Phong lighting.
	float3 R = normalize(reflect(-L, N));
	float RdotV = max(0, dot(R, V));

	// Blinn-Phong lighting
	float3 H = normalize(L + V);
	float NdotH = max(0, dot(N, H));

	return light.Color * pow(RdotV, Material.SpecularPower_UseTexture.x);
}

LightingResult DoDirectionalLight(Light light, float3 V, float4 P, float3 N)
{
	LightingResult result;

	float3 L = -light.Direction.xyz;

	result.Diffuse = DoDiffuse(light, L, N);
	result.Specular = DoSpecular(light, V, L, N);

	return result;
}