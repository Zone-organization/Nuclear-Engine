struct PixelInputType
{
    float4 Position : SV_POSITION;
	float3 Tex : TEXCOORD;
};

TextureCube NE_SkyboxTexture : register(t0);;
SamplerState NE_SkyboxSampler : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{
    return NE_SkyboxTexture.Sample(NE_SkyboxSampler, input.Tex);
}
//
//#version 330 core
//out vec4 FragColor;
//in vec3 TexCoords;
//uniform samplerCube NE_SkyboxTexture;
//void main()
//{
//    FragColor = texture(NE_SkyboxTexture, TexCoords);
//}