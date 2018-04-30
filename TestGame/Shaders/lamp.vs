#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 5) in vec3 sColour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Colour;

void main()
{
	Colour = sColour;
	if(sColour == vec3(0,0,0))
		Colour = vec3(1,1,1);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}