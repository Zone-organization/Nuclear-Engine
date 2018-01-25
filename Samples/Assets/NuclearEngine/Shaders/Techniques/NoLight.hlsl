
float4 DoLighting(float3 Normal, float3 FragPos, float3 viewpos, float2 TexCoords)
{
    return NE_Tex_Diffuse1.Sample(NE_Diffuse1_Sampler, TexCoords);
}