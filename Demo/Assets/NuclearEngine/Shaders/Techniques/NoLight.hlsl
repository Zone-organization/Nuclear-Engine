
float4 DoLighting(float2 TexCoords)
{
	return NE_Diffuse_Tex.Sample(NE_Diffuse_Sampler, TexCoords);
}
