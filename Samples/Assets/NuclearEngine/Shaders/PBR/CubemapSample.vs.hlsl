struct VertexInputType
{
    float4 Position : ATTRIB0;
};

struct PixelInputType
{
    float4 Position : SV_POSITION;
    float3 LocalPos : POSITION;
};

cbuffer NEStatic_CaptureInfo : register(b0)
{
    matrix View;
    matrix Projection;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType result;

    float4 OutPos;
    float4x4 aview = float4x4(
        View._m00, View._m01, View._m02, 0.0f,
        View._m10, View._m11, View._m12, 0.0f,
        View._m20, View._m21, View._m22, 0.0f,
        View._m30, View._m31, View._m32, 1.0f
        );
    OutPos = mul(aview, float4(input.Position.xyz , 1.0f));
    OutPos = mul(Projection, OutPos);

    result.Position = OutPos;
    result.LocalPos = input.Position.xyz;

    return result;
}
