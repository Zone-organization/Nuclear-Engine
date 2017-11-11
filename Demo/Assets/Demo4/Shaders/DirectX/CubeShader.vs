struct VertexInputType
{
    float3 position : POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD;
};

cbuffer PTE_Camera : register(b0)
{
	matrix Model;
	matrix View;
	matrix Projection;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	
	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.norm = input.norm;

	output.position = mul(Model, input.position);
    output.position = mul(View, output.position);
    output.position = mul(Projection, output.position);

	// Store the input texture for the pixel shader to use.
    output.tex = input.tex;
    
    return output;
}