struct VertexInputType
{
    float4 Position : ATTRIB0;
};

struct PixelInputType
{
    float4 Position : SV_POSITION;
	float3 Tex : TEXCOORD;
};

cbuffer NEStatic_Camera : register(b0)
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

	output.Tex = input.Position.xyz;

    float4x4 aview = float4x4(
			View._m00, View._m01, View._m02, 0.0f,
			View._m10, View._m11, View._m12, 0.0f,
			View._m20, View._m21, View._m22, 0.0f,
			View._m30, View._m31, View._m32, 1.0f
		);

    float4 VP;
    VP = mul(aview, input.Position);
    VP = mul(Projection, VP);

	//Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
	output.Position = VP.xyww;    
    return output;
}
