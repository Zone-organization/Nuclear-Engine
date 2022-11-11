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
    float3 Normals : ATTRIB2;
    float3 Tangents : ATTRIB3;
    float3 Bitangents : ATTRIB4;
    int4 BoneIDs : ATTRIB5;
    float4 Weights : ATTRIB6;
};


struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 Normals : NORMAL0;
    float3 FragPos : TEXCOORD1;
    float3x3 TBN : TANGENT0;
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

#define MAX_BONES  100
#define MAX_BONE_INFLUENCE 4

cbuffer NEStatic_Animation : register(b1)
{
    matrix BoneTransforms[MAX_BONES];
};

inline float3x3 UnpackNormal(float3 norm, float3 tang)
{
    const float3 T = normalize(tang - dot(norm, tang) * norm);
    const float3 N = normalize(norm);
    const float3 B = normalize(cross(T, N));
    float3x3 TBN = float3x3(T, B, N);
    return TBN;
}

PixelInputType main(VertexInputType input)
{
    PixelInputType output;


    float4 FinalPos = float4(input.Position.xyz, 1.0f);
    float4 FinalNorm = float4(input.Normals.xyz, 0.0f);

#ifdef NE_ENABLE_ANIMATION
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
        FinalPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
        FinalNorm = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 localPosition = mul(BoneTransforms[input.BoneIDs[i]], float4(input.Position.xyz, 1.0f));
        FinalPos += mul(input.Weights[i], float4(localPosition.xyz, 1.0f));

        float3 localNormal = mul((float3x3)BoneTransforms[input.BoneIDs[i]], input.Normals);
        FinalNorm += mul(input.Weights[i], float4(localNormal, 0.0f));
    }
#endif

    output.Position = mul(ModelViewProjection, FinalPos);

    output.TexCoord = input.TexCoord;

    matrix viewModel = View * Model;

    output.Normals = normalize(mul(Model, FinalNorm).xyz);

    output.FragPos = mul(Model, FinalPos).xyz;


    float3 T = normalize(mul(input.Tangents, (float3x3)Model));
    float3 B = normalize(mul(input.Bitangents, (float3x3)Model));
    float3 N = output.Normals;  
    // TBN must form a right handed coord system.
   // Some models have symetric UVs. Check and fix.
    if (dot(cross(N, T), B) < 0.0)
    {
      T = T * -1.0;
    }
    float3x3 TBN = float3x3(T, B, N);

    output.TBN = TBN;

    return output;
}