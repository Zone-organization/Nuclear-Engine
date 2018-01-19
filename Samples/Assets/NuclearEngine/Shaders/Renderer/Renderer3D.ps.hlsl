Texture2D NE_Diffuse_Tex : register(t0);
SamplerState NE_Diffuse_Sampler : register(s0);

Texture2D NE_Specular_Tex : register(t1);
SamplerState NE_Specular_Sampler : register(s1);

struct PixelInputType
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
    float3 FragPos : TEXCOORD1;
};

struct Light
{
	float4 Position;
	float4 Direction;
    float4 Intensity_Attenuation;
	float4 InnerCutOf_OuterCutoff;
	float4 Color;
};

//#ifdef NE_LIGHT_SHADING_TECH
cbuffer NE_Light_CB
{
    float4 EyePosition_MaterialShininess;
	Light Lights[NE_LIGHTS_NUM];
};
//#endif

struct LightingParams
{
    Light light;
    float3 Normal;
    float2 TexCoord;
    float3 FragPos;
    float3 ViewDir;
};

// calculates the color when using a directional light.
float3 CalcDirLight(LightingParams input)
{
    float3 lightDir = normalize(-input.light.Direction).xyz;
    // diffuse shading
    float diff = max(dot(input.Normal, lightDir), 0.0);
    // specular shading
    float3 reflectDir = reflect(-lightDir, input.Normal);
    float spec = pow(max(dot(input.ViewDir, reflectDir), 0.0), EyePosition_MaterialShininess.w);
    // combine results
    float4 ambient = (0.1f * input.light.Color) * NE_Diffuse_Tex.Sample(NE_Diffuse_Sampler, input.TexCoord);
    float4 diffuse = input.light.Color * diff * NE_Diffuse_Tex.Sample(NE_Diffuse_Sampler, input.TexCoord);
    float4 specular = input.light.Color * spec * NE_Specular_Tex.Sample(NE_Specular_Sampler, input.TexCoord);

    return (ambient + diffuse + specular).xyz;
}

//// calculates the color when using a point light.
float3 CalcPointLight(LightingParams input)
{
    float3 lightDir = normalize(input.light.Position.xyz - input.FragPos);
    // diffuse shading
    float diff = max(dot(input.Normal, lightDir), 0.0);
    // specular shading
    float3 reflectDir = reflect(-lightDir, input.Normal);
    float spec = pow(max(dot(input.ViewDir, reflectDir), 0.0), EyePosition_MaterialShininess.w);
    // attenuation
    float distance = length(input.light.Position.xyz - input.FragPos);
    
    float attenuation = input.light.Intensity_Attenuation.x / (input.light.Intensity_Attenuation.y + input.light.Intensity_Attenuation.z * distance + input.light.Intensity_Attenuation.w * (distance * distance));

    // combine results
    float4 ambient = (0.1f * input.light.Color) * NE_Diffuse_Tex.Sample(NE_Diffuse_Sampler, input.TexCoord);
    float4 diffuse = input.light.Color * diff * NE_Diffuse_Tex.Sample(NE_Diffuse_Sampler, input.TexCoord);
    float4 specular = input.light.Color * spec * NE_Specular_Tex.Sample(NE_Specular_Sampler, input.TexCoord);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular).xyz;
}

//// calculates the color when using a spot light.
//float3 CalcSpotLight(Light light, float3 normal, float3 fragPos, float3 viewDir)
//{
//    float3 lightDir = normalize(light.position - fragPos);
//    // diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);
//    // specular shading
//    float3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    // attenuation
//    float distance = length(light.position - fragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
//    // spotlight intensity
//    float theta = dot(lightDir, normalize(-light.direction)); 
//    float epsilon = light.cutOff - light.outerCutOff;
//    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//    // combine results
//    float3 ambient = light.ambient * float3(texture(material.diffuse, TexCoords));
//    float3 diffuse = light.diffuse * diff * float3(texture(material.diffuse, TexCoords));
//    float3 specular = light.specular * spec * float3(texture(material.specular, TexCoords));
//    ambient *= attenuation * intensity;
//    diffuse *= attenuation * intensity;
//    specular *= attenuation * intensity;
//    return (ambient + diffuse + specular);
//}


float4 DoLighting(LightingParams Input)
{
    float3 result;
 
    [branch]
    if(Input.light.Position.w == 0)
    {
        result = CalcDirLight(Input);
    }
    else if (Input.light.Position.w == 1)
    {
       result = CalcPointLight(Input);
    }
    
    return float4(result, 1.0);
}

float4 main(PixelInputType input) : SV_TARGET
{ 
    float4 result = float4(0, 0, 0, 0);

    LightingParams lightfuncparams;
    lightfuncparams.FragPos = input.FragPos;
    lightfuncparams.Normal = normalize(input.Normal);
    lightfuncparams.ViewDir = normalize(EyePosition_MaterialShininess.xyz - input.FragPos);
    lightfuncparams.TexCoord = input.TexCoord;

    [unroll]
    for (unsigned int i = 0; i < NE_LIGHTS_NUM; i++)
    {
        lightfuncparams.light = Lights[i];
        result += DoLighting(lightfuncparams);
    }

    return result;
 }