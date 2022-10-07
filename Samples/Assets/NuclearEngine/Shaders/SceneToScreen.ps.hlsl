struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

Texture2D NEMat_RenderTargetTex : register(t0);
SamplerState NEMat_RenderTargetTex_sampler : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{
    return float4(NEMat_RenderTargetTex.Sample(NEMat_RenderTargetTex_sampler, input.TexCoord).xyz, 1.0f);
}