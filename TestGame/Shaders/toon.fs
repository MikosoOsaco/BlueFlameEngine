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
in vec3 Colour;
in vec4 weight;
in vec4 id;

#define numDirLights 10
#define numPointLights 10

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 viewPos;
uniform DirLight dirLight[numDirLights];
uniform PointLight pointLight[numPointLights];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    vec3 norm = normalize(Normal);
    
	float intensity;
	vec4 color;
	intensity = dot(dirLight[0].direction,norm);

	if (intensity > 95)
		color = vec4(1.2,1.2,1.2,1.0);
	else if (intensity > 0.5)
		color = vec4(1.0,1.0,1.0,1.0);
	else if (intensity > 0.25)
		color = vec4(0.6,0.6,0.6,1.0);
	else
		color = vec4(0.3,0.3,0.3,1.0);
	
	FragColor = texture(texture_diffuse1, TexCoords);
	FragColor *= color;
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
    return (ambient + diffuse + specular);
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
	// If light is 0 do not add to other lights
	if(ambient != vec3(0,0,0)) {
		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;
	}
    return (ambient + diffuse + specular);
}