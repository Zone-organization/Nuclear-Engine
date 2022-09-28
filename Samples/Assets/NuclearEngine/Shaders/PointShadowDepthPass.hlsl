
/////////////////////////////////////////////////////////VS
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

cbuffer NEStatic_PointShadowVS
{
    matrix Model;
};

float4 PointShadowMapDepthVS(VertexInputType input) : SV_POSITION
{
   return mul(Model, float4(input.Position.xyz, 1.0f));
}


////////////////////////////////////////////////////////////GS
struct GSOutput
{
    float4 FragPos : SV_POSITION;
    uint slice : SV_RenderTargetArrayIndex;
};

cbuffer NEStatic_PointShadowGS
{
    matrix ShadowMatrices[6];
};

[maxvertexcount(18)]
void PointShadowMapDepthGS(triangle float4 input[3] : SV_POSITION,  inout TriangleStream< GSOutput > output )
{
    for (uint face = 0; face < 6; ++face)
    {
        GSOutput element;
        element.slice = face; // built-in variable that specifies to which face we render.

        for (uint i = 0; i < 3; i++)  // for each triangle's vertices
        {
            element.FragPos = mul(ShadowMatrices[face], input[i]);
            output.Append(element);
        }
        output.RestartStrip();

        //---------------------------------GLSL Reference CODE-----------------------------------
        //for (uint i = 0; i < 3; ++i)
        //{
        //    FragPos = gl_in[i].gl_Position;
        //    gl_Position = ShadowMatrices[face] * FragPos;
        //    EmitVertex();
        //}
        //EndPrimitive();
    }

   
}


///////////////////////////////////////////////////////////////////PS

cbuffer NEStatic_PointShadowPS
{
    float3 gLightPos;
    float gFarPlane;
};

float PointShadowMapDepthPS(GSOutput input) : SV_Depth
{
    float lightDistance = length(input.FragPos.xyz - gLightPos);

    //// map to [0;1] range by dividing by far_plane
    lightDistance = input.FragPos.z / (gFarPlane / 25.f);
   // lightDistance = input.FragPos.z;
    //// write this as modified depth
    return lightDistance;

}