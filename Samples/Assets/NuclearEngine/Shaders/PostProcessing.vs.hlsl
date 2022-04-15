struct VertexInputType
{
	float4 Position : ATTRIB0;	
	float2 TexCoord : ATTRIB1;
};

struct PixelInputType
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	output.Position = input.Position;
	output.TexCoord = input.TexCoord;
	return output;
}