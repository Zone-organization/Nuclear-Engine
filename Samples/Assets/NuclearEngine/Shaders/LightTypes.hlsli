
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