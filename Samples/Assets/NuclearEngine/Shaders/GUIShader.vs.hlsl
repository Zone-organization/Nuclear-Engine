cbuffer vertexBuffer : register(b0)
{
	float4x4 ProjectionMatrix;
};
struct VS_INPUT
{
	float2 pos : POSITION;
	float4 col : COLOR0;
	float2 uv : TEXCOORD0;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 uv : TEXCOORD0;
};

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output;
	output.pos = mul(ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));

	//float4x4 mat = { 0.00158, 0.00, 0.00, -1.00, 0.00, -0.00263, 0.00, 1.00, 0.00, 0.00, 0.50, 0.50, 0.00, 0.00, 0.00, 1.00 };
	//output.pos = mul(mat, float4(input.pos.xy, 0.f, 1.f));

	output.col = input.col;
	output.uv = input.uv;
	return output;
}
