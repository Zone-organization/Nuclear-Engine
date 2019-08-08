struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD;
};

Texture2D NEMat_SceneTex : register(t0);
SamplerState NEMat_SceneTex_sampler : register(s0);

#ifdef _NE_BLOOM_ENABLED
Texture2D BloomBlurTexture : register(t1);
SamplerState BloomBlur_sampler : register(s1);
#endif

float4 main(PixelInputType input) : SV_TARGET
{
    float3 SceneColor = NEMat_SceneTex.Sample(NEMat_SceneTex_sampler, input.Tex).rgb;

#ifdef _NE_BLOOM_ENABLED
	float3 BloomColor = BloomBlurTexture.Sample(BloomBlur_sampler, input.Tex).rgb;

	SceneColor += BloomColor;
#endif

#ifdef NE_ENABLE_HDR
	SceneColor = SceneColor / (SceneColor + float3(1.0f, 1.0f, 1.0f));
#endif

#ifdef NE_ENABLE_GAMMA
	float GammaCorrectionValue = 1.0f / 2.2f;
	SceneColor = pow(SceneColor, float3(GammaCorrectionValue, GammaCorrectionValue, GammaCorrectionValue));
#endif

    return float4(SceneColor, 1.0f);
}