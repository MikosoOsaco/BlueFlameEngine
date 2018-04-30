#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 5) in vec3 sColour;
layout (location = 6) in vec4 sWeights;
layout (location = 7) in ivec4 sIDs;

out vec3 Position;
out vec2 TexCoords;
out vec3 Normal;
out vec3 Colour;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float Time;
uniform mat4 lightSpaceMatrix;

const int MAX_BONES = 100;
uniform mat4 gBones[MAX_BONES];

out vec4 weight;
out vec4 id;

void main()
{
	mat4 BMatrix = gBones[sIDs[0]] * sWeights[0];
         BMatrix += gBones[sIDs[1]] * sWeights[1];
         BMatrix += gBones[sIDs[2]] * sWeights[2];
         BMatrix += gBones[sIDs[3]] * sWeights[3];

    Position = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;    
    Colour = sColour;
	weight = sWeights;
    id = sIDs;
    Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPosLightSpace = lightSpaceMatrix * vec4(Position, 1.0);	
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}