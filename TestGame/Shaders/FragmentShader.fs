#version 330 core

//This will get the ourColor variable from the Vertex shader
//Once again the variable name MUST match the vertex shader
in vec3 ourColor;

//This will be the final colour of our fragment
out vec4 color;


void main(){
	
	//Color the fragment this colour!
	color = vec4(ourColor, 1.0);

}