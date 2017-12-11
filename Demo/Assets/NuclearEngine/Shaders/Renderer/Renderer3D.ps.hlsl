struct PixelInputType
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

Texture2D NE_Diffuse_Tex : register(t0);
SamplerState NE_Diffuse_Sampler : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{ 
	float4 result;
	#ifdef NE_LIGHT_SHADING_TECH
		result = DoLight(input.TexCoord);
	#endif
	
	return result;
}