struct VertexInputType
{
    float4 Position : POSITION0;
#ifdef NE_USE_NORMALS
	float4 Normal : NORMAL0;
#endif
	float2 TexCoord : TEXCOORD0;
};

struct PixelInputType
{
    float4 Position : SV_POSITION;
#ifdef NE_USE_NORMALS
	float4 Normal : NORMAL0;
#endif
	float2 TexCoord : TEXCOORD0;
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
	output.Position = mul(Model, input.Position);
    output.Position = mul(View, output.Position);
    output.Position = mul(Projection, output.Position);

	// Store the input texture for the pixel shader to use.
    output.TexCoord = input.TexCoord;
#ifdef NE_USE_NORMALS
	output.Normal : input.Normal;
#endif

    return output;
}