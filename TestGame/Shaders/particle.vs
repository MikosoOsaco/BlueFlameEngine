#version 330 core
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 5) in vec3 sColour;

out vec2 TexCoords;
out vec4 Colour;
out float eTime;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float Time;
uniform vec3 inColour;
uniform float Alpha;

void main() {
	TexCoords = aTexCoords;

    float t = Time;
	
	eTime = Time;
	
	vec3 vertPos = (aNormal * t) + (0.5*(vec3(0.0, -9.8, 0.0) * (t * t)));

    Colour = vec4(sColour, Alpha);

	gl_Position = projection * view * model * vec4(vertPos, 1.0);	
}