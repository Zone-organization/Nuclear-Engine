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

float4 DirShadowMapDepthVS(VertexInputType input) : SV_POSITION
{ 
     //TODO: Support animation

      float4 pos = mul(mul(LightSpace, Model), float4(input.Position.xyz, 1.f));

      return pos;
}

void DirShadowMapDepthPS(float4 Position : SV_POSITION)
{
    //Do nothing
}