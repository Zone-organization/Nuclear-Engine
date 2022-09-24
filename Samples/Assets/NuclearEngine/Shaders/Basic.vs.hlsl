/*
Defines:
NE_DEFFERED

*/

#include <CommonInput.vs.hlsl>
#include <CommonInput.ps.hlsl>


PixelInputType main(VertexInputType input)
{
    PixelInputType output;

#ifndef NE_DEFFERED

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
    output.FragPos = mul(Model, FinalPos).xyz;
    output.Normals = normalize(mul(Model, FinalNorm).xyz);

    float3 T = normalize(mul(input.Tangents, (float3x3)Model));
    float3 B = normalize(mul(input.Bitangents, (float3x3)Model));
    float3 N = output.Normals;
    // TBN must form a right handed coord system.
   // Some models have symetric UVs. Check and fix.
    if (dot(cross(N, T), B) < 0.0)
    {
        T = T * -1.0;
    }
    output.TBN = float3x3(T, B, N);
	
#ifdef NE_SHADOWS

#ifdef NE_MAX_DIR_CASTERS
    for (int i0 = 0; i0 < NE_MAX_DIR_CASTERS; i0++)
    {
        output.DirLight_FragPos[i0] = mul(Dir_LightSpace[i0], float4(output.FragPos, 1.0f));
    }
#endif

#ifdef NE_MAX_SPOT_CASTERS
	for (int i1 = 0; i1 < NE_MAX_SPOT_CASTERS; i1++)
    {
        //output.SpotLight_FragPos[i0] = mul(Spot_LightSpace[i0], float4(output.FragPos, 1.0f));

		output.SpotLight_FragPos[i1] = mul(float4(output.FragPos, 1.0f), Spot_LightSpace[i1]);
    }
#endif

#endif
	
#else

    output.Position = input.Position;
#endif
    output.TexCoord = input.TexCoord;

    return output;
}