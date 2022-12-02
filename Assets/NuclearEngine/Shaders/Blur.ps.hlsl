struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
	float2 texCoord4 : TEXCOORD4;
	float2 texCoord5 : TEXCOORD5;
	float2 texCoord6 : TEXCOORD6;
	float2 texCoord7 : TEXCOORD7;
	float2 texCoord8 : TEXCOORD8;
	float2 texCoord9 : TEXCOORD9;
};


Texture2D Texture : register(t0);
SamplerState Texture_sampler : register(s0);

static float weight[5] = { 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162 };


float4 main(PixelInputType input) : SV_TARGET
{
	float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Add the nine vertical pixels to the color by the specific weight of each.
	color += Texture.Sample(Texture_sampler, input.texCoord1) * weight[4];
	color += Texture.Sample(Texture_sampler, input.texCoord2) * weight[3];
	color += Texture.Sample(Texture_sampler, input.texCoord3) * weight[2];
	color += Texture.Sample(Texture_sampler, input.texCoord4) * weight[1];
	color += Texture.Sample(Texture_sampler, input.texCoord5) * weight[0];
	color += Texture.Sample(Texture_sampler, input.texCoord6) * weight[1];
	color += Texture.Sample(Texture_sampler, input.texCoord7) * weight[2];
	color += Texture.Sample(Texture_sampler, input.texCoord8) * weight[3];
	color += Texture.Sample(Texture_sampler, input.texCoord9) * weight[4];

	// Set the alpha channel to one.
	color.a = 1.0f;

	return color;
}