/*
Defines:
#define NE_USE_DEF_CAMERA
#define NE_OUT_FRAG_POS

*/
//#define NE_USE_DEF_CAMERA
//#define NE_OUT_FRAG_POS

struct VertexInputType
{
    float4 Position : ATTRIB0;
    float2 TexCoord : ATTRIB1;
#ifdef NE_USE_DEF_CAMERA
    float3 Normals : ATTRIB2;
    float3 Tangents : ATTRIB3;
    float3 Bitangents : ATTRIB4;
    int4 BoneIDs : ATTRIB5;
    float4 Weights : ATTRIB6;
#endif
};


struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
#ifdef NE_USE_DEF_CAMERA
    float3 Normals : NORMAL0;
    float3 FragPos : TEXCOORD1;
    float3x3 TBN : TANGENT0;
#endif
};

#ifdef NE_USE_DEF_CAMERA
cbuffer NEStatic_Camera : register(b0)
{
    matrix Model;
    matrix ModelInvTranspose;
    matrix ModelViewProjection;

    //Needed for some objects (as skybox & 2D Sprites & etc)
    matrix View;
    matrix Projection;
};
#endif
#ifdef NE_USE_DEF_CAMERA

#define MAX_BONES  100
#define MAX_BONE_INFLUENCE 4

cbuffer NEStatic_Animation : register(b1)
{
    matrix BoneTransforms[MAX_BONES];
};
#endif

PixelInputType main(VertexInputType input)
{
    PixelInputType output;

#ifdef NE_USE_DEF_CAMERA

    float4 FinalPos = float4(input.Position.xyz, 1.0f);
    float4 FinalNorm = float4(input.Normals.xyz, 0.0f);

    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if (input.BoneIDs[i] == -1)
        {
            continue;
        }
        if (input.BoneIDs[i] >= MAX_BONES)
        {
            break;
        }
        FinalPos = float4(0.0f,0.0f,0.0f, 0.0f);
        FinalNorm = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 localPosition = mul(BoneTransforms[input.BoneIDs[i]], float4(input.Position.xyz, 1.0f));
        FinalPos += mul(input.Weights[i], float4(localPosition.xyz, 1.0f));

        float3 localNormal = mul((float3x3)BoneTransforms[input.BoneIDs[i]], input.Normals);
        FinalNorm += mul(input.Weights[i], float4(localNormal, 0.0f));
    }

    output.Position = mul(ModelViewProjection, FinalPos);

#else
    output.Position = input.Position;
#endif

    output.TexCoord = input.TexCoord;


#ifdef NE_USE_DEF_CAMERA
    output.Normals = mul((float3x3)ModelInvTranspose, FinalNorm.xyz);
#endif


#ifdef NE_OUT_FRAG_POS
    output.FragPos = mul(Model, FinalPos).xyz;
#endif

#ifdef NE_USE_DEF_CAMERA
    float3 T = normalize(mul(float4(input.Tangents.xyz, 0.0f), Model).xyz);
    float3 B = normalize(mul(float4(input.Bitangents.xyz, 0.0f), Model).xyz);
    float3 N = normalize(mul(float4(FinalNorm.xyz, 0.0f), Model).xyz);

    output.TBN = float3x3(T, B, N);
#endif

    return output;
}