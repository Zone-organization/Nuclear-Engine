#version 330 core
out vec4 FragColor;

in vec4 FragPos;
in vec4 Normal;
in vec2 TexCoords;

uniform sampler2D diffusemap;
uniform sampler2D specularmap;

vec4 ambientvalue = vec4(0.05f, 0.05f, 0.05f,1.0f);

struct DirLight {
	vec4 direction;
	vec4 color;
};

struct PointLight {
	vec4 position;
	vec4 Intensity_attenuation;
	vec4 color;
};

struct SpotLight {
	vec4 position;
	vec4 direction;
	vec4 Intensity_attenuation;
	vec4 SpotlightCutOf;
	vec4 color;
};

layout(std140) uniform NE_LightUBO
{
	vec4 viewPosandMaterialshininess;
#ifdef NR_DIR_LIGHTS
	DirLight dirLights[NR_DIR_LIGHTS];
#endif
#ifdef NR_POINT_LIGHTS
	PointLight pointLights[NR_POINT_LIGHTS];
#endif
#ifdef NR_SPOT_LIGHTS
	SpotLight spotLights[NR_SPOT_LIGHTS];
#endif
};

// function prototypes
vec4 CalcDirLight(DirLight light, vec4 normal, vec4 viewDir);
vec4 CalcPointLight(PointLight light, vec4 normal, vec4 fragPos, vec4 viewDir);
vec4 CalcSpotLight(SpotLight light, vec4 normal, vec4 fragPos, vec4 viewDir);

void main()
{
	// properties
	vec4 norm = normalize(Normal);
	vec4 viewDir = normalize(viewPosandMaterialshininess - FragPos);
	
	#ifdef NR_DIR_LIGHTS
	for (int i = 0; i < NR_DIR_LIGHTS; i++)
		FragColor = CalcDirLight(dirLights[i], norm, viewDir);
	#endif

	#ifdef NR_POINT_LIGHTS
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		FragColor += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	#endif

	#ifdef NR_SPOT_LIGHTS
	for (int i = 0; i < NR_SPOT_LIGHTS; i++)
		FragColor += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
	#endif

}

// calculates the color when using a directional light.
vec4 CalcDirLight(DirLight light, vec4 normal, vec4 viewDir)
{
	vec4 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec4 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), viewPosandMaterialshininess.w);
	// combine results
	vec4 ambient = ambientvalue * texture(diffusemap, TexCoords);
	vec4 diffuse = light.color * diff * texture(diffusemap, TexCoords);
	vec4 specular = light.color * spec * texture(specularmap, TexCoords);
	return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec4 CalcPointLight(PointLight light, vec4 normal, vec4 fragPos, vec4 viewDir)
{
	vec4 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec4 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), viewPosandMaterialshininess.w);
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = light.Intensity_attenuation.x / (light.Intensity_attenuation.y + light.Intensity_attenuation.z * distance + light.Intensity_attenuation.w * (distance * distance));
	// combine results
	vec4 ambient = ambientvalue * texture(diffusemap, TexCoords);
	vec4 diffuse = light.color * diff * texture(diffusemap, TexCoords);
	vec4 specular = light.color * spec * texture(specularmap, TexCoords);
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec4 CalcSpotLight(SpotLight light, vec4 normal, vec4 fragPos, vec4 viewDir)
{
	vec4 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec4 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), viewPosandMaterialshininess.w);
	// attenuation
	float distance = length(vec3(light.position) - vec3(fragPos));
	float attenuation = light.Intensity_attenuation.x / (light.Intensity_attenuation.y + light.Intensity_attenuation.z * distance + light.Intensity_attenuation.w * (distance * distance));
	// spotlight intensity
	float theta = dot(vec3(lightDir), normalize(-vec3(light.direction)));
	float epsilon = light.SpotlightCutOf.x - light.SpotlightCutOf.y;
	float intensity = clamp((theta - light.SpotlightCutOf.y) / epsilon, 0.0, 1.0);
	// combine results
	vec4 ambient = ambientvalue * texture(diffusemap, TexCoords);
	vec4 diffuse = light.color * diff * texture(diffusemap, TexCoords);
	vec4 specular = light.color * spec * texture(specularmap, TexCoords);
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}