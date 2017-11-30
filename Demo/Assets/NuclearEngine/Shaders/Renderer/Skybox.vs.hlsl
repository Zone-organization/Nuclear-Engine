//struct VertexInputType
//{
//    float4 Position : POSITION;
//};
//
//struct PixelInputType
//{
//    float4 Position : SV_POSITION;
//	float3 Tex : TEXCOORD;
//};
//
//cbuffer NE_Camera : register(b0)
//{
//	float4x4 Model;
//	float4x4 View;
//	float4x4 Projection;
//};
//
//PixelInputType main(VertexInputType input)
//{
//    PixelInputType output;
//
//	output.Tex = input.Position.xyz;
//
//	float4x4 aview = float4x4(
//			View._m00, View._m01, View._m02, 0.0f,
//			View._m10, View._m11, View._m12, 0.0f,
//			View._m20, View._m21, View._m22, 0.0f,
//			0.0f, 0.0f, 0.0f, 1.0f
//		);
//
//	float4 MVP = mul(Projection, mul(aview, float4(input.Position.xyz, 1.0f)));
//
//	//Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
//	output.Position = MVP.xyww;    
//    return output;
//}
//
#version 330 core
in vec3 aPos;
out vec3 TexCoords;
layout (std140) uniform NE_Camera
{
	 mat4 model;
	 mat4 view;
	 mat4 projection;
};
void main()
{
    TexCoords = aPos;
    vec4 pos = projection * mat4(mat3(view)) * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}