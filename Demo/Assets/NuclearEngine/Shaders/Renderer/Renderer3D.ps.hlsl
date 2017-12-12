struct PixelInputType
{
    float4 Position : SV_POSITION;
#ifdef NE_USE_NORMALS
	float4 Normal : NORMAL0;
#endif
	float2 TexCoord : TEXCOORD0;
};


struct Light
{
	float4 Position;
	float4 Direction;
	float4 Attenuation_Intensity;
	float4 InnerCutOf_OuterCutoff;
	float4 Color;
};

#ifdef NE_LIGHT_SHADING_TECH
cbuffer NE_Light_CB
{
	float4 EyePosition;
	float4 GlobalAmbient;
	Light Lights[NE_LIGHTS_NUM];
};
#endif


float4 main(PixelInputType input) : SV_TARGET
{ 
	float4 result;
	float4 normal;

	#ifdef NE_USE_NORMALS
	normal = normalize(input.normal);
	#endif

	#ifdef NE_LIGHT_SHADING_TECH
			//   DoLighting(float4 Normal, float2 Texcoord)
		result = DoLighting(normal, input.TexCoord);
	#endif
	
	return result;
}