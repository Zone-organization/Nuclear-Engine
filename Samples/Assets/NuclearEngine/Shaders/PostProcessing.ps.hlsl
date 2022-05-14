struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD;
};

Texture2D NEMat_RenderTargetTex : register(t0);
SamplerState NEMat_RenderTargetTex_sampler : register(s0);

#ifdef BLOOM
Texture2D  NEMat_BloomBlurTex : register(t1);
SamplerState  NEMat_BloomBlurTex_sampler : register(s1);
#endif

float4 main(PixelInputType input) : SV_TARGET
{
    float3 SceneColor = NEMat_RenderTargetTex.Sample(NEMat_RenderTargetTex_sampler, input.Tex).rgb;

#ifdef HDR
	SceneColor = SceneColor / (SceneColor + float3(1.0f, 1.0f, 1.0f));
#endif

#ifdef BLOOM

	float3 BloomColor = NEMat_BloomBlurTex.Sample(NEMat_BloomBlurTex_sampler, input.Tex).rgb;

	SceneColor += BloomColor;
#endif


#ifdef GAMMACORRECTION
	float GammaCorrectionValue = 1.0f / 2.2f;
	SceneColor = pow(abs(SceneColor), float3(GammaCorrectionValue, GammaCorrectionValue, GammaCorrectionValue));
#endif

    return float4(SceneColor, 1.0f);
}