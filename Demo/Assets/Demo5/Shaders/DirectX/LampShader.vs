struct VertexInputType
{
    float4 Position : POSITION;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
};

cbuffer NE_Camera : register(b0)
{
	matrix Model;
	matrix View;
	matrix Projection;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	
	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(Model, input.Position);
    output.position = mul(View, output.position);
    output.position = mul(Projection, output.position);

    return output;
}