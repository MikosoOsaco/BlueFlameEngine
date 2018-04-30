#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 Colour;

void main()
{
    FragColor = vec4(Colour, 1.0); // set alle 4 vector values to 1.0
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 0.9)
        BrightColor = vec4(FragColor.rgb, 1.0);
}