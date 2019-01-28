/*
Defines:
#define NE_USE_UV
#define NE_USE_NORMALS
#define NE_USE_TANGENTS
#define NE_USE_DEF_CAMERA
#define NE_OUT_FRAG_POS

*/
//#define NE_USE_UV
//#define NE_USE_NORMALS
//#define NE_USE_TANGENTS
//#define NE_USE_DEF_CAMERA
//#define NE_OUT_FRAG_POS

struct VertexInputType
{
    float4 Position : ATTRIB0;
#ifdef NE_USE_UV
    float2 TexCoord : ATTRIB1;
#endif
#ifdef NE_USE_NORMALS
    float3 Normals : ATTRIB2;
#endif
#ifdef NE_USE_TANGENTS
    float3 Tangents : ATTRIB3;
#endif
};

struct PixelInputType
{
    float4 Position : SV_POSITION;
#ifdef NE_USE_UV
    float2 TexCoord : TEXCOORD0;
#endif
#ifdef NE_USE_NORMALS
    float3 Normals : NORMAL0;
#endif
#ifdef NE_OUT_FRAG_POS
    float3 FragPos : TEXCOORD1;
#endif
#ifdef NE_USE_TANGENTS
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

PixelInputType main(VertexInputType input)
{
    PixelInputType output;

#ifdef NE_USE_DEF_CAMERA
	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.Position = mul(ModelViewProjection, input.Position);
#else
    output.Position = input.Position;
#endif

#ifdef NE_USE_UV
    output.TexCoord = input.TexCoord;
#endif

#if defined(NE_USE_NORMALS) && !defined(NE_USE_DEF_CAMERA)
    output.Normals = input.Normals;
#endif

#if defined(NE_USE_NORMALS) && defined(NE_USE_DEF_CAMERA)
    output.Normals = mul((float3x3)ModelInvTranspose, input.Normals);
#endif

#ifdef NE_OUT_FRAG_POS
    output.FragPos = mul(Model, input.Position).xyz;
#endif
#ifdef NE_USE_TANGENTS
    float3 T = normalize(mul(float4(input.Tangents.xyz, 0.0f), Model).xyz);
    float3 N = normalize(mul(float4(input.Normals.xyz, 0.0f), Model).xyz);
    //Gram-Schmidt process
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    float3 B = cross(N, T);
    
    output.TBN = float3x3(T, B, N);

#endif
    return output;
}