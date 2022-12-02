struct VertexInputType
{
    float4 Position : ATTRIB0;
    float2 TexCoord : ATTRIB1;
};


struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
	float2 texCoord4 : TEXCOORD4;
	float2 texCoord5 : TEXCOORD5;
	float2 texCoord6 : TEXCOORD6;
	float2 texCoord7 : TEXCOORD7;
	float2 texCoord8 : TEXCOORD8;
	float2 texCoord9 : TEXCOORD9;
};

cbuffer ScreenSizeBuffer
{
    int4 ScreenSize;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;

    output.position = input.Position;
    output.tex = input.TexCoord;

	float texelSize;
#ifdef HORIZENTAL
	// Determine the floating point size of a texel for a screen with this specific width.
	texelSize = 1.0f / ScreenSize.x;

	// Create UV coordinates for the pixel and its four horizontal neighbors on either side.
	output.texCoord1 = input.TexCoord + float2(texelSize * -4.0f, 0.0f);
	output.texCoord2 = input.TexCoord + float2(texelSize * -3.0f, 0.0f);
	output.texCoord3 = input.TexCoord + float2(texelSize * -2.0f, 0.0f);
	output.texCoord4 = input.TexCoord + float2(texelSize * -1.0f, 0.0f);
	output.texCoord5 = input.TexCoord + float2(texelSize * 0.0f, 0.0f);
	output.texCoord6 = input.TexCoord + float2(texelSize * 1.0f, 0.0f);
	output.texCoord7 = input.TexCoord + float2(texelSize * 2.0f, 0.0f);
	output.texCoord8 = input.TexCoord + float2(texelSize * 3.0f, 0.0f);
	output.texCoord9 = input.TexCoord + float2(texelSize * 4.0f, 0.0f);
#else
	// Determine the floating point size of a texel for a screen with this specific height.
	texelSize = 1.0f / ScreenSize.y;

	// Create UV coordinates for the pixel and its four vertical neighbors on either side.
	output.texCoord1 = input.TexCoord + float2(0.0f, texelSize * -4.0f);
	output.texCoord2 = input.TexCoord + float2(0.0f, texelSize * -3.0f);
	output.texCoord3 = input.TexCoord + float2(0.0f, texelSize * -2.0f);
	output.texCoord4 = input.TexCoord + float2(0.0f, texelSize * -1.0f);
	output.texCoord5 = input.TexCoord + float2(0.0f, texelSize * 0.0f);
	output.texCoord6 = input.TexCoord + float2(0.0f, texelSize * 1.0f);
	output.texCoord7 = input.TexCoord + float2(0.0f, texelSize * 2.0f);
	output.texCoord8 = input.TexCoord + float2(0.0f, texelSize * 3.0f);
	output.texCoord9 = input.TexCoord + float2(0.0f, texelSize * 4.0f);
#endif


    return output;
}