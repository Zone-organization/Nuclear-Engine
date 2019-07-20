//#define NE_ENABLE_POSTPROCESSING

struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD;
};

Texture2D ScreenTex : register(t0);
SamplerState ScreenTex_sampler : register(s0);

#ifdef NE_ENABLE_POSTPROCESSING
cbuffer PostProcessingSettings
{
    int HDR_Enabled;
    int GammaCorrection_Enabled;
};
#endif

float4 main(PixelInputType input) : SV_TARGET
{
    float3 result = ScreenTex.Sample(ScreenTex_sampler,input.Tex).rgb;

    float GammaCorrectionValue = 1.0f / 2.2f;
    
#define NE_ENABLE_POSTPROCESSING

    if (HDR_Enabled == 1)
    {
		result = result / (result + float3(1.0f)); // reinhard
    }

	if (GammaCorrection_Enabled == 1)
	{
		result = pow(result, float3(GammaCorrectionValue, GammaCorrectionValue, GammaCorrectionValue));

	}

#endif

    return float4(result, 1.0f);
}