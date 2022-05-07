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
    OutPos = mul(View, float4(input.Position.xyz , 1.0f));
    OutPos = mul(Projection, OutPos);

    result.Position = OutPos;
    result.LocalPos = input.Position.xyz;

    return result;
}
