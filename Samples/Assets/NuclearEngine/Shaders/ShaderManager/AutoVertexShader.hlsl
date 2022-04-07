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
    int4 BoneIDs : ATTRIB4;
    float4 Weights : ATTRIB5;
};


struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 Normals : NORMAL0;
    float3 FragPos : TEXCOORD1;
    float3x3 TBN : TANGENT0;
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

#define MAX_BONES  100
#define MAX_BONE_INFLUENCE 4

cbuffer NEStatic_Animation : register(b1)
{
    matrix finalBonesMatrices[MAX_BONES];
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;

#ifdef NE_USE_DEF_CAMERA

    float4 totalPosition = float4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if (input.BoneIDs[i] == -1)
        {
            totalPosition = input.Position;
            continue;
        }
        if (input.BoneIDs[i] >= MAX_BONES)
        {
            totalPosition = input.Position;
            break;
        }
        float4 localPosition = mul(finalBonesMatrices[input.BoneIDs[i]], float4(input.Position.x , input.Position.y, input.Position.z,1.0f));
        totalPosition += localPosition * input.Weights[i];
        float3 localNormal = mul((float3x3)finalBonesMatrices[input.BoneIDs[i]], input.Normals);

    }

    output.Position = mul(ModelViewProjection, totalPosition);


    // Calculate the position of the vertex against the world, view, and projection matrices.
    //output.Position = mul(ModelViewProjection, input.Position);
#else
    output.Position = input.Position;
#endif

    output.TexCoord = input.TexCoord;


#if defined(NE_USE_DEF_CAMERA)
    output.Normals = mul((float3x3)ModelInvTranspose, input.Normals);
#else
    output.Normals = input.Normals;
#endif


#ifdef NE_OUT_FRAG_POS
    output.FragPos = mul(Model, input.Position).xyz;
#endif

    float3 T = normalize(mul(float4(input.Tangents.xyz, 0.0f), Model).xyz);
    float3 N = normalize(mul(float4(input.Normals.xyz, 0.0f), Model).xyz);
    //Gram-Schmidt process
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    float3 B = cross(N, T);
    
    output.TBN = float3x3(T, B, N);

    return output;
}