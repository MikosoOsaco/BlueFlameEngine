//Set the type of OpenGL you will be using
//3.3 is common on most computer
#version 330 core

//The location is like public variables that you can 
//store data in.  These variables will act like constants.
//If you notice, this matches our vertex structure!
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

//This is the output of the vertex shader
//This data will be sent to the linked fragment shader
//The variable name MUST match to link properly
out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	
	//Set the position of the fragment using the position from the layout
	gl_Position = projection * view * model * vec4(position, 1.0f);
	//Set the output colour using the input colour from the layout
	ourColor = color;
}

