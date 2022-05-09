struct PixelInputType
{
    float4 Position : SV_POSITION;
    float3 WorldPos : POSITION;
};

cbuffer NEStatic_CaptureInfo : register(b0)
{
    float4x4 g_Rotation;
};

PixelInputType main(in uint VertexId : SV_VertexID)
{
    PixelInputType result;

    float4 Pos;

    float2 PosXY[4];
    PosXY[0] = float2(-1.0, -1.0);
    PosXY[1] = float2(-1.0, +1.0);
    PosXY[2] = float2(+1.0, -1.0);
    PosXY[3] = float2(+1.0, +1.0);
    Pos = float4(PosXY[VertexId], 1.0, 1.0);
    float4 f4WorldPos = mul(g_Rotation, Pos);
#if (defined(GLSL) || defined(GL_ES)) && !defined(VULKAN)
    Pos.y *= -1.0;
#endif

    result.Position = Pos;
    result.WorldPos = f4WorldPos.xyz / f4WorldPos.w;
    return result;
}
