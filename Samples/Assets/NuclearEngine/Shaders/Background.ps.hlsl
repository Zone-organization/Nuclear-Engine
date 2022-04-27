struct PixelInputType
{
    float4 Position : SV_POSITION;
	float3 Tex : TEXCOORD;
};

TextureCube NE_Skybox : register(t0);;
SamplerState NE_Skybox_sampler : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{
    return NE_Skybox.Sample(NE_Skybox_sampler, input.Tex);
}