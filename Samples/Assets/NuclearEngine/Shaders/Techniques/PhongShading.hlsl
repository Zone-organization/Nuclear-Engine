
#define Shininess 64.0f
// calculates the color when using a directional light.
float3 CalcDirLight(DirLight light, float3 normal, float3 viewDir, float2 TexCoords)
{
    float3 lightDir = normalize(-light.Direction.xyz);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    // combine results
    float3 ambient = 0.05f * float3(light.Color.xyz * NE_Tex_Diffuse1.Sample(NE_Diffuse1_Sampler, TexCoords).xyz);
    float3 diffuse = light.Color.xyz * diff * NE_Tex_Diffuse1.Sample(NE_Diffuse1_Sampler, TexCoords).xyz;
    float3 specular = light.Color.xyz * spec * NE_Tex_Specular1.Sample(NE_Specular1_Sampler, TexCoords).xyz;
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
float3 CalcPointLight(PointLight light, float3 normal, float3 fragPos, float3 viewDir, float2 TexCoords)
{
    float3 lightDir = normalize(light.Position.xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    // attenuation
    float distance = length(light.Position.xyz - fragPos);
    float attenuation = light.Intensity_Attenuation.x / (light.Intensity_Attenuation.y + light.Intensity_Attenuation.z * distance + light.Intensity_Attenuation.w * (distance * distance));

    // combine results
    float3 ambient = 0.05f * float3(light.Color.xyz * NE_Tex_Diffuse1.Sample(NE_Diffuse1_Sampler, TexCoords).xyz);
    float3 diffuse = light.Color.xyz * diff * NE_Tex_Diffuse1.Sample(NE_Diffuse1_Sampler, TexCoords).xyz;
    float3 specular = light.Color.xyz * spec * NE_Tex_Specular1.Sample(NE_Specular1_Sampler, TexCoords).xyz;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
float3 CalcSpotLight(SpotLight light, float3 normal, float3 fragPos, float3 viewDir, float2 TexCoords)
{
    float3 lightDir = normalize(light.Position.xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    // attenuation
    float distance = length(light.Position.xyz - fragPos);
    float attenuation = light.Intensity_Attenuation.x / (light.Intensity_Attenuation.y + light.Intensity_Attenuation.z * distance + light.Intensity_Attenuation.w * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.Direction.xyz));
    float epsilon = light.InnerCutOf_OuterCutoff.x - light.InnerCutOf_OuterCutoff.y;
    float intensity = clamp((theta - light.InnerCutOf_OuterCutoff.y) / epsilon, 0.0, 1.0);
    // combine results
    float3 ambient = 0.05f * float3(light.Color.xyz * NE_Tex_Diffuse1.Sample(NE_Diffuse1_Sampler, TexCoords).xyz);
    float3 diffuse = light.Color.xyz * diff * NE_Tex_Diffuse1.Sample(NE_Diffuse1_Sampler, TexCoords).xyz;
    float3 specular = light.Color.xyz * spec * NE_Tex_Specular1.Sample(NE_Specular1_Sampler, TexCoords).xyz;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse);
}

float4 DoLighting(float3 Normal, float3 FragPos, float3 viewpos, float2 TexCoords)
{
    // properties
    float3 norm = normalize(Normal);
    float3 viewDir = normalize(viewpos - FragPos);
    float3 result = float3(0.0f, 0.0f, 0.0f);

#ifdef NE_DIR_LIGHTS_NUM
  // phase 1: directional lighting
    for (int i0 = 0; i0 < NE_DIR_LIGHTS_NUM; i0++)
    {
        result += CalcDirLight(DirLights[i0], norm, viewDir, TexCoords);
    }
#endif
#ifdef NE_POINT_LIGHTS_NUM  
    // phase 2: point lights
    for (int i1 = 0; i1 < NE_POINT_LIGHTS_NUM; i1++)
    {
        result += CalcPointLight(PointLights[i1], norm, FragPos, viewDir, TexCoords);
    }
#endif
#ifdef NE_SPOT_LIGHTS_NUM
    // phase 3: spot light
    for (int i2 = 0; i2 < NE_SPOT_LIGHTS_NUM; i2++)
    {
    
        result += CalcSpotLight(SpotLights[i2], norm, FragPos, viewDir, TexCoords);
    }
#endif

    return float4(result, 1.0f);
}
