
float4 main(PixelInputType input) : SV_TARGET
{ 
    float4 result = float4(0.0f, 0.0f, 0.0f, 0.0f);

    #ifdef NE_LIGHT_SHADING_TECH
        result = DoLighting(input.Normal, input.FragPos, ViewPos, input.TexCoord);
    #endif
    
    return result;
 }