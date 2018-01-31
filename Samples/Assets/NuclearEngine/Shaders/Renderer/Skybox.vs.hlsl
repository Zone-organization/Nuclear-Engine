struct VertexInputType
{
    float4 Position : POSITION;
};

struct PixelInputType
{
    float4 Position : SV_POSITION;
	float3 Tex : TEXCOORD;
};

cbuffer NE_Camera
{
	float4x4 Model;
	float4x4 View;
	float4x4 Projection;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;

	output.Tex = input.Position.xyz;

    float4x4 aview = float4x4(
			View._m00, View._m01, View._m02, 0.0f,
			View._m10, View._m11, View._m12, 0.0f,
			View._m20, View._m21, View._m22, 0.0f,
			View._m30, View._m31, View._m32, 1.0f
		);

    float4 MVP;
    MVP = mul(aview, input.Position);
    MVP = mul(Projection, MVP);

	//Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
	output.Position = MVP.xyww;    
    return output;
}
