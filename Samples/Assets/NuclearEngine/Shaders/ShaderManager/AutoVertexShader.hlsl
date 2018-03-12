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
    float4 Position : POSITION0;
#ifdef NE_USE_UV
    float2 TexCoord : TEXCOORD0;
#endif
#ifdef NE_USE_NORMALS
    float3 Normals : NORMAL0;
#endif
#ifdef NE_USE_TANGENTS
    float3 Tangents : TANGENT0;
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
cbuffer NE_Camera : register(b0)
{
    matrix Model;
    matrix View;
    matrix Projection;
};
#endif

PixelInputType main(VertexInputType input)
{
    PixelInputType output;

#ifdef NE_USE_DEF_CAMERA
	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.Position = mul(Model, input.Position);
    output.Position = mul(View, output.Position);
    output.Position = mul(Projection, output.Position);
#else
    output.Position = input.Position;
#endif

#ifdef NE_USE_UV
    output.TexCoord = input.TexCoord;
#endif
#ifdef NE_USE_NORMALS
    output.Normals = input.Normals;
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