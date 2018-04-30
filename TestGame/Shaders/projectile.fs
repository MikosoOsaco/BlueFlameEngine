#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    float constant;
    float linear;
    float quadratic;

    vec3 position;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
 
in vec3 Position;
in vec2 TexCoords;
in vec3 Normal;

#define numDirLights 10
#define numPointLights 10

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

uniform vec3 Colour;

uniform vec3 viewPos;

uniform DirLight dirLight[numDirLights];
uniform PointLight pointLight[numPointLights];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - Position);
    vec3 result = vec3(0,0,0);

	for(int i = 0; i < numPointLights; i++) {
		result += CalcPointLight(pointLight[i], norm, Position, viewDir);   
	} 
	
	result += CalcDirLight(dirLight[0], norm, viewDir);
	
	// check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(result + Colour, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(result + Colour, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	
    FragColor = vec4(result + Colour, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
	vec3 normalt = light.specular * spec * vec3(texture(texture_normal1, TexCoords));
	//float shadow = ShadowCalculation(FragPosLightSpace);  	
    return (ambient + diffuse + specular + normalt);
	//return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0);//material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
	vec3 normalt = light.specular * spec * vec3(texture(texture_normal1, TexCoords));
	// If light is 0 do not add to other lights
	if(ambient != vec3(0,0,0)) {
		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;
		normalt *= attenuation;
	}
    return (ambient + diffuse + specular + normalt);
}