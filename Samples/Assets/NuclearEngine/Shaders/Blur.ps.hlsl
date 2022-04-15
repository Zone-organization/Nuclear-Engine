struct PixelInputType
{
	float2 Tex : TEXCOORD;
};

Texture2D Texture : register(t0);
SamplerState Texture_sampler : register(s0);

static float weight[5] = { 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162 };

cbuffer BlurOptions
{
	//X: Texture Width
	//Y: Texutre Height
	//Z: Horizental Blur Enabled  (Enabled = 2 , disabled = -2)
	//W: Unused
	int4 TextureDimensions_Horizental;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float2 tex_offset = float2(1.0f / TextureDimensions_Horizental.x, 1.0f / TextureDimensions_Horizental.y);

	float3 result = mul(weight[0], Texture.Sample(Texture_sampler, input.Tex).rgb);

	if (TextureDimensions_Horizental.z > 1)
	{
		for (int i0 = 1; i0 < 5; ++i0)
		{
			result += mul(weight[i0], Texture.Sample(Texture_sampler, input.Tex + float2(tex_offset.x * i0, 0.0)).rgb);
			result += mul(weight[i0], Texture.Sample(Texture_sampler, input.Tex - float2(tex_offset.x * i0, 0.0)).rgb);
		}
	}
	else
	{
		for (int i1 = 1; i1 < 5; ++i1)
		{
			result += mul(weight[i1], Texture.Sample(Texture_sampler, input.Tex + float2(0.0, tex_offset.y * i1)).rgb);
			result += mul(weight[i1], Texture.Sample(Texture_sampler, input.Tex - float2(0.0, tex_offset.y * i1)).rgb);
		}
	}
	return float4(result, 1.0);
}