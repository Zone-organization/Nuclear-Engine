struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD;
};

Texture2D ScreenTex : register(t0);
SamplerState ScreenTex_sampler : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{
    float3 result = ScreenTex.Sample(ScreenTex_sampler,input.Tex).rgb;

    
#ifdef NE_HDR_ENABLED
	result = result / (result + float3(1.0f, 1.0f, 1.0f)); // reinhard
#endif

#ifdef NE_GAMMA_CORRECTION_ENABLED
	float GammaCorrectionValue = 1.0f / 2.2f;
	result = pow(result, float3(GammaCorrectionValue, GammaCorrectionValue, GammaCorrectionValue));
#endif

    return float4(result, 1.0f);
}