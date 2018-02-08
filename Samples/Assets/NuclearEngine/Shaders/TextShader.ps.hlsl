/*
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D NE_TextTexture;

layout(std140) uniform NE_TextColor_CB
{
    vec4 textColor;
};

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(NE_TextTexture, TexCoords).r);
    color = textColor * sampled;
}  
*/
struct PixelInputType
{    
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
};
cbuffer NE_TextColor_CB
{
	float4 textColor;
};

Texture2D NE_TextTexture : register(t0);;
SamplerState NE_TextTextureSampler : register(s0);

float4 main(PixelInputType input) : SV_Target
{        

    float4 sampled = float4(1.0f, 1.0f, 1.0f, NE_TextTexture.Sample(NE_TextTextureSampler, input.TexCoord).r);
	return textColor * sampled;
}  