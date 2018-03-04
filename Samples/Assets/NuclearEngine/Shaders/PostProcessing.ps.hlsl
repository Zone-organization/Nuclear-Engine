struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD;
};

Texture2D NE_Screen_Texture : register(t0);;
SamplerState NE_Screen_Sampler : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{
    float3 result = NE_Screen_Texture.Sample(NE_Screen_Sampler,input.Tex).rgb;

    float GammaCorrectionValue = 1.0f / 2.2f;
    
#ifdef NE_HDR_ENABLED  
    // reinhard
    result = result / (result + float3(1.0f));
#endif

#ifdef NE_GAMMA_CORRECTION_ENABLED
    result = pow(result, float3(GammaCorrectionValue));
#endif
    return float4(result, 1.0f);
}