struct VertexInputType
{
    float4 Position : ATTRIB0;
    float2 TexCoord : ATTRIB1;
    float3 Normals : ATTRIB2;
    float3 Tangents : ATTRIB3;
    float3 Bitangents : ATTRIB4;
    int4 BoneIDs : ATTRIB5;
    float4 Weights : ATTRIB6;
};

cbuffer NEStatic_LightInfo : register(b0)
{
    matrix Model;
    matrix LightSpace;

};

float4 SpotShadowMapDepth(VertexInputType input) : SV_POSITION
{
    float4 result;
    result = mul(Model, float4(input.Position.xyz, 1.0f));
    result = mul(LightSpace, result);

	return float4(result.xyz ,1.0f);
}