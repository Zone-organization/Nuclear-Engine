
struct VertexInputType
{
    float4 Position : ATTRIB0;
    float2 TexCoord : ATTRIB1;
#ifndef NE_DEFFERED
    float3 Normals : ATTRIB2;
    float3 Tangents : ATTRIB3;
    float3 Bitangents : ATTRIB4;
    int4 BoneIDs : ATTRIB5;
    float4 Weights : ATTRIB6;
#endif

};

#ifdef NE_SHADOWS

cbuffer NEStatic_ShadowCasters
{

#ifdef NE_MAX_SPOT_CASTERS  //Spotlights that casts shadows
	matrix Spot_LightSpace[NE_MAX_SPOT_CASTERS];
#endif


};

#endif


#ifndef NE_DEFFERED
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
#endif