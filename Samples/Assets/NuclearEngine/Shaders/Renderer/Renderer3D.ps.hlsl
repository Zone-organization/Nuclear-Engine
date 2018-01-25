

float4 main(PixelInputType input) : SV_TARGET
{ 
    float4 result = float4(0, 0, 0, 0);


    #ifdef NE_LIGHT_SHADING_TECH
    	result =  NE_Tex_Diffuse1.Sample(NE_Diffuse1_Sampler, input.TexCoord);
    #endif
    
    return result;
 }