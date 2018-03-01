struct PixelInputType
{
    float4 Position : SV_POSITION;
    float3 Tex : TEXCOORD;
};

TextureCube NE_Screen_Texture : register(t0);;
SamplerState NE_Screen_Sampler : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{
    float3 result = NE_Screen_Texture.Sample(NE_Screen_Sampler,input.Tex);
    result = pow(result, float3(1.0 / 2.2));

    return result;
}