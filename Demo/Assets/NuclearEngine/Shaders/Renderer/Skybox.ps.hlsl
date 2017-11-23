struct PixelInputType
{
    float4 Position : SV_POSITION;
	float2 Tex : TEXCOORD;
};

TextureCube NE_SkyboxTexture : register(t0);;
SamplerState NE_SkyboxSampler : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{
    return NE_SkyboxTexture.Sample(NE_SkyboxSampler, input.Position);
}