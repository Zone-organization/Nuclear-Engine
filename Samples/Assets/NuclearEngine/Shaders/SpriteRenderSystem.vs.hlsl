struct VertexInputType
{
    float4 Position : POSITION;
	float2 TexCoord : TEXCOORD;
};

struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

cbuffer NE_Camera : register(b0)
{
    matrix Model;
    matrix ModelInvTranspose;
    matrix ModelViewProjection;

    //Needed for some objects (as skybox & 2D Sprites & etc)
    matrix View;
    matrix Projection;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.Position = mul(mul(float4(input.Position.xyz, 1.0f), Model), Projection);

	// Store the input texture for the pixel shader to use.
    output.TexCoord = input.TexCoord;
    
    return output;
}