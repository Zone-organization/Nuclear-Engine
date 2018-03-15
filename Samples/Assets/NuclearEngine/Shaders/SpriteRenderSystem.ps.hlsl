struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

Texture2D SpriteTexture : register(t0);
SamplerState SpriteTexture_Sampler : register(s0);

cbuffer SpriteColor
{
    float4 Color;
};

float4 main(PixelInputType input) : SV_TARGET
{
    return Color *  SpriteTexture.Sample(SpriteTexture_Sampler, input.TexCoord);
}