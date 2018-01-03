struct VertexInputType
{
    float4 Position : POSITION;
};

struct PixelInputType
{
    float4 Position : SV_POSITION;
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
    output.Position = mul(mul(float4(input.Position.xy, 0.0f, 1.0f), Model), Projection);

	// Store the input texture for the pixel shader to use.
    output.TexCoord = float2(input.Position.z, input.Position.w);
    
    return output;
}