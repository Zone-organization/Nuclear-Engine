
struct PixelInputType
{
    float4 Position : SV_POSITION;
#ifdef NE_OUTPUT_TEXTURE
    float2 UV : TEXCOORD0;
#endif
};

Texture2D NEMat_Diffuse1 : register(t0);;
SamplerState NEMat_Diffuse1_sampler : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{
    float4 result = float4(1.0f, 1.0f, 1.0f, 1.0f);
#ifdef NE_OUTPUT_TEXTURE
    result = NEMat_Diffuse1.Sample(NEMat_Diffuse1_sampler, input.UV);
#endif
    return result;
}