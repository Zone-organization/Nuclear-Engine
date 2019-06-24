//#define NE_HDR_ENABLED
//#define NE_GAMMA_CORRECTION_ENABLED
//#define NE_CUSTOM_HDR_EXPOSURE
//#define NE_POST_PROCESS_SETTINGS
struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD;
};

Texture2D ScreenTex : register(t0);;
SamplerState ScreenTex_sampler : register(s0);

#ifdef NE_POST_PROCESS_SETTINGS
cbuffer PostProcessingSettings
{
#ifdef NE_CUSTOM_HDR_EXPOSURE
    float HDR_Exposure;
#endif
    int HDR_Enabled;
    int GammaCorrection_Enabled;
};
#endif

float4 main(PixelInputType input) : SV_TARGET
{
    float3 result = ScreenTex.Sample(ScreenTex_sampler,input.Tex).rgb;

    float GammaCorrectionValue = 1.0f / 2.2f;
    
#ifdef NE_POST_PROCESS_SETTINGS
    if (HDR_Enabled == 1)
    {
#ifdef NE_CUSTOM_HDR_EXPOSURE  
      result = float3(1.0f) - exp(-result * HDR_Exposure);
#else
      result = result / (result + float3(1.0f)); // reinhard
#endif
    }

    if (GammaCorrection_Enabled == 1)
    {
        result = pow(result, float3(GammaCorrectionValue));
    }
#else 
#ifdef NE_HDR_ENABLED  

#ifdef NE_CUSTOM_HDR_EXPOSURE  
    result = float3(1.0f) - exp(-result * HDR_Exposure);
#else
    result = result / (result + float3(1.0f)); // reinhard
#endif

#endif

#ifdef NE_GAMMA_CORRECTION_ENABLED
    result = pow(result, float3(GammaCorrectionValue));
#endif
#endif
    return float4(result, 1.0f);
}