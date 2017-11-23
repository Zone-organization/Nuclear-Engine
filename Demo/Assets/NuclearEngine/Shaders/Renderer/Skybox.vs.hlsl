struct VertexInputType
{
    float4 Position : POSITION;
};

struct PixelInputType
{
    float4 Position : SV_POSITION;
	float2 Tex : TEXCOORD;
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

	float4 MVP;
	// Calculate the position of the vertex against the world, view, and projection matrices.
	MVP = mul(View, input.Position);
	MVP = mul(Projection, MVP);

    //Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
	//MVP = mul(input.Position, MVP);

	output.Position = MVP.xyww;
    output.Tex = input.Position;
    
    return output;
}