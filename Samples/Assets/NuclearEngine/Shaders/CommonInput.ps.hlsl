
struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
#ifndef NE_DEFFERED
    float3 Normals : NORMAL0;
    float3 FragPos : TEXCOORD1;
    float3x3 TBN : TANGENT0;
#endif
//Shadows
#ifdef NE_SHADOWS

#ifdef NE_MAX_DIR_CASTERS  //Spotlights that casts shadows
    float4 DirLight_FragPos[NE_MAX_DIR_CASTERS] : COLOR;
#endif

#ifdef NE_MAX_SPOT_CASTERS  //Spotlights that casts shadows
	float4 SpotLight_FragPos[NE_MAX_SPOT_CASTERS] : COLOR;
#endif

#endif
};