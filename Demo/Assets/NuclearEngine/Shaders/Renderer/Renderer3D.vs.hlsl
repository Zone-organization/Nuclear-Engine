struct VertexInputType
{
    float4 Position : POSITION0;
	float3 Normal : NORMAL0;
	float2 TexCoord : TEXCOORD0;
};

struct PixelInputType
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL0;
	float2 TexCoord : TEXCOORD0;
    float3 FragPos : POSITION1;
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
	output.Normal = input.Normal;
    output.FragPos = mul(input.Position, Model).xyz;

    return output;
}