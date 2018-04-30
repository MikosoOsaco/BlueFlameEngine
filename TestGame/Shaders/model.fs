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
in vec4 FragPosLightSpace;

#define numDirLights 10
#define numPointLights 10

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

uniform sampler2D shadowMap;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform DirLight dirLight[numDirLights];
uniform PointLight pointLight[numPointLights];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace);

void main()
{    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - Position);
    vec3 result = vec3(0,0,0);

	for(int i = 0; i < numPointLights; i++) {
		result += CalcPointLight(pointLight[i], norm, Position, viewDir);   
	} 
	
    for(int i = 0; i < numDirLights; i++) {
		result += CalcDirLight(dirLight[i], norm, viewDir);
	}   
	
	// check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(result + Colour, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	
    FragColor = vec4(result + Colour, 1.0);
	
	//Display only colours
	//FragColor = vec4(Colour, 1.0);
    
	// Display bone weights
	//vec4 weightsColor = vec4(weight.xyz,1.0);
	//FragColor = weightsColor;
	
	// Display only textures
    //FragColor = texture(texture_diffuse1, TexCoords);
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
	
	// TOON SHADING
	/*
	vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	if((max(dot(lightDir, normal), 0.0) > 0)) {
		spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0);//material.shininess);
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
	else
		return vec3(0,0,0);
	*/
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}