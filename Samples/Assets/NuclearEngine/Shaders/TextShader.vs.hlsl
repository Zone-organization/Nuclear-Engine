/*#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

layout(std140) uniform NE_Projection_CB
{
    mat4 projection;
};
void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}  */

struct VertexInputType
{
	// <vec2 pos, vec2 tex>
    float4 Vertex : POSITION0;
};

struct PixelInputType
{    
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
};
cbuffer NE_Projection_CB
{
	matrix Projection;
};

PixelInputType main(VertexInputType input)
{    
	PixelInputType output;

    output.Position = mul(float4(input.Vertex.xy, 0.0f, 1.0f) ,Projection);
	output.TexCoord = input.Vertex.zw;
	
	return output;
} 