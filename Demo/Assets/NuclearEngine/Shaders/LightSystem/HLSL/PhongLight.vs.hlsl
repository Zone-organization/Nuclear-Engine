struct VertexInputType
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD;
};

struct PixelInputType
{
    float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD;
};

cbuffer NE_Camera
{
	matrix Model;
	matrix View;
	matrix Projection;
}; 

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	
	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.Position = mul(Model, input.Position);
	output.Position = mul(View, output.Position);
	output.Position = mul(Projection, output.Position);

	// Store the input texture for the pixel shader to use.
    output.TexCoord = input.TexCoord;

	output.Normal = input.Normal;

    return output;
}