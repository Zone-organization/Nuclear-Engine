struct VertexInputType
{
    float4 Position : ATTRIB0;
};

cbuffer NEStatic_CaptureInfo : register(b0)
{
    matrix View;
    matrix Projection;
};

float4 main(VertexInputType input) : SV_POSITION
{
    float4 result;
    result = mul(View, float4(input.Position.xyz , 1.0f));
    result = mul(Projection, result);

    return result;
}
