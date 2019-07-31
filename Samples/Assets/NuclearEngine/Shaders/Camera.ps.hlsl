struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD;
};

Texture2D SceneTexture : register(t0);
SamplerState SceneTexture_sampler : register(s0);

#ifdef _NE_BLOOM_ENABLED
Texture2D BloomBlurTexture : register(t1);
SamplerState BloomBlur_sampler : register(s1);
#endif

float4 main(PixelInputType input) : SV_TARGET
{
    float3 SceneColor = SceneTexture.Sample(SceneTexture_sampler, input.Tex).rgb;

#ifdef _NE_BLOOM_ENABLED
	float3 BloomColor = BloomBlurTexture.Sample(BloomBlur_sampler, input.Tex).rgb;

	SceneColor += BloomColor;
#endif

#ifdef NE_HDR_ENABLED
	SceneColor = SceneColor / (SceneColor + float3(1.0f, 1.0f, 1.0f));
#endif

#ifdef NE_GAMMA_CORRECTION_ENABLED
	float GammaCorrectionValue = 1.0f / 2.2f;
	SceneColor = pow(SceneColor, float3(GammaCorrectionValue, GammaCorrectionValue, GammaCorrectionValue));
#endif

    return float4(SceneColor, 1.0f);
}