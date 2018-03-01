
struct PixelInputType
{
    float4 Position : SV_POSITION;
#ifdef NE_OUTPUT_TEXTURE
    float3 UV : TEXCOORD;
#endif
};

TextureCube Texture : register(t0);;
SamplerState TextureSampler : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{
    float4 result = float4(1.0f, 1.0f, 1.0f, 1.0f);
#ifdef NE_OUTPUT_TEXTURE
    result = Texture.Sample(TextureSampler, input.UV);
#endif
    return result;
}