struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

Texture2D NEMat_RenderTargetTex : register(t0);
SamplerState NEMat_RenderTargetTex_sampler : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{
    float bloom = float4(0.0, 0.0, 0.0, 1.0);

    float3 color = NEMat_RenderTargetTex.Sample(NEMat_RenderTargetTex_sampler, input.TexCoord).xyz;

    float brightness = dot(color, float3(0.2126, 0.7152, 0.0722));

    if (brightness > 1.0)
    {
        bloom = float4(color, 1.0);
    }

    return bloom;
}